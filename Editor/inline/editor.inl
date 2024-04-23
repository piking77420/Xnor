#pragma once

#include "utils/meta_programming.hpp"

template <typename T, typename... Args>
void Editor::OpenWindow(Args&&... args)
{
    static_assert(XnorCore::Meta::IsBaseOf<UiWindow, T>, "Provided type wasn't a ui window");
    
    m_UiWindows.push_back(new T(this, std::forward<Args>(args)...));
}

