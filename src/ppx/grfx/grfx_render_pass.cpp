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

#include "ppx/grfx/grfx_render_pass.h"
#include "ppx/grfx/grfx_device.h"
#include "ppx/grfx/grfx_image.h"

namespace ppx {
namespace grfx {

// -------------------------------------------------------------------------------------------------
// RenderPassCreateInfo
// -------------------------------------------------------------------------------------------------
void RenderPassCreateInfo::SetAllRenderTargetClearValue(const grfx::RenderTargetClearValue& value)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetClearValues[i] = value;
    }
}

// -------------------------------------------------------------------------------------------------
// RenderPassCreateInfo2
// -------------------------------------------------------------------------------------------------
void RenderPassCreateInfo2::SetAllRenderTargetUsageFlags(const grfx::ImageUsageFlags& flags)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetUsageFlags[i] = flags;
    }
}

void RenderPassCreateInfo2::SetAllRenderTargetClearValue(const grfx::RenderTargetClearValue& value)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetClearValues[i] = value;
    }
}

void RenderPassCreateInfo2::SetAllRenderTargetLoadOp(grfx::AttachmentLoadOp op)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetLoadOps[i] = op;
    }
}

void RenderPassCreateInfo2::SetAllRenderTargetStoreOp(grfx::AttachmentStoreOp op)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetStoreOps[i] = op;
    }
}

void RenderPassCreateInfo2::SetAllRenderTargetToClear()
{
    SetAllRenderTargetLoadOp(grfx::ATTACHMENT_LOAD_OP_CLEAR);
}

// -------------------------------------------------------------------------------------------------
// RenderPassCreateInfo3
// -------------------------------------------------------------------------------------------------
void RenderPassCreateInfo3::SetAllRenderTargetClearValue(const grfx::RenderTargetClearValue& value)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetClearValues[i] = value;
    }
}

void RenderPassCreateInfo3::SetAllRenderTargetLoadOp(grfx::AttachmentLoadOp op)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetLoadOps[i] = op;
    }
}

void RenderPassCreateInfo3::SetAllRenderTargetStoreOp(grfx::AttachmentStoreOp op)
{
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetStoreOps[i] = op;
    }
}

void RenderPassCreateInfo3::SetAllRenderTargetToClear()
{
    SetAllRenderTargetLoadOp(grfx::ATTACHMENT_LOAD_OP_CLEAR);
}

// -------------------------------------------------------------------------------------------------
// internal
// -------------------------------------------------------------------------------------------------
namespace internal {

RenderPassCreateInfo::RenderPassCreateInfo(const grfx::RenderPassCreateInfo& obj)
{
    this->version           = CREATE_INFO_VERSION_1;
    this->width             = obj.width;
    this->height            = obj.height;
    this->renderTargetCount = obj.renderTargetCount;
    this->depthStencilState = obj.depthStencilState;

    // Views
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->V1.pRenderTargetViews[i] = obj.pRenderTargetViews[i];
    }
    this->V1.pDepthStencilView = obj.pDepthStencilView;

    // Clear values
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetClearValues[i] = obj.renderTargetClearValues[i];
    }
    this->depthStencilClearValue = obj.depthStencilClearValue;
}

