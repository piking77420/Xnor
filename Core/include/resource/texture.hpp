#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"

BEGIN_XNOR_CORE

class Texture : public Resource
{
public:
    int32_t desiredChannels = 4;

    XNOR_ENGINE Texture() = default;

    XNOR_ENGINE explicit Texture(const std::string& name);

    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;

    XNOR_ENGINE void Load(File& file) override;

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
};

END_XNOR_CORE
