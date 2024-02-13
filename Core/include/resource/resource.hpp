#pragma once

#include "core.hpp"
#include "file/file.hpp"

BEGIN_XNOR_CORE

class Resource
{
public:
    Resource() = default;

    explicit Resource(const std::string& name);

    virtual ~Resource() = default;

    Resource(const Resource& other) = default;

    Resource(Resource&& other) = default;

    Resource& operator=(const Resource& other) = default;

    Resource& operator=(Resource&& other) = default;

    /// @brief Load resource from memory.
    virtual void Load(const uint8_t* buffer, int64_t length) = 0;

    /// @brief Load resource from file.
    virtual void Load(const File& file) = 0;

    virtual void Unload() = 0;
    
    [[nodiscard]]
    bool IsLoaded() const;

    [[nodiscard]]
    std::string GetName() const;

protected:
    bool m_Loaded = false;

    std::string m_Name;
};

END_XNOR_CORE
