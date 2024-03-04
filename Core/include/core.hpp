// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#include <cmath>
#include <cstdint>

/// @file core.hpp
/// @brief This file is meant to be included by every single other header file of this project.
///
/// It includes the standard headers @c <cmath> and @c <cstdint> so that types such as
/// @c int8_t or @c float_t are defined. It also defines typedefs for @ref char_t and
/// @ref bool_t, as they aren't defined by those standard headers.
///
/// Apart from typedefs, this file declares macros ranging from necessary (@ref XNOR_ENGINE)
/// to useful and of general-use (@ref DEFAULT_COPY_MOVE_OPERATIONS).

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
/// @see @ref char_t for reason.
typedef bool bool_t;

/// @brief Macro used for DLL export/import.
///
/// This macro should be used at the beginning of static member variable and non-inline function declarations.
#ifdef XNOR_EXPORT
#define XNOR_ENGINE __declspec(dllexport)
#else
#define XNOR_ENGINE __declspec(dllimport)
#endif

/// @brief Starts the @ref XnorCore namespace.
#define BEGIN_XNOR_CORE namespace XnorCore {

/// @brief Ends the @ref XnorCore namespace.
#define END_XNOR_CORE }

/// @brief Creates default copy and move operations for a given @p type.
///
/// @paragraph usage Usage
/// This macro should be used for any type that defines at least one of:
/// - A non-default destructor (this includes default implementations),
/// - A non-default copy constructor
/// - A non-default move constructor
/// - A non-default copy operator
/// - A non-default move operator
/// 
/// It should be used right after the last constructor declaration:
/// @code
/// class A
/// {
/// public:
///     A();
///     
///     A(int);
///
///     DEFAULT_COPY_MOVE_OPERATIONS(A)
/// };
/// @endcode
/// Or after the destructor declaration if there is one:
/// @code
/// class A
/// {
/// public:
///     A();
///     
///     A(int);
///
///     ~A();
///
///     DEFAULT_COPY_MOVE_OPERATIONS(A)
/// };
/// @endcode
/// 
/// @param type The type to default the copy and move operations of.
#define DEFAULT_COPY_MOVE_OPERATIONS(type)         \
    type(const type& other) = default;             \
    type(type&& other) = default;                  \
    type& operator=(const type& other) = default;  \
    type& operator=(type&& other) = default;

/// @brief Deletes the copy and move operations of a given @p type.
/// 
/// @param type The type to delete the copy and move operations of.
///
/// @see @ref DEFAULT_COPY_MOVE_OPERATIONS for usage.
#define DELETE_COPY_MOVE_OPERATIONS(type)          \
    type(const type& other) = delete;              \
    type(type&& other) = delete;                   \
    type& operator=(const type& other) = delete;   \
    type& operator=(type&& other) = delete;

/// @brief Macro used to declare a static class, e.g. a class that cannot be instantiated.
/// 
/// For consistency, this should be the first statement inside of the class definition:
/// @code
/// class A
/// {
///     STATIC_CLASS(A)
///     
/// public:
///     ...
/// };
/// @endcode
/// 
/// @param type The type to make static.
#define STATIC_CLASS(type)              \
    public:                             \
    type() = delete;                    \
    ~type() = delete;                   \
    DELETE_COPY_MOVE_OPERATIONS(type)   \
    private:
