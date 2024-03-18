#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

namespace DotnetUtils
{
#ifdef SWIG_ONLY
    XNOR_ENGINE const Component& GetComponentListItem(const ::XnorCore::List<Component*>& list, size_t index, int32_t* managedTypeId);
#endif
}

END_XNOR_CORE
