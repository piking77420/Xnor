#include "csharp/dotnet_utils.hpp"

#include "csharp/dotnet_assembly.hpp"
#include "reflection/factory.hpp"

using namespace XnorCore;

namespace XnorCore::DotnetUtils { XNOR_ENGINE const Component& GetComponentListItem(const ::XnorCore::List<Component*>& list, size_t index, int32_t* managedTypeId); }

const Component& DotnetUtils::GetComponentListItem(const ::XnorCore::List<Component*>& list, const size_t index, int32_t* managedTypeId)
{
    const Component& result = reinterpret_cast<const Component&>(list[index]);

    if (managedTypeId == nullptr)
        return result;
    
    // Get type name from raw pointer using C++ reflection
    const char_t* typeName = Factory::GetName(Utils::GetTypeHash<Component>(&result));

    // Convert these names to their C# type equivalent
    auto&& dotnetAssembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();

    *managedTypeId = dotnetAssembly->GetType(DotnetAssembly::XnorCoreNamespace + '.' + typeName).GetTypeId();
    
    return result;
}
