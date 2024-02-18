#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"
#include "rendering/rhi_typedef.hpp"

BEGIN_XNOR_CORE

class Texture : public Resource
{
public:
    
    // Same constructor from base class
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;

    Texture(TextureCreateInfo createInfo);
    
    ~Texture();

    XNOR_ENGINE void Load(File& file) override;
    
    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Unload() override;

    [[nodiscard]]
    XNOR_ENGINE const uint8_t* GetData() const;

    [[nodiscard]]
    XNOR_ENGINE Vector2i GetSize() const;

    [[nodiscard]]
    XNOR_ENGINE int GetChannels() const;

    XNOR_ENGINE virtual void BindTexture(uint32_t index);

private:
    uint8_t* m_Data = nullptr;
    Vector2i m_Size;
    int32_t m_Channels = 0;
    uint32_t m_Id = 0;

    TextureFormat GetFormat(uint32_t textureFormat);
};

END_XNOR_CORE