RenderPassCreateInfo::RenderPassCreateInfo(const grfx::RenderPassCreateInfo2& obj)
{
    this->version           = CREATE_INFO_VERSION_2;
    this->width             = obj.width;
    this->height            = obj.height;
    this->renderTargetCount = obj.renderTargetCount;

    // Formats
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->V2.renderTargetFormats[i] = obj.renderTargetFormats[i];
    }
    this->V2.depthStencilFormat = obj.depthStencilFormat;

    // Sample count
    this->V2.sampleCount = obj.sampleCount;

    // Usage flags
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->V2.renderTargetUsageFlags[i] = obj.renderTargetUsageFlags[i];
    }
    this->V2.depthStencilUsageFlags = obj.depthStencilUsageFlags;

    // Clear values
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetClearValues[i] = obj.renderTargetClearValues[i];
    }
    this->depthStencilClearValue = obj.depthStencilClearValue;

    // Load/store ops
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetLoadOps[i]  = obj.renderTargetLoadOps[i];
        this->renderTargetStoreOps[i] = obj.renderTargetStoreOps[i];
    }
    this->depthLoadOp    = obj.depthLoadOp;
    this->depthStoreOp   = obj.depthStoreOp;
    this->stencilLoadOp  = obj.stencilLoadOp;
    this->stencilStoreOp = obj.stencilStoreOp;

    // Initial states
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->V2.renderTargetInitialStates[i] = obj.renderTargetInitialStates[i];
    }
    this->V2.depthStencilInitialState = obj.depthStencilInitialState;
}

RenderPassCreateInfo::RenderPassCreateInfo(const grfx::RenderPassCreateInfo3& obj)
{
    this->version           = CREATE_INFO_VERSION_3;
    this->width             = obj.width;
    this->height            = obj.height;
    this->renderTargetCount = obj.renderTargetCount;
    this->depthStencilState = obj.depthStencilState;

    // Images
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->V3.pRenderTargetImages[i] = obj.pRenderTargetImages[i];
    }
    this->V3.pDepthStencilImage = obj.pDepthStencilImage;

    this->V3.pVRSImage = obj.pVRSImage;

    // Clear values
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetClearValues[i] = obj.renderTargetClearValues[i];
    }
    this->depthStencilClearValue = obj.depthStencilClearValue;

    // Load/store ops
    for (uint32_t i = 0; i < this->renderTargetCount; ++i) {
        this->renderTargetLoadOps[i]  = obj.renderTargetLoadOps[i];
        this->renderTargetStoreOps[i] = obj.renderTargetStoreOps[i];
    }
    this->depthLoadOp    = obj.depthLoadOp;
    this->depthStoreOp   = obj.depthStoreOp;
    this->stencilLoadOp  = obj.stencilLoadOp;
    this->stencilStoreOp = obj.stencilStoreOp;
}

} // namespace internal

// -------------------------------------------------------------------------------------------------
// RenderPass
// -------------------------------------------------------------------------------------------------
Result RenderPass::CreateImagesAndViewsV1(const grfx::internal::RenderPassCreateInfo* pCreateInfo)
{
    // Copy RTV and images
    for (uint32_t i = 0; i < pCreateInfo->renderTargetCount; ++i) {
        grfx::RenderTargetViewPtr rtv = pCreateInfo->V1.pRenderTargetViews[i];
        if (!rtv) {
            PPX_ASSERT_MSG(false, "RTV << " << i << " is null");
            return ppx::ERROR_UNEXPECTED_NULL_ARGUMENT;
        }
        if (!rtv->GetImage()) {
            PPX_ASSERT_MSG(false, "image << " << i << " is null");
            return ppx::ERROR_UNEXPECTED_NULL_ARGUMENT;
        }

        mRenderTargetViews.push_back(rtv);
        mRenderTargetImages.push_back(rtv->GetImage());
    }
    // Copy DSV and image
    if (!IsNull(pCreateInfo->V1.pDepthStencilView)) {
        grfx::DepthStencilViewPtr dsv = pCreateInfo->V1.pDepthStencilView;

        mDepthStencilView  = dsv;
        mDepthStencilImage = dsv->GetImage();
    }

    return ppx::SUCCESS;
}

