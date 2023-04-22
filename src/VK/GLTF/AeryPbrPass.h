#pragma once

#include "GLTFTexturesAndBuffers.h"
#include "GltfPbrPass.h"
#include "PostProc/SkyDome.h"
#include "Base/GBuffer.h"
#include "../common/GLTF/GltfPbrMaterial.h"

namespace CAULDRON_VK
{
    class AeryPbrPass
    {
    public:
        struct per_object
        {
            math::Matrix4 mCurrentWorld;
            math::Matrix4 mPreviousWorld;

            PBRMaterialParametersConstantBuffer m_pbrParams;
        };

        struct BatchList
        {
            float m_depth;
            PBRPrimitives *m_pPrimitive;
            VkDescriptorBufferInfo m_perFrameDesc;
            VkDescriptorBufferInfo m_perObjectDesc;
            VkDescriptorBufferInfo *m_pPerSkeleton;
            operator float() { return -m_depth; }
        };

        enum class ShadowMode
        {
            None,
            HardShadow,
            PCF,
            PCSS,
            VSM,
            ModesNum
        };

        void OnCreate(
            Device* pDevice,
            UploadHeap* pUploadHeap,
            ResourceViewHeaps *pHeaps,
            DynamicBufferRing *pDynamicBufferRing,
            StaticBufferPool *pStaticBufferPool,
            GLTFTexturesAndBuffers *pGLTFTexturesAndBuffers,
            SkyDome *pSkyDome,
            bool bUseSSAOMask,
            std::vector<VkImageView>& ShadowMapViewPool,
            GBufferRenderPass *pRenderPass,
            AsyncPool *pAsyncPool = NULL,
            bool invertedDepth = false
        );

        void OnDestroy();
        void BuildBatchLists(std::vector<BatchList> *pSolid, std::vector<BatchList> *pTransparent, bool bWireframe=false);
        void DrawBatchList(VkCommandBuffer commandBuffer, std::vector<BatchList> *pBatchList, bool bWireframe=false);
        void OnUpdateWindowSizeDependentResources(VkImageView SSAO);
    private:
        GLTFTexturesAndBuffers *m_pGLTFTexturesAndBuffers;

        ResourceViewHeaps *m_pResourceViewHeaps;
        DynamicBufferRing *m_pDynamicBufferRing;
        StaticBufferPool *m_pStaticBufferPool;

        std::vector<PBRMesh> m_meshes;
        std::vector<PBRMaterial> m_materialsData;

        AeryPbrPass::per_frame m_cbPerFrame;

        PBRMaterial m_defaultMaterial;

        Device   *m_pDevice;
        GBufferRenderPass *m_pRenderPass;
        VkSampler m_samplerPbr = VK_NULL_HANDLE, m_samplerShadow = VK_NULL_HANDLE;

        // PBR Brdf
        Texture m_brdfLutTexture;
        VkImageView m_brdfLutView = VK_NULL_HANDLE;
        VkSampler m_brdfLutSampler = VK_NULL_HANDLE;

        bool                     m_bInvertedDepth;
        ShadowMode m_shadowMode;

        void CreateDescriptorTableForMaterialTextures(PBRMaterial *tfmat, std::map<std::string, VkImageView> &texturesBase, SkyDome *pSkyDome, std::vector<VkImageView>& ShadowMapViewPool, bool bUseSSAOMask);
        void CreateDescriptors(int inverseMatrixBufferSize, DefineList *pAttributeDefines, PBRPrimitives *pPrimitive, bool bUseSSAOMask);
        void CreatePipeline(std::vector<VkVertexInputAttributeDescription> layout, const DefineList &defines, PBRPrimitives *pPrimitive);
    };
}


