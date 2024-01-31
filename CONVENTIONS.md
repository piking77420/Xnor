# Conventions

This document shows our coding conventions and explains why we chose to use them.

## C++ Version

C++23 because we want to have access to the latest features such as the <code>[assume](https://en.cppreference.com/w/cpp/language/attributes/assume)</code> attribute, less <code>[constexpr](https://en.cppreference.com/w/cpp/language/constexpr)</code> restrictions, and [more](https://en.cppreference.com/w/cpp/23).

Another advantage of using C++23 is that we also have access to all previous features, such as for C++17:
- <code>[std::filesystem](https://en.cppreference.com/w/cpp/header/filesystem)</code>
- <code>[if constexpr](https://en.cppreference.com/w/cpp/language/if)</code>
- <code>[nodiscard](https://en.cppreference.com/w/cpp/language/attributes/nodiscard)</code>
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

For header files, we use the <code>.hpp</code> file extension. This helps differentiating between C and C++ header files.

In general every <code>.cpp</code> file should have an associated <code>.hpp</code> file. Some common exceptions are source files for unit tests or files containing a <code>main</code> function.

### The #define guard

We choose not to use define guards, e.g.
```c++
#ifndef HEADER_H
#define HEADER_H

...

#endif // HEADER_H
```
And instead use the <code>#pragma once</code> directive once on top of every header file. This decision was made for simplicity as we only need to use a single directive. We are aware that this is slower than using define guards but we find it easier to use.

### Forward Declarations

Avoid using forward declarations where possible, instead include the headers you need. However, in an include-loop situation where forward declarations are the only way of compiling without errors, we allow their usage.

### Inline Functions

Define functions in their respective source file. However, if the function is very small, such as less than 3 lines of code, you are allowed to define it directly inside of the class declaration.

This rule also applies to templated or <code>constexpr</code> functions, however if these functions are not small enough they need to be defined in the same translation unit as their declaration, e.g. in the same file. This means that they should be defined under the class declaration.

### Names and Order of Includes

Include headers in the following order: Related header, Standard library headers, External libraries headers, your project's headers. These categories should be split using a blank line between each.

An include directive should never use a path with directory aliases such as <code>.</code> or <code>..</code>. They should also only use forward slashes.

When specifying the path to include a header file, you should use quotes (<code>""</code>) for your project's headers, and angle brackets (<code><></code>) for external or standard headers.

For example, for a source file <code>application.cpp</code> the includes should be as follow:
```c++
#include "application.hpp"

#include <filesystem>
#include <iostream>

#include <ImGui/imgui.h>
#include <GLFW/glfw.h>

#include "other_header.hpp"
```

## Scoping



## Classes

## Functions

## Other C++ Features

## Inclusive Language

## Naming

## Comments

## Formatting

## Exceptions to the Rules
