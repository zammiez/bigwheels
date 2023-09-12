// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ppx/grfx/grfx_foveation.h"
#include "ppx/grfx/grfx_image.h"
#include "ppx/grfx/grfx_device.h"
#include "ppx/grfx/grfx_gpu.h"

namespace ppx {
namespace grfx {

Result FoveationPattern::CreateApiObjects(const grfx::FoveationPatternCreateInfo* pCreateInfo)
{
    grfx::FoveationCapabilities foveationCapabilites = GetDevice()->GetGpu()->GetFoveationCapabilities();
    mFoveationMode                                   = grfx::FOVEATION_NONE;

    if (pCreateInfo->foveationMode == grfx::FOVEATION_DENSITY_MAP) {
        grfx::TexturePtr foveationTexture;
        Result           ppxres = CreateDefaultTextureForDensityMap(pCreateInfo, &foveationTexture);
        if (Failed(ppxres)) {
            PPX_LOG_INFO("Failed to create default foveation textue.");
            return ppxres;
        }
        mFoveationTexture = foveationTexture;
        mFoveationMode    = pCreateInfo->foveationMode;
        return ppx::SUCCESS;
    }

    if (pCreateInfo->foveationMode == grfx::FOVEATION_VRS) {
        // TODO(zzong): implement VRS based foveation
        PPX_LOG_WARN("VRS based foveation not supported yet. Diable foveation.");
    }

    return ppx::SUCCESS;
}

void FoveationPattern::DestroyApiObjects()
{
    if (mFoveationTexture) {
        GetDevice()->DestroyTexture(mFoveationTexture);
        mFoveationTexture.Reset();
    }
}

Result FoveationPattern::CreateDefaultTextureForDensityMap(const grfx::FoveationPatternCreateInfo* pCreateInfo, grfx::Texture** ppFoveationTexture)
{
    grfx::FoveationCapabilities foveationCapabilites = GetDevice()->GetGpu()->GetFoveationCapabilities();
    if (!foveationCapabilites.densityMap.supported) {
        PPX_LOG_WARN("Density map not supported by GPU. Disable foveation.");
        return ppx::SUCCESS;
    }

    // Create foveation texture
    uint32_t texel_w = glm::clamp((uint32_t)16, foveationCapabilites.densityMap.texelSize.min.width, foveationCapabilites.densityMap.texelSize.max.width);
    uint32_t texel_h = glm::clamp((uint32_t)16, foveationCapabilites.densityMap.texelSize.min.height, foveationCapabilites.densityMap.texelSize.max.height);
    uint32_t w       = pCreateInfo->fbWidth / texel_w;
    uint32_t h       = pCreateInfo->fbHeight / texel_h;
    // uint32_t double_w = pCreateInfo->fbWidth * 2 / texel_w;
    // uint32_t double_h = pCreateInfo->fbHeight * 2 / texel_h;
    PPX_LOG_INFO("[zzong] 1:1 density map: " << w << "x" << h << ", with texel size of: " << texel_w << ", " << texel_h);
    grfx::TextureCreateInfo textureCreateInfo                = {};
    textureCreateInfo.imageType                              = grfx::IMAGE_TYPE_2D;
    textureCreateInfo.width                                  = w;
    textureCreateInfo.height                                 = h;
    textureCreateInfo.depth                                  = 1;
    textureCreateInfo.imageFormat                            = grfx::FORMAT_R8G8_UNORM;
    textureCreateInfo.sampleCount                            = grfx::SAMPLE_COUNT_1;
    textureCreateInfo.mipLevelCount                          = 1;
    textureCreateInfo.arrayLayerCount                        = 1;
    textureCreateInfo.usageFlags.bits.transferSrc            = false;
    textureCreateInfo.usageFlags.bits.transferDst            = true;
    textureCreateInfo.usageFlags.bits.sampled                = true;
    textureCreateInfo.usageFlags.bits.storage                = true;
    textureCreateInfo.usageFlags.bits.colorAttachment        = false;
    textureCreateInfo.usageFlags.bits.fragmentDensityMap     = true;
    textureCreateInfo.usageFlags.bits.depthStencilAttachment = false;
    textureCreateInfo.usageFlags.bits.inputAttachment        = false;
    textureCreateInfo.usageFlags.bits.transientAttachment    = false;

    Result ppxres = GetDevice()->CreateTexture(&textureCreateInfo, ppFoveationTexture);
    if (Failed(ppxres)) {
        PPX_ASSERT_MSG(false, "Foveation: density map texture creation failed");
        return ppxres;
    }

    // Create default foveation pattern content and copy to foveation image
    {
        struct R8G8
        {
            u_char r;
            u_char g;
        };
        std::vector<R8G8> densityMap(w * h);
        for (uint32_t y = 0; y < h; y++) {
            for (uint32_t x = 0; x < w; x++) {
                u_char   val = 255;
                uint32_t id  = y * w + x;

                float y_       = (float)y / (float)h - 0.5;
                float x_       = (float)x / float(w) - 0.5;
                float dist_sqr = x_ * x_ + y_ * y_;
                if (dist_sqr <= 0.04 * 0.04)
                    val = 255;
                else if (dist_sqr <= 0.25 * 0.25)
                    val = 120;
                else
                    val = 10;

                densityMap[id].r = val;
                densityMap[id].g = val;
            }
        }

        int uploadSize = w * h * sizeof(R8G8);

        grfx::BufferPtr        uploadBuffer;
        grfx::BufferCreateInfo bufferCreateInfo      = {};
        bufferCreateInfo.size                        = uploadSize;
        bufferCreateInfo.usageFlags.bits.transferSrc = true;
        bufferCreateInfo.memoryUsage                 = grfx::MEMORY_USAGE_CPU_TO_GPU;
        PPX_CHECKED_CALL(GetDevice()->CreateBuffer(&bufferCreateInfo, &uploadBuffer));
        PPX_CHECKED_CALL(uploadBuffer->CopyFromSource(uploadSize, densityMap.data()));

        grfx::BufferToImageCopyInfo copyInfo;
        copyInfo.srcBuffer = {
            .imageWidth      = w,
            .imageHeight     = h,
            .imageRowStride  = uint(sizeof(R8G8)) * w,
            .footprintOffset = 0,
            .footprintWidth  = w,
            .footprintHeight = h,
            .footprintDepth  = 1,
        };
        copyInfo.dstImage = {
            .mipLevel        = 0,
            .arrayLayer      = 0,
            .arrayLayerCount = 1,
            .x               = 0,
            .y               = 0,
            .z               = 0,
            .width           = w,
            .height          = h,
            .depth           = 1,
        };

        grfx::QueuePtr pQueue = GetDevice()->GetGraphicsQueue();
        PPX_CHECKED_CALL(pQueue->CopyBufferToImage(
            /*pCopyInfos=*/std::vector<grfx::BufferToImageCopyInfo>{copyInfo},
            /*pSrcBuffer=*/uploadBuffer,
            /*pDstImage=*/(*ppFoveationTexture)->GetImage(),
            /*mipLevel=*/0,
            /*mipLevelCount=*/1,
            /*arrayLayer=*/0,
            /*arrayLayerCount=*/1,
            /*stateBefore=*/RESOURCE_STATE_GENERAL,
            /*stateAfter=*/RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
    }
    return ppx::SUCCESS;
}

} // namespace grfx
} // namespace ppx