Result RenderPass::CreateImagesAndViewsV2(const grfx::internal::RenderPassCreateInfo* pCreateInfo)
{
    // Create images
    {
        // RTV images
        for (uint32_t i = 0; i < pCreateInfo->renderTargetCount; ++i) {
            grfx::ResourceState initialState = grfx::RESOURCE_STATE_RENDER_TARGET;
            if (pCreateInfo->V2.renderTargetInitialStates[i] != grfx::RESOURCE_STATE_UNDEFINED) {
                initialState = pCreateInfo->V2.renderTargetInitialStates[i];
            }

            grfx::ImageCreateInfo imageCreateInfo = {};
            imageCreateInfo.type                  = grfx::IMAGE_TYPE_2D;
            imageCreateInfo.width                 = pCreateInfo->width;
            imageCreateInfo.height                = pCreateInfo->height;
            imageCreateInfo.depth                 = 1;
            imageCreateInfo.format                = pCreateInfo->V2.renderTargetFormats[i];
            imageCreateInfo.sampleCount           = pCreateInfo->V2.sampleCount;
            imageCreateInfo.mipLevelCount         = 1;
            imageCreateInfo.arrayLayerCount       = 1;
            imageCreateInfo.usageFlags            = pCreateInfo->V2.renderTargetUsageFlags[i];
            imageCreateInfo.memoryUsage           = grfx::MEMORY_USAGE_GPU_ONLY;
            imageCreateInfo.initialState          = grfx::RESOURCE_STATE_RENDER_TARGET;
            imageCreateInfo.RTVClearValue         = pCreateInfo->renderTargetClearValues[i];
            imageCreateInfo.ownership             = pCreateInfo->ownership;

            grfx::ImagePtr image;
            Result         ppxres = GetDevice()->CreateImage(&imageCreateInfo, &image);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "RTV image create failed");
                return ppxres;
            }

            mRenderTargetImages.push_back(image);
        }

        // DSV image
        if (pCreateInfo->V2.depthStencilFormat != grfx::FORMAT_UNDEFINED) {
            grfx::ResourceState initialState = grfx::RESOURCE_STATE_DEPTH_STENCIL_WRITE;
            if (pCreateInfo->V2.depthStencilInitialState != grfx::RESOURCE_STATE_UNDEFINED) {
                initialState = pCreateInfo->V2.depthStencilInitialState;
            }

            grfx::ImageCreateInfo imageCreateInfo = {};
            imageCreateInfo.type                  = grfx::IMAGE_TYPE_2D;
            imageCreateInfo.width                 = pCreateInfo->width;
            imageCreateInfo.height                = pCreateInfo->height;
            imageCreateInfo.depth                 = 1;
            imageCreateInfo.format                = pCreateInfo->V2.depthStencilFormat;
            imageCreateInfo.sampleCount           = pCreateInfo->V2.sampleCount;
            imageCreateInfo.mipLevelCount         = 1;
            imageCreateInfo.arrayLayerCount       = 1;
            imageCreateInfo.usageFlags            = pCreateInfo->V2.depthStencilUsageFlags;
            imageCreateInfo.memoryUsage           = grfx::MEMORY_USAGE_GPU_ONLY;
            imageCreateInfo.initialState          = initialState;
            imageCreateInfo.DSVClearValue         = pCreateInfo->depthStencilClearValue;
            imageCreateInfo.ownership             = pCreateInfo->ownership;

            grfx::ImagePtr image;
            Result         ppxres = GetDevice()->CreateImage(&imageCreateInfo, &image);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "DSV image create failed");
                return ppxres;
            }

            mDepthStencilImage = image;
        }
    }

    // Create views
    {
        // RTVs
        for (uint32_t i = 0; i < pCreateInfo->renderTargetCount; ++i) {
            grfx::ImagePtr image = mRenderTargetImages[i];

            grfx::RenderTargetViewCreateInfo rtvCreateInfo = {};
            rtvCreateInfo.pImage                           = image;
            rtvCreateInfo.imageViewType                    = grfx::IMAGE_VIEW_TYPE_2D;
            rtvCreateInfo.format                           = pCreateInfo->V2.renderTargetFormats[i];
            rtvCreateInfo.sampleCount                      = image->GetSampleCount();
            rtvCreateInfo.mipLevel                         = 0;
            rtvCreateInfo.mipLevelCount                    = 1;
            rtvCreateInfo.arrayLayer                       = 0;
            rtvCreateInfo.arrayLayerCount                  = 1;
            rtvCreateInfo.components                       = {};
            rtvCreateInfo.loadOp                           = pCreateInfo->renderTargetLoadOps[i];
            rtvCreateInfo.storeOp                          = pCreateInfo->renderTargetStoreOps[i];
            rtvCreateInfo.ownership                        = pCreateInfo->ownership;

            grfx::RenderTargetViewPtr rtv;
            Result                    ppxres = GetDevice()->CreateRenderTargetView(&rtvCreateInfo, &rtv);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "RTV create failed");
                return ppxres;
            }

            mRenderTargetViews.push_back(rtv);
        }

        // DSV
        if (pCreateInfo->V2.depthStencilFormat != grfx::FORMAT_UNDEFINED) {
            grfx::ImagePtr image = mDepthStencilImage;

            grfx::DepthStencilViewCreateInfo dsvCreateInfo = {};
            dsvCreateInfo.pImage                           = image;
            dsvCreateInfo.imageViewType                    = grfx::IMAGE_VIEW_TYPE_2D;
            dsvCreateInfo.format                           = pCreateInfo->V2.depthStencilFormat;
            dsvCreateInfo.mipLevel                         = 0;
            dsvCreateInfo.mipLevelCount                    = 1;
            dsvCreateInfo.arrayLayer                       = 0;
            dsvCreateInfo.arrayLayerCount                  = 1;
            dsvCreateInfo.components                       = {};
            dsvCreateInfo.depthLoadOp                      = pCreateInfo->depthLoadOp;
            dsvCreateInfo.depthStoreOp                     = pCreateInfo->depthStoreOp;
            dsvCreateInfo.stencilLoadOp                    = pCreateInfo->stencilLoadOp;
            dsvCreateInfo.stencilStoreOp                   = pCreateInfo->stencilStoreOp;
            dsvCreateInfo.ownership                        = pCreateInfo->ownership;

            grfx::DepthStencilViewPtr dsv;
            Result                    ppxres = GetDevice()->CreateDepthStencilView(&dsvCreateInfo, &dsv);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "RTV create failed");
                return ppxres;
            }

            mDepthStencilView = dsv;
        }
    }

    return ppx::SUCCESS;
}

