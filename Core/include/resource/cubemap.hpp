#pragma once

#include <array>
#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"

BEGIN_XNOR_CORE
class Cubemap : public Resource
{
public:
    void Load(const uint8_t* buffer, int64_t length) override;
    
    void Load(const Pointer<File>& file) override;
    
    void CreateInRhi() override;
    
    void DestroyInRhi() override;
    
    void Unload() override;
    
    explicit Cubemap(const std::array<std::string,6>& cubeMapsTextures);

private:
    uint32_t m_Id = 0;
    std::array<void*,6> m_Datas;
    vec2i m_CubeMapSize;
    int32_t m_DataChannels = 0;
};

END_XNOR_CORE
