
set(CMAKE_SYSTEM_PROCESSOR "arm" CACHE STRING "")
set(CMAKE_SYSTEM_NAME "Generic" CACHE STRING "")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TARGET_TOOLCHAIN_PREFIX "arm-none-eabi-")
set(TARGET_TOOLCHAIN_PREFIX "arm-none-eabi-")
set(CMAKE_C_COMPILER "${TARGET_TOOLCHAIN_PREFIX}gcc")
set(CMAKE_ASM_COMPILER "${TARGET_TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TARGET_TOOLCHAIN_PREFIX}g++")
set(CMAKE_AR "${TARGET_TOOLCHAIN_PREFIX}ar")
set(CMAKE_LINKER "{TARGET_TOOLCHAIN_PREFIX}ld")
set(CMAKE_OBJCOPY "${TARGET_TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_RANLIB "${TARGET_TOOLCHAIN_PREFIX}ranlib")
set(CMAKE_SIZE "${TARGET_TOOLCHAIN_PREFIX}size")
set(CMAKE_STRIP "${TARGET_TOOLCHAIN_PREFIX}ld")