Result RenderPass::CreateImagesAndViewsV3(const grfx::internal::RenderPassCreateInfo* pCreateInfo)
{
    // Copy images
    {
        // Copy RTV images
        for (uint32_t i = 0; i < pCreateInfo->renderTargetCount; ++i) {
            grfx::ImagePtr image = pCreateInfo->V3.pRenderTargetImages[i];
            if (!image) {
                PPX_ASSERT_MSG(false, "image << " << i << " is null");
                return ppx::ERROR_UNEXPECTED_NULL_ARGUMENT;
            }

            mRenderTargetImages.push_back(image);
        }
        // Copy DSV image
        if (!IsNull(pCreateInfo->V3.pDepthStencilImage)) {
            mDepthStencilImage = pCreateInfo->V3.pDepthStencilImage;
        }

        // Copy VRS image
        if (!IsNull(pCreateInfo->V3.pVRSImage)) {
            mVrsImage = pCreateInfo->V3.pVRSImage;
        }
    }

    // Create views
    {
        // RTVs
        for (uint32_t i = 0; i < pCreateInfo->renderTargetCount; ++i) {
            grfx::ImagePtr image = mRenderTargetImages[i];

            grfx::RenderTargetViewCreateInfo rtvCreateInfo = {};
            rtvCreateInfo.pImage                           = image;
            rtvCreateInfo.imageViewType                    = image->GuessImageViewType();
            rtvCreateInfo.format                           = image->GetFormat();
            rtvCreateInfo.sampleCount                      = image->GetSampleCount();
            rtvCreateInfo.mipLevel                         = 0;
            rtvCreateInfo.mipLevelCount                    = image->GetMipLevelCount();
            rtvCreateInfo.arrayLayer                       = 0;
            rtvCreateInfo.arrayLayerCount                  = image->GetArrayLayerCount();
            rtvCreateInfo.components                       = {};
            rtvCreateInfo.loadOp                           = pCreateInfo->renderTargetLoadOps[i];
            rtvCreateInfo.storeOp                          = pCreateInfo->renderTargetStoreOps[i];
            rtvCreateInfo.ownership                        = pCreateInfo->ownership;

            grfx::RenderTargetViewPtr rtv;
            Result                    ppxres = GetDevice()->CreateRenderTargetView(&rtvCreateInfo, &rtv);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "RTV create failed");
                return ppxres;
            }

            mRenderTargetViews.push_back(rtv);
        }

        // DSV
        if (mDepthStencilImage) {
            grfx::ImagePtr image = mDepthStencilImage;

            grfx::DepthStencilViewCreateInfo dsvCreateInfo = {};
            dsvCreateInfo.pImage                           = image;
            dsvCreateInfo.imageViewType                    = image->GuessImageViewType();
            dsvCreateInfo.format                           = image->GetFormat();
            dsvCreateInfo.mipLevel                         = 0;
            dsvCreateInfo.mipLevelCount                    = image->GetMipLevelCount();
            dsvCreateInfo.arrayLayer                       = 0;
            dsvCreateInfo.arrayLayerCount                  = image->GetArrayLayerCount();
            dsvCreateInfo.components                       = {};
            dsvCreateInfo.depthLoadOp                      = pCreateInfo->depthLoadOp;
            dsvCreateInfo.depthStoreOp                     = pCreateInfo->depthStoreOp;
            dsvCreateInfo.stencilLoadOp                    = pCreateInfo->stencilLoadOp;
            dsvCreateInfo.stencilStoreOp                   = pCreateInfo->stencilStoreOp;
            dsvCreateInfo.ownership                        = pCreateInfo->ownership;

            grfx::DepthStencilViewPtr dsv;
            Result                    ppxres = GetDevice()->CreateDepthStencilView(&dsvCreateInfo, &dsv);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "DSV create failed");
                return ppxres;
            }

            mDepthStencilView = dsv;
        }
        uint32_t w = 1 + pCreateInfo->width / VRS_TEXEL_W; // Todo: use texel size from got properties
        uint32_t h = 1 + pCreateInfo->height / VRS_TEXEL_H;
        // [VRS] Create VRS image. TODO: add support for assigning VRS image instead of create here
        if (mVrsImage.IsNull()) {
            grfx::ImageCreateInfo vrsImageCreateInfo                         = {};
            vrsImageCreateInfo.type                                          = grfx::IMAGE_TYPE_2D;
            vrsImageCreateInfo.width                                         = w;
            vrsImageCreateInfo.height                                        = h;
            vrsImageCreateInfo.depth                                         = 1;
            vrsImageCreateInfo.format                                        = grfx::FORMAT_R8_UINT;
            vrsImageCreateInfo.sampleCount                                   = grfx::SAMPLE_COUNT_1;
            vrsImageCreateInfo.mipLevelCount                                 = 1;
            vrsImageCreateInfo.arrayLayerCount                               = 1;
            vrsImageCreateInfo.usageFlags.bits.fragmentShadingRateAttachment = true;
            vrsImageCreateInfo.usageFlags.bits.transferSrc                   = false;
            vrsImageCreateInfo.usageFlags.bits.transferDst                   = true;
            vrsImageCreateInfo.usageFlags.bits.sampled                       = true;
            vrsImageCreateInfo.usageFlags.bits.storage                       = true;
            vrsImageCreateInfo.usageFlags.bits.colorAttachment               = true;

            grfx::ImagePtr vrsImage;
            Result         ppxres = GetDevice()->CreateImage(&vrsImageCreateInfo, &vrsImage);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "[VRS] VRS image create failed");
                return ppxres;
            }
            // [VRS] create vrs content and copy to vrs image
            {
                std::vector<uint8_t> vrs_values(w * h, 10);

                /*
                sizew = 2^((texel / 4) & 3)^
                sizeh = 2^(texel & 3)^
                */
                for (uint x = 0; x <= 2; ++x) {
                    for (uint y = 0; y <= 2; ++y) {
                        uint texel = (x << 2) + y;
                        PPX_LOG_INFO("[VRS] size{" << (1 << x) << "x" << (1 << y) << "} texel value: " << texel);
                    }
                }
                int size1x1 = 0;
                int size1x2 = 1;
                int size1x4 = 2;
                int size2x1 = 4;
                int size2x2 = 5;
                int size2x4 = 6;
                int size4x1 = 8;
                int size4x2 = 9;
                int size4x4 = 10;

                for (int x = 0; x < h; x++) {
                    for (int y = 0; y < w; y++) {
                        if (y < (w / 2))
                            vrs_values[x * w + y] = size4x4;
                        else
                            vrs_values[x * w + y] = size1x1;
                    }
                }
                int uploadSize = w * h * sizeof(uint8_t);

                grfx::BufferPtr        uploadBuffer;
                grfx::BufferCreateInfo bufferCreateInfo      = {};
                bufferCreateInfo.size                        = uploadSize;
                bufferCreateInfo.usageFlags.bits.transferSrc = true;
                bufferCreateInfo.memoryUsage                 = grfx::MEMORY_USAGE_CPU_TO_GPU;
                PPX_CHECKED_CALL(GetDevice()->CreateBuffer(&bufferCreateInfo, &uploadBuffer));

                PPX_CHECKED_CALL(uploadBuffer->CopyFromSource(uploadSize, vrs_values.data()));

                grfx::BufferToImageCopyInfo copyInfo;
                copyInfo.srcBuffer = {
                    .imageWidth      = w,
                    .imageHeight     = h,
                    .imageRowStride  = uint(sizeof(char)) * w,
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

                auto pQueue = GetDevice()->GetGraphicsQueue();
                pQueue->CopyBufferToImage(
                    /*pCopyInfos=*/std::vector<grfx::BufferToImageCopyInfo>{copyInfo},
                    /*pSrcBuffer=*/uploadBuffer,
                    /*pDstImage=*/vrsImage,
                    /*mipLevel=*/0,
                    /*mipLevelCount=*/1,
                    /*arrayLayer=*/0,
                    /*arrayLayerCount=*/1,
                    /*stateBefore=*/RESOURCE_STATE_GENERAL,
                    /*stateAfter=*/RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
            }

            mVrsImage = vrsImage;
            PPX_LOG_INFO("[VRS] VRS image created.");
        }

        // [VRS] create VRS image view
        if (!mVrsImage.IsNull() && mVrsImageView.IsNull()) {
            grfx::ImagePtr image = mVrsImage;

            grfx::SampledImageViewCreateInfo vrsCreateInfo = {};
            vrsCreateInfo.pImage                           = image;
            vrsCreateInfo.imageViewType                    = image->GuessImageViewType();
            vrsCreateInfo.format                           = image->GetFormat();
            vrsCreateInfo.mipLevel                         = 0;
            vrsCreateInfo.mipLevelCount                    = image->GetMipLevelCount();
            vrsCreateInfo.arrayLayer                       = 0;
            vrsCreateInfo.arrayLayerCount                  = image->GetArrayLayerCount();
            vrsCreateInfo.components                       = {};
            vrsCreateInfo.ownership                        = pCreateInfo->ownership;

            grfx::SampledImageViewPtr vrsImageView;
            Result                    ppxres = GetDevice()->CreateSampledImageView(&vrsCreateInfo, &vrsImageView);
            if (Failed(ppxres)) {
                PPX_ASSERT_MSG(false, "[VRS] Failed to create VRS image view.");
                return ppxres;
            }
            mVrsImageView = vrsImageView;
        }
    }

    return ppx::SUCCESS;
}

