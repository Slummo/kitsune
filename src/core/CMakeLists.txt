# Core
set(CORE_INC "${CMAKE_SOURCE_DIR}/include/kitsune/core")
file(GLOB CORE_HEADERS CONFIGURE_DEPENDS "${CORE_INC}/*.h")
kitsune_create_module(
    NAME Core
    INTERFACE
    HEADERS ${CORE_HEADERS}
)