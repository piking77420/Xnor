// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#include <cstdint>

#ifdef XNOR_EXPORT
#define XNOR_ENGINE __declspec(dllexport)
#else
#define XNOR_ENGINE __declspec(dllimport)
#endif

#define BEGIN_XNOR_CORE namespace XnorCore {

#define END_XNOR_CORE }

#define DEFAULT_COPY_MOVE_OPERATIONS(type)                      \
    XNOR_ENGINE type(const type& other) = default;              \
    XNOR_ENGINE type(type&& other) = default;                   \
    XNOR_ENGINE type& operator=(const type& other) = default;   \
    XNOR_ENGINE type& operator=(type&& other) = default;

#define DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(type)    \
    type(const type& other) = default;                  \
    type(type&& other) = default;                       \
    type& operator=(const type& other) = default;       \
    type& operator=(type&& other) = default;

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