Result RenderPass::Create(const grfx::internal::RenderPassCreateInfo* pCreateInfo)
{
    mRenderArea = {0, 0, pCreateInfo->width, pCreateInfo->height};
    mViewport   = {0.0f, 0.0f, static_cast<float>(pCreateInfo->width), static_cast<float>(pCreateInfo->height), 0.0f, 1.0f};

    switch (pCreateInfo->version) {
        default: return ppx::ERROR_INVALID_CREATE_ARGUMENT; break;

        case grfx::internal::RenderPassCreateInfo::CREATE_INFO_VERSION_1: {
            Result ppxres = CreateImagesAndViewsV1(pCreateInfo);
            if (Failed(ppxres)) {
                return ppxres;
            }
        } break;

        case grfx::internal::RenderPassCreateInfo::CREATE_INFO_VERSION_2: {
            Result ppxres = CreateImagesAndViewsV2(pCreateInfo);
            if (Failed(ppxres)) {
                return ppxres;
            }
        } break;

        case grfx::internal::RenderPassCreateInfo::CREATE_INFO_VERSION_3: {
            Result ppxres = CreateImagesAndViewsV3(pCreateInfo);
            if (Failed(ppxres)) {
                return ppxres;
            }
        } break;
    }

    Result ppxres = grfx::DeviceObject<grfx::internal::RenderPassCreateInfo>::Create(pCreateInfo);
    if (Failed(ppxres)) {
        return ppxres;
    }

    return ppx::SUCCESS;
}

