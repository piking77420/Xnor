# Conventions

This document shows our coding conventions and explains why we chose to use them.

## Summary

1. C++ Version
2. Header Files
    1. The #define Guard

TODO

## C++ Version

C++23 because we want to have access to the latest features such as the `[assume](https://en.cppreference.com/w/cpp/language/attributes/assume)` attribute, less `[constexpr](https://en.cppreference.com/w/cpp/language/constexpr)` restrictions, and [more](https://en.cppreference.com/w/cpp/23).

Another advantage of using C++23 is that we also have access to all previous features, such as for C++17:
- `[std::filesystem](https://en.cppreference.com/w/cpp/header/filesystem)`
- `[if constexpr](https://en.cppreference.com/w/cpp/language/if)`
- `[nodiscard](https://en.cppreference.com/w/cpp/language/attributes/nodiscard)`
- [fold-expressions](https://en.cppreference.com/w/cpp/language/fold)
- [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)
- And [more](https://en.cppreference.com/w/cpp/17)

Or for C++20:
- [numbers](https://en.cppreference.com/w/cpp/header/numbers)
- [concepts](https://en.cppreference.com/w/cpp/header/concepts)
- [ranges](https://en.cppreference.com/w/cpp/header/ranges)
- And [more](https://en.cppreference.com/w/cpp/20)

We do not use language extensions such as [boost](https://www.boost.org/).

## Header Files

For header files, we use the `.hpp` file extension. This helps differentiating between C and C++ header files.

In general every `.cpp` file should have an associated `.hpp` file. Some common exceptions are source files for unit tests or files containing a `main` function.

### The #define Guard

We choose not to use define guards, e.g.
```c++
#ifndef HEADER_H
#define HEADER_H

...

#endif // HEADER_H
```
And instead use the `#pragma once` directive once on top of every header file. This decision was made for simplicity as we only need to use a single directive. We are aware that this is slower than using define guards but we find it easier to use.

### Forward Declarations

Avoid using forward declarations where possible, instead include the headers you need. However, in an include-loop situation where forward declarations are the only way of compiling without errors, we allow their usage.

### Inline Functions

Define functions in their respective source file. However, if the function is very small, such as less than 3 lines of code, you are allowed to define it directly inside of the class declaration.

This rule also applies to templated or `constexpr` functions, however if these functions are not small enough they need to be defined in the same translation unit as their declaration, e.g. in the same file. This means that they should be defined under the class declaration.

### Names and Order of Includes

Include headers in the following order: Related header, Standard library headers, External libraries headers, your project's headers. These categories should be split using a blank line between each.

An `#include` directive should never use a path with directory aliases such as `.` or `..`. They should also only use forward slashes.

When specifying the path to include a header file, you should use quotes (`""`) for your project's headers, and angle brackets (`<>`) for external or standard headers.

The `#include` directives should also be sorted by alphabetic order.

For example, for a source file `application.cpp` the includes should be as follow:

```c++
#include "application.hpp"

#include <filesystem>
#include <iostream>

#include <GLFW/glfw.h>
#include <ImGui/imgui.h>

#include "other_header.hpp"
```

## Scoping

### Namespaces

In this project, we use two main namespaces: `XnorCore` and `XnorEditor`. Few exceptions are files such as the one containing the `main` function.

Do not use `using namespace` directives in header files. You are allowed to use them in source files in the global scope but it is discouraged. Using them in function scope is recommended in very specific cases like functions use a lot of `std::filesystem` features or `std::chrono` where it greatly improves readability.

Avoid using nested namespaces as we don't like having to write static accessors (`::`) three times in a row to call a single function.

### Internal Linkage

When definitions in a source file do not need to be referenced outside of this file (e.g. they are not declared in any header file), give them internal likage by declaring them `static`.

### Nonmember, Static Member, and Global Functions

Never declare a function in the global namespace except if it has [internal linkage](#internal-linkage) or if the file is the one containing the `main` function.

You can use singletons (static classes, ones that we can't instantiate), but only if they need private members. Otherwise, use a namespace to group global functions.

### Local Variables

Initialize variables in the closest scope possible.

```c++
int i;
i = f();      // Bad -- initialization separate from declaration.
```

```c++
int i = f();  // Good -- declaration has initialization.
```

```c++
int jobs = NumJobs();
// More code...
f(jobs);      // Bad -- declaration separate from use.
```

```c++
int jobs = NumJobs();
f(jobs);      // Good -- declaration immediately (or closely) followed by use.
```

```c++
std::vector<int> v;
v.push_back(1);  // Prefer initializing using brace initialization.
v.push_back(2);
```

```c++
std::vector<int> v = {1, 2};  // Good -- v starts initialized.
```

Same for struct initialization

```c++
Texture t;
t.width = 64;
t.height = 64;
```

```c++
Texture t
{
    .width = 64,
    .height = 64
};
```

An exception is if the variable is an object, and its constructor is called every time we enter the scope, and its destructor every time we leave it.

```c++
// Inefficient implementation:
for (int i = 0; i < 1000000; i++)
{
    Foo f;  // My ctor and dtor get called 1000000 times each.
    f.DoSomething(i);
}
```
It may be more efficient to declare such a variable used in a loop outside that loop:

```c++
Foo f;  // My ctor and dtor get called once each.
for (int i = 0; i < 1000000; i++)
{
    f.DoSomething(i);
}
```

Do not use `if` initializers, whether it is for a `if`, `for`, `while`, or any other control flow statement.

```c++
while (const char* p = std::strchr(str, '/')) str = p + 1; // Bad -- declaration inside the condition
```

```c++
const char* p = nullptr;
while (p = std::strchr(str, '/')) str = p + 1; // Good -- declaration before the condition check
```

### Static and Global Variables

We allow the usage of global variables <strong>if they are outside of the global namespace</strong> and static member variables but discourage their usage. They should only be used in very rare cases.

## Classes

### Implicit Conversions

Never define implicit conversions. Use the `explicit` keyword for conversion operators and single-argument constructors. The only exception to the latter is for types like Vector2 in which case the constructor is defined without the `explicit` keyword to allow a declaration like this one:

```c++
Vector2 v = 3.f;
```

In this case, the vector's `x` and `y` components are initialized to `3.f`.

### Struct vs. Classes

Use a `struct` only for passive objects that carry data; everything else is a `class`.

The `struct` and `class` keywords behave almost identically in C++ so we add our own semantic meaning to each keyword. The three main rules are:
- All `struct` members should be `public`
- A `struct` should never define member functions
- A `struct` should never have static members

If any of this is false, the type should be a `class`.

### Structs vs. Pairs and Tuples

Never use `std::pair` or `std::tuple`, instead use a struct in which the member variables have meaningful names.

### Inheritance

TODO

### Operator Overloading

Do not use operator overloading, except for math types or user-defined smart pointers.

### Access Control

All class member variables must be `private`, with `public` accessors if needed. The only exception is if a member variable's accessors are default implemented, it can instead be `public` and its accessors removed. For example:

```c++
class Foo
{
public:
    const int& GetI() const { return i; }
    int& GetI() { return i; }
    void SetI(int newI) { i = newI; }

private:
    int i;
};
```

Or

```c++
class Foo
{
public:
    int GetI() const { return i; }
    void SetI(int newI) { i = newI; }

private:
    int i;
};
```

Here, the accessors provided are default implemented. This means that `i` should instead be declared `public` like this:

```c++
class Foo
{
public:
    int i;
};
```

If the accessors are not default implemented, whether they are doing more operations, or they are missing some, the variable must be `private`.

The very first example is encouraged for the getters as it provides the user a reference which allows them to use the variables as an `lvalue` instead of an `rvalue` (see [C++ value categories](https://en.cppreference.com/w/cpp/language/value_category)). This is very practical, as the user can then get the address of the variable for example, or use assignation operators like `=`, `+=` or `%=`.

### Declaration Order

Group similar declarations together, placing `public` earlier.

A class definition should usually start with a `public:` section, followed by `protected:`, then `private:`. Omit sections that would be empty.

Within each section, prefer grouping similar kinds of declarations together, and prefer the following order for `static` members:

1. Types and type aliases (`typedef`, `using`, `enum`, nested structs and classes)
2. Constants
3. Data members
4. All other functions

And for non-`static` members:

1. Data members
2. Constructors
3. Destructor
4. All other functions

This order should then be used for the following sections:

1. `public` `static`
2. `private` `static`
3. `public` non-`static`
4. `private` non-`static`

All functions must be separated by a blank line except for accessors. A blank line separates the accessors for different members.

Here is a sample class as an example:

```c++
class Foo
{
public:
    static inline const staticConstInt = 0;

    static inline int staticInt = 5;

    static void Bar(int i);

private:
    static inline const privateStaticConstInt = 1;

    static inline int privateStaticInt = -7;

    static void PrivateBar(int i);

public:
    int member;

    explicit Foo(int i);

    Foo(int i, int j);

    ~Foo();

    void FooBar(int i);

private:
    int privateMember;

    Foo(int i, int j, int k);

    void FooBar(int i);
};
```

## Functions

### Inputs and outputs

The output of a C++ function is naturally provided via a return value and sometimes via output parameters (or in/out parameters).

Prefer using return values over output parameters: they improve readability, and often provide the same or better performance.

Prefer to return by value or, failing that, return by reference. Avoid returning a pointer unless it can be null.

Parameters are either inputs to the function, outputs from the function, or both. Input parameters should usually be values or `const` references, while output parameters should usually be pointer. Even though pointers may be null, we find passing output parameters much clearer by taking the address of a variables than passing it as a reference.

### Default Arguments

Use default function arguments only if it doesn't interfere with a different overload of this function. For example:

```c++
void Foo();

void Foo(int i = 0); // Bad -- function call who be ambiguous
```

```c++
void Foo(); // Consider that i is 0

void Foo(int i);
```

### Trailing Return Type Syntax

Do not use [trailing return types](https://en.cppreference.com/w/cpp/language/function) except where necessary.

## Other C++ Features

### Friends

Do not use `friend` classes or functions, as it only makes things unclear and split across different files.

### Exceptions

We use C++ exceptions to allow the application to log when an expected error occurs.

### `noexcept`

We use `noexcept` for low-level general-use library functions. A use case is the math library whose functions are declared noexcept where possible. For all other use cases, we do not use the `noexcept` keyword.

### Run-Time Type Information (RTTI)

RTTI allows a programmer to query the C++ class of an object at run-time. This is done by use of `typeid` or `dynamic_cast`.

Most of the time, avoid using RTTI as it is relatively slow, and most calls to `dynamic_cast` can actually be replaced by `reinterpret_cast` which is much faster. However, in the case of inheritance and polymorphism, this may create undefined behavior and a `dynamic_cast` may be necessary.

In a case where a `dynamic_cast` would be necessary, prefer comparing `typeid`s instead, as the [Wikipedia page on RTTI](https://en.wikipedia.org/wiki/Run-time_type_information#C++_%E2%80%93_typeid) says this is much faster.

The use of RTTI is limited to these edge cases and must be very specific.

### Casting

Use C++ casts like `static_cast<float>(doubleValue)` as it is much faster than C-style casts (`(float) doubleValue`).

### Preincrement and Predecrement

Use the postfix form (`i++`) of the increment and decrement operators unless you need prefix semantics.

### Use of `const`

Use const wherever you can, whether it is for function parameters, local variables or global constants.

However, never use const for a class member as it automatically deletes the assignment operator which can then cause a lot of problems.

#### Where to put the `const`

Some people prefer the form `int const*` to `const int*`. This is understandable as it is how the compiler reads it. 

However, we prefer the second form as we find it much clearer to read, like for the example this is a constant integer pointer, and not a integer constant pointer.

### Use of `constexpr`, `constinit` and `consteval`

Use `constexpr` where possible, to allow compile-time initialization, and therefore better run-time performance.

We don't use `constinit` and `consteval` because their use cases are very rare and they can cause compilation errors when not used correctly.

### Integer types

We use typedefs from the `<cstdint>` header for integer types, e.g. `int32_t` for `int` or `uint8_t` for `unsigned char`.

### 64-bit portability

Our code is intended to work on 64-bit only, because it is the most spread architecture and is more optimized.

### Preprocessor Macros

We only define macros for conditional compilation, e.g. code enclosed in `#ifdef` or `#ifndef` preprocessors. Other constants should be defined using [`const`](#use-of-const) or [`constexpr`](#use-of-constexpr-constinit-and-consteval).

If a macro is defined in a header file, and its only use is within this one, use `#undef` after its been used.

### `0`/`\0` and `nullptr`/`NULL`

We use `nullptr` for pointers, we do not use the `NULL` macro.

We use `\0` for `char`s, and `0` for numeric non-pointer types.

### `sizeof`

Prefer `sizeof(varName)` to `sizeof(varType)`.

Use `sizeof(varName)` when you take the size of a particular variable. `sizeof(varName)` will update appropriately if someone changes the variable type either now or later. You may use `sizeof(varType)` for code unrelated to any particular variable, such as code that manages an external or internal data format where a variable of an appropriate C++ type is not convenient.

### Type deduction

We only use type deduction for function scopes, where it makes the code more readable. Its main use case is for `for` loops. Consider the following examples:

```c++
std::vector<uint32_t> integers = { 1, 8, 2, -8, 7, 9, 2 };

for (std::vector<uint32_t>::iterator it = integers.begin(); it != integers.end(); it++)
{
    // Some code
}
```

We could use `decltype` instead:

```c++
std::vector<uint32_t> integers = { 1, 8, 2, -8, 7, 9, 2 };

for (decltype(integers)::iterator it = integers.begin(); it != integers.end(); it++)
{
    // Some code
}
```

Or if we don't explicitly need the iterator, we use a range-for loop:

```c++
std::vector<uint32_t> integers = { 1, 8, 2, -8, 7, 9, 2 };

for (const uint32_t& integer : integers)
{
    // Some code
}
```

In a case like this one, for a range-for loop, type deduction is allowed because the type is very clear. If the body of the loop is quite long, prefer writing the type explicitly instead.

```c++
std::vector<uint32_t> integers = { 1, 8, 2, -8, 7, 9, 2 };

for (const auto& integer : integers)
{
    // Some code
}
```

However, we only allow the use of type deduction for variable types. Never use them for function return type, function parameter types, template types.

We allow the use of templated functions (also called generics) for some specific cases but it should be avoided most of the time.

### Template argument deduction

This is allowed because most of the time it is clear enough for the compiler to be for the developper as well.

### Lambda expressions

Use lambda expressions wherever you need to pass a function as a parameter to another one if it is only used here.

### Concepts and Constraints

Use concepts sparingly. They only make a template function code more specific and allow the developper to understand the eligible types more easily.

### Typedefs and Aliases

We use aliases instead of typedefs because we find it clearer.

### Switch Statements

If not on an enumerated value, switch statements should always have a `default` case. If the `default` case should never execute, use the current MSVC equivalent of `std::unreachable()`: `__assume(false)` which creates undefined behavior where used. This allows the compiler to optimize the instructions by completely removing the `default` case in the machine code.

```c++
switch (value)
{
    case 2:
        break;
        
    case 4:
        break;
        
    case 8:
        break;
        
    case 16:
        break;

    default:
        __assume(false)
}
```

For fallthrough usages:

```c++
switch (value)
{
    case 2: // No annotation needed here because of empty statement
    case 4:
        if (someCondition)
        {
            // Use this instead of or along with annotations in comments.
            [[fallthrough]]
        }
        else
        {
            Func();
            break;
        }
        
    case 8:
        Func2();
        [[fallthrough]]
        
    case 16:
        break;
}
```

## Naming

The most important consistency rules are those about naming. The style of a name immediately informs us what sort of thing the named entity is: a type, a variable, a function, a constant, a macro, etc., without requiring us to search for the declaration of that entity.

We mostly follow the C# naming conventions, except for some special cases.

### General Naming Rules

Use names that describe the purpose or intent of the object. Do not worry about saving horizontal space as it is far more important to make your code immediately understandable by a new reader. Minimize the use of abbreviations that would likely be unknown to someone outside your project (especially acronyms and initialisms). Do not abbreviate by deleting letters within a word. As a rule of thumb, an abbreviation is probably OK if it's listed in Wikipedia. Generally speaking, descriptiveness should be proportional to the name's scope of visibility. For example, n may be a fine name within a 5-line function, but within the scope of a class, it's likely too vague.

### File names

File names should be in `snake_case`. The default file extension for header files is `.h` for C headers and `.hpp` for C++ headers. For source files, this is `.c` for C sources, and `.cpp` for C++ sources.

The name of the source file associated to a header file should match except for the file extension.

### Type names

Type names follow the `PascalCase` rule, except for basic types, in which case it follow the `snake_case` rule, like `Vector2` for which an alias is `vec2`.

### Variable Names

Common variable names: `camelCase`
Custom type public members: `camelCase`
Custom type protected members: `m_PascalCase`
Custom type private members: `m_PascalCase`
Constant names: `PascalCase`
`#define` preprocessors: `UPPER_CASE`
`enum` constants: `UPPER_CASE`

## Comments

## Formatting

## Exceptions to the Rules
