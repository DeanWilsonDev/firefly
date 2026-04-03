# Firefly - Logging Library

A lightweight C++ logging library for game development and tooling.

## Features

✅ Easy to use\
✅ Supports formatting\
✅ Modular and reusable

## Installation

### Using CMake:

```bash
git submodule add git@github.com:DeanWilsonDev/firefly.git external/firefly
git submodule update --init --recursive
```

Also add the following into your projects CMakeLists.txt file

```cmake
add_subdirectory(external/firefly)
target_link_libraries(your-project PRIVATE firefly)
```
