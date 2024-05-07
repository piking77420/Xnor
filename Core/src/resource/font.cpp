#include "resource/font.hpp"

#include "utils/logger.hpp"


using namespace XnorCore;


bool_t Font::Load(const Pointer<File>& file)
{
    bool_t returnValue = false;
    
    return returnValue;
}

void Font::CreateInInterface()
{
    
}

void Font::DestroyInInterface()
{
    
}

const Font::Character& Font::GetGlyphByChar(char_t characters) const 
{
    return m_Characters.at(characters);
}