void RenderPass::Destroy()
{
    for (uint32_t i = 0; i < mCreateInfo.renderTargetCount; ++i) {
        grfx::RenderTargetViewPtr& rtv = mRenderTargetViews[i];
        if (rtv && (rtv->GetOwnership() != grfx::OWNERSHIP_REFERENCE)) {
            GetDevice()->DestroyRenderTargetView(rtv);
            rtv.Reset();
        }

        grfx::ImagePtr& image = mRenderTargetImages[i];
        if (image && (image->GetOwnership() != grfx::OWNERSHIP_REFERENCE)) {
            GetDevice()->DestroyImage(image);
            image.Reset();
        }
    }
    mRenderTargetViews.clear();
    mRenderTargetImages.clear();

    if (mDepthStencilView && (mDepthStencilView->GetOwnership() != grfx::OWNERSHIP_REFERENCE)) {
        GetDevice()->DestroyDepthStencilView(mDepthStencilView);
        mDepthStencilView.Reset();
    }

    if (mDepthStencilImage && (mDepthStencilImage->GetOwnership() != grfx::OWNERSHIP_REFERENCE)) {
        GetDevice()->DestroyImage(mDepthStencilImage);
        mDepthStencilImage.Reset();
    }

    grfx::DeviceObject<grfx::internal::RenderPassCreateInfo>::Destroy();
}

