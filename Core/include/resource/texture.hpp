#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"

BEGIN_XNOR_CORE

class Texture : public Resource
{
public:
    int32_t desiredChannels = 4;

    void Load(const uint8_t* buffer, int64_t length) override;

    void Load(const File& file) override;

    void Unload() override;

    [[nodiscard]]
    const uint8_t* GetData() const;

    [[nodiscard]]
    Vector2i GetSize() const;

    [[nodiscard]]
    int GetChannels() const;

    virtual void BindTexture(uint32_t index);

private:
    uint8_t* m_Data = nullptr;
    Vector2i m_Size;
    int32_t m_Channels = 0;
    
    uint32_t m_Id = 0;
};

END_XNOR_CORE
