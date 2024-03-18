#include "rendering/render_systems/bloom_pass.hpp"

#include "resource/resource_manager.hpp"

using namespace XnorCore;

void BloomPass::Init(uint32_t mipChainLength)
{
    //m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
    //mMipChain.resize(mipChainLength);

    
    
  
}

void BloomPass::Destroy()
{
}


void BloomPass::ComputeBloom(const Texture& textureWithoutBloom)
{
}

void BloomPass::ComputeMiMap(Vector2i currentViewPortSize)
{
    /*
    m_BloomPassSize = sizeof
    
    for (unsigned int i = 0; i < mipChainLength; i++)
    {
        mipSize *= 0.5f;
        mipIntSize /= 2;
        mip.size = mipSize;
        mip.intSize = mipIntSize;
        
        TextureCreateInfo createInfo =
            {
            .data = nullptr,
            .size = ,
            .filtering = TextureFiltering::Linear,
            .wrapping = TextureWrapping::ClampToEdge,
            .format = TextureFormat::Rgb,
            .internalFormat = TextureInternalFormat::R11FG11FB10F,
            .dataType = DataType::Float
        };

        
        mMipChain[i] = new Texture();



        glGenTextures(1, &mip.texture);
        glBindTexture(GL_TEXTURE_2D, mip.texture);
        // we are downscaling an HDR color buffer, so we need a float texture format
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                     (int)mipSize.x, (int)mipSize.y,
                     0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }*/
}