Result RenderPass::GetRenderTargetView(uint32_t index, grfx::RenderTargetView** ppView) const
{
    if (!IsIndexInRange(index, mRenderTargetViews)) {
        return ppx::ERROR_OUT_OF_RANGE;
    }
    *ppView = mRenderTargetViews[index];
    return ppx::SUCCESS;
}

Result RenderPass::GetDepthStencilView(grfx::DepthStencilView** ppView) const
{
    if (!mDepthStencilView) {
        return ppx::ERROR_ELEMENT_NOT_FOUND;
    }
    *ppView = mDepthStencilView;
    return ppx::SUCCESS;
}

Result RenderPass::GetRenderTargetImage(uint32_t index, grfx::Image** ppImage) const
{
    if (!IsIndexInRange(index, mRenderTargetImages)) {
        return ppx::ERROR_OUT_OF_RANGE;
    }
    *ppImage = mRenderTargetImages[index];
    return ppx::SUCCESS;
}

Result RenderPass::GetDepthStencilImage(grfx::Image** ppImage) const
{
    if (!mDepthStencilImage) {
        return ppx::ERROR_ELEMENT_NOT_FOUND;
    }
    *ppImage = mDepthStencilImage;
    return ppx::SUCCESS;
}

grfx::RenderTargetViewPtr RenderPass::GetRenderTargetView(uint32_t index) const
{
    grfx::RenderTargetViewPtr object;
    GetRenderTargetView(index, &object);
    return object;
}

