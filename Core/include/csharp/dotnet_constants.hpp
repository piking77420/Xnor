﻿#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

namespace Dotnet
{
    constexpr const char_t* const GameProjectName = "Game";
    
    constexpr const char_t* const AssembliesDirectory = "DotnetAssemblies";
    
    constexpr const char_t* const CoreProjectName = "CoreManaged";
    
    constexpr const char_t* const CoreProjectLocation = CoreProjectName;
    
    constexpr const char_t* const GameProjectLocation = GameProjectName;
    
    constexpr const char_t* const GameProjectBuildOptions = "--no-self-contained";
}

END_XNOR_CORE
