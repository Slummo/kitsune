# Kitsune - C kit, with useful data structures, memory management objects, math procedures and more

## Overview

Kitsune is an STB-style library and provides different modules:

1. core.h: logging, utility macros of all types, bit manipulation, results and optionals;
2. string.h: SSO strings;
3. io.h: simple IO functions;
4. math.h: algebra, analysis;
5. mem.h: memory management objects like allocators, arenas, pools and smart pointers;
6. ds.h: data structures and algorithms;
7. time.h: os time utilities;

## Usage

### Run tests/examples

```c
make help
```

### Compile

Make sure to define implementation macros and include headers:

```c
#define KS_CORE_IMPL
#define KS_MEM_IMPL
#define KS_STRING_IMPL

#include <ks/string.h>
...
```

Note: In the example above both core and mem implementation are needed, because string requires them;
not defining the necessary implementations throws a compilation error, so it shouldn't be a big problem.
For lazy users, just use the all-inclusive 'KITSUNE_IMPL' macro.

### Link (CMake)

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

- [OpenMP](https://www.openmp.org/)

Full third-party license text can be found in [THIRD-PARTY-NOTICES.md](./THIRD-PARTY-NOTICES.md).

## Legal notes

Copyright (c) 2026 Luca Marchisio. See [LICENSE](./LICENSE) for details
