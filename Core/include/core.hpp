// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#include <cmath>
#include <cstdint>

// ReSharper disable once CppEnforceTypeAliasCodeStyle
/// @brief Equivalent to <c>char</c>.
/// 
/// @paragraph reason Reason
/// We use a typedef here instead of a type alias for consistency
/// with how the other integral types are defined in the <c>cstdint</c> header.
/// Also, we need to manually add this type because it is considered
/// different to <c>int8_t</c> as the latter is the equivalent of <c>signed char</c>
/// and not <c>char</c>.
typedef char char_t;

// ReSharper disable once CppEnforceTypeAliasCodeStyle
/// @brief Equivalent to <c>bool</c>.
///
/// @see @ref char_t for reason
typedef bool bool_t;

#ifdef XNOR_EXPORT
/// @brief Macro that declares a DLL export
#define XNOR_ENGINE __declspec(dllexport)
#else
/// @brief Macro that declares a DLL import
#define XNOR_ENGINE __declspec(dllimport)
#endif

/// @brief Begins the namespace XnorCore
#define BEGIN_XNOR_CORE namespace XnorCore {

/// @brief Ends the namespace XnorCore
#define END_XNOR_CORE }

/// @brief Automatically generates the copy constructors and move operators for a specified type
/// @param type Type name
#define DEFAULT_COPY_MOVE_OPERATIONS(type)                      \
    XNOR_ENGINE type(const type& other) = default;              \
    XNOR_ENGINE type(type&& other) = default;                   \
    XNOR_ENGINE type& operator=(const type& other) = default;   \
    XNOR_ENGINE type& operator=(type&& other) = default;

/// @brief Automatically generates the copy constructors and move operators for a specified type (without the @c XNOR_ENGINE macro)
/// @param type Type name
#define DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(type)    \
    type(const type& other) = default;                  \
    type(type&& other) = default;                       \
    type& operator=(const type& other) = default;       \
    type& operator=(type&& other) = default;

/// @brief Automatically deletes the copy constructors and move operators for a specified type
/// @param type Type name
#define DELETE_COPY_MOVE_OPERATIONS(type)          \
    type(const type& other) = delete;              \
    type(type&& other) = delete;                   \
    type& operator=(const type& other) = delete;   \
    type& operator=(type&& other) = delete;

// Macro used to declare a static class, e.g. a class that cannot be instantiated
// This should be the first statement inside of the class definition
#define STATIC_CLASS(type)              \
    public:                             \
    type() = delete;                    \
    ~type() = delete;                   \
    DELETE_COPY_MOVE_OPERATIONS(type)   \
    private:
