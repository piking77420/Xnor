#pragma once

#include <string>

#include <Coral/Assembly.hpp>

#include "core.hpp"

BEGIN_XNOR_CORE

class DotnetAssembly
{
public:
    static inline const DotnetAssembly* xnorCoreAssembly = nullptr;
    static inline constexpr std::string XnorCoreNamespace = "Xnor.Core";

    explicit DotnetAssembly(std::string filepath);

    bool_t Load(Coral::AssemblyLoadContext& alc);
    
    void ProcessTypes();

    [[nodiscard]]
    const Coral::ManagedAssembly* GetCoralAssembly() const;

    [[nodiscard]]
    const std::string& GetFilename() const;

private:
    Coral::ManagedAssembly* m_Assembly = nullptr;

    std::string m_Filepath;
    
    void ProcessScriptComponent(Coral::Type& subclass);
};

END_XNOR_CORE
