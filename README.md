# Kitsune - C kit, with useful data structures, memory management objects, math procedures and more

## Overview

Kitsune is an STB-style library and provides different modules:

1. core.h: logging, utility macros of all types, bit manipulation and SSO Strings;
2. io.h: simple IO functions;
3. math.h: algebra, analysis;
4. mem.h: memory management objects like arenas, pools and smart pointers;
5. ds.h: data structures and algorithms;
6. time.h: os time utilities;

## Usage

### Run tests/examples

```c
make help
```

### Link in another project (cmake)

1. Create a standard CMakeLists.txt;
2. Be sure to either clone the repo manually or use FetchContent;
3. Add the subdirectory;
4. Add a target;
5. Link the library:

```c
target_link_libraries(target PRIVATE kitsune::lib)
```

## Credits & Licenses

Kitsune uses the following libraries:

* [OpenMP](https://www.openmp.org/)

Full third-party license text can be found in [THIRD-PARTY-NOTICES.md](./THIRD-PARTY-NOTICES.md).

## Legal notes

Copyright (c) 2026 Luca Marchisio. See [LICENSE](./LICENSE) for details
