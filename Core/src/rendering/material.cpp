#include "rendering/material.hpp"

using namespace XnorCore;

void XnorCore::Material::BindMaterial() const
{
    if (albedoTexture.IsValid())
        albedoTexture->BindTexture(MaterialTextureEnum::Albedo);
		
    if (metallicTexture.IsValid())
        metallicTexture->BindTexture(MaterialTextureEnum::Metallic);

    if (roughnessTexture.IsValid())
        roughnessTexture->BindTexture(MaterialTextureEnum::Roughness);

    if (normalTexture.IsValid())
        normalTexture->BindTexture(MaterialTextureEnum::Normal);

    if (ambientOcclusionTexture.IsValid())
        ambientOcclusionTexture->BindTexture(MaterialTextureEnum::AmbiantOcclusion);
}
