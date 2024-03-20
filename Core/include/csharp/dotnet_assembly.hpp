#pragma once

#include <string>

#include <Coral/Assembly.hpp>

#include "core.hpp"

BEGIN_XNOR_CORE

class DotnetAssembly
{
public:
    XNOR_ENGINE static inline const DotnetAssembly* xnorCoreAssembly = nullptr;
    XNOR_ENGINE static inline const std::string XnorCoreNamespace = "Xnor.Core";

    XNOR_ENGINE explicit DotnetAssembly(std::string filepath);

    XNOR_ENGINE bool_t Load(Coral::AssemblyLoadContext& alc);
    
    XNOR_ENGINE void ProcessTypes();

    [[nodiscard]]
    XNOR_ENGINE const Coral::ManagedAssembly* GetCoralAssembly() const;

    [[nodiscard]]
    XNOR_ENGINE const std::string& GetFilename() const;

private:
    Coral::ManagedAssembly* m_Assembly = nullptr;

    std::string m_Filepath;
    
    void ProcessScriptComponent(Coral::Type& subclass);
};

END_XNOR_CORE
