#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"

BEGIN_XNOR_CORE

class Texture : public Resource
{
public:
    explicit Texture(const std::string& filepath);

    void Load() override;

    void Load(int desiredChannels);
    
    void Unload() override;

    [[nodiscard]]
    const unsigned char* GetData() const;

    [[nodiscard]]
    vec2i GetSize() const;

    [[nodiscard]]
    int GetChannels() const;

    virtual void BindTexture(uint32_t index);

private:
    unsigned char* m_Data = nullptr;
    vec2i m_Size;
    int m_Channels = 0;
    
    uint32_t m_Id;
};

END_XNOR_CORE
