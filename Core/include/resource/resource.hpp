#pragma once

#include "core.hpp"
#include "utils/guid.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class File;

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
    XNOR_ENGINE virtual void Load(const Pointer<File>& file);

    XNOR_ENGINE virtual void CreateInRhi();

    XNOR_ENGINE virtual void DestroyInRhi();

    XNOR_ENGINE virtual void Unload() = 0;
    
    [[nodiscard]]
    XNOR_ENGINE bool_t GetLoaded() const;

    [[nodiscard]]
    XNOR_ENGINE bool_t GetLoadedInRhi() const;

    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

    XNOR_ENGINE void SetName(std::string newName);

    [[nodiscard]]
    XNOR_ENGINE const Guid& GetGuid() const;

protected:
    bool_t m_Loaded = false;
    bool_t m_LoadedInRhi = false;

    std::string m_Name;
    Guid m_Guid;
};

END_XNOR_CORE
