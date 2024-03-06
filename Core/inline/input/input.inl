#pragma once

BEGIN_XNOR_CORE

template <class T>
T Input::GetMousePosition()
{
    static_assert(Meta::IsAny<T, Vector2, Vector2i>, "GetMousePosition<T> only works with T as a Vector2");
    
    return static_cast<T>(m_MousePos);
}

END_XNOR_CORE
