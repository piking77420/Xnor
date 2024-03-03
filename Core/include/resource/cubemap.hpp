#pragma once

#include <array>
#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"

BEGIN_XNOR_CORE
class Cubemap : public Resource
{
public:
    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Load(const Pointer<File>& file) override;
    
    XNOR_ENGINE void CreateInRhi() override;
    
    XNOR_ENGINE void DestroyInRhi() override;
    
    XNOR_ENGINE void Unload() override;
    
    XNOR_ENGINE explicit Cubemap(const std::array<std::string, 6>& cubeMapsTextures);

    XNOR_ENGINE void BindTexture(uint32_t unit) const;

    XNOR_ENGINE void UnBindTexture(uint32_t unit) const;
    
private:
    uint32_t m_Id = 0;
    std::array<void*, 6> m_Datas;
    Vector2i m_CubeMapSize;
    int32_t m_DataChannels = 0;
};

END_XNOR_CORE
