# Kitsune - C kit, with useful data structures, memory management objects, math procedures and more

## Overview

Kitsune is an STB-style library and provides different modules:

1. core.h: logging, utility macros of all types, bit manipulation and SSO Strings
2. io.h: simple IO functions
3. math.h: algebra, analysis
4. mem.h: memory management objects like arenas, pools and smart pointers
5. ds.h: data structures and algorithms
6. render.h: raylib-like graphic engine

## Usage

### Run tests/examples

```c
make help
```

### Link in another project

1. Create a standard CMakeLists.txt;
2. Be sure to either clone the repo manually or use FetchContent;
3. Add a target;
4. Link the library:

```c
target_link_libraries(target PRIVATE ks::lib)
```