grfx::DepthStencilViewPtr RenderPass::GetDepthStencilView() const
{
    grfx::DepthStencilViewPtr object;
    GetDepthStencilView(&object);
    return object;
}

grfx::ImagePtr RenderPass::GetRenderTargetImage(uint32_t index) const
{
    grfx::ImagePtr object;
    GetRenderTargetImage(index, &object);
    return object;
}

grfx::ImagePtr RenderPass::GetDepthStencilImage() const
{
    grfx::ImagePtr object;
    GetDepthStencilImage(&object);
    return object;
}

Result RenderPass::DisownRenderTargetView(uint32_t index, grfx::RenderTargetView** ppView)
{
    if (IsIndexInRange(index, mRenderTargetViews)) {
        return ppx::ERROR_OUT_OF_RANGE;
    }
    if (mRenderTargetViews[index]->GetOwnership() == grfx::OWNERSHIP_RESTRICTED) {
        return ppx::ERROR_GRFX_OBJECT_OWNERSHIP_IS_RESTRICTED;
    }

    mRenderTargetViews[index]->SetOwnership(grfx::OWNERSHIP_REFERENCE);

    if (!IsNull(ppView)) {
        *ppView = mRenderTargetViews[index];
    }
    return ppx::SUCCESS;
}

Result RenderPass::DisownDepthStencilView(grfx::DepthStencilView** ppView)
{
    if (!mDepthStencilView) {
        return ppx::ERROR_ELEMENT_NOT_FOUND;
    }
    if (mDepthStencilView->GetOwnership() == grfx::OWNERSHIP_RESTRICTED) {
        return ppx::ERROR_GRFX_OBJECT_OWNERSHIP_IS_RESTRICTED;
    }

    mDepthStencilView->SetOwnership(grfx::OWNERSHIP_REFERENCE);

    if (!IsNull(ppView)) {
        *ppView = mDepthStencilView;
    }
    return ppx::SUCCESS;
}

Result RenderPass::DisownRenderTargetImage(uint32_t index, grfx::Image** ppImage)
{
    if (IsIndexInRange(index, mRenderTargetImages)) {
        return ppx::ERROR_OUT_OF_RANGE;
    }
    if (mRenderTargetImages[index]->GetOwnership() == grfx::OWNERSHIP_RESTRICTED) {
        return ppx::ERROR_GRFX_OBJECT_OWNERSHIP_IS_RESTRICTED;
    }

    mRenderTargetImages[index]->SetOwnership(grfx::OWNERSHIP_REFERENCE);

    if (!IsNull(ppImage)) {
        *ppImage = mRenderTargetImages[index];
    }
    return ppx::SUCCESS;
}

Result RenderPass::DisownDepthStencilImage(grfx::Image** ppImage)
{
    if (!mDepthStencilImage) {
        return ppx::ERROR_ELEMENT_NOT_FOUND;
    }
    if (mDepthStencilImage->GetOwnership() == grfx::OWNERSHIP_RESTRICTED) {
        return ppx::ERROR_GRFX_OBJECT_OWNERSHIP_IS_RESTRICTED;
    }

    mDepthStencilImage->SetOwnership(grfx::OWNERSHIP_REFERENCE);

    if (!IsNull(ppImage)) {
        *ppImage = mDepthStencilImage;
    }
    return ppx::SUCCESS;
}

} // namespace grfx
} // namespace ppx
