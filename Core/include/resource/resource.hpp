#pragma once

#include "core.hpp"
#include "file/file.hpp"

BEGIN_XNOR_CORE

class Resource
{
public:
    XNOR_ENGINE Resource() = default;

    XNOR_ENGINE explicit Resource(std::string name);

    XNOR_ENGINE virtual ~Resource() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Resource)

    /// @brief Load resource from memory.
    XNOR_ENGINE virtual void Load(const uint8_t* buffer, int64_t length) = 0;

    /// @brief Load resource from file.
    XNOR_ENGINE virtual void Load(File& file);

    XNOR_ENGINE virtual void Unload() = 0;
    
    [[nodiscard]]
    XNOR_ENGINE bool IsLoaded() const;

    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

protected:
    bool m_Loaded = false;

    std::string m_Name;
};

END_XNOR_CORE
