
# add source files
file(GLOB_RECURSE SOURCE_FILES ./hal_driver/src/*.c)

# add static library
add_library(libraries STATIC ${SOURCE_FILES})

# set include path
target_include_directories(libraries PUBLIC
    ./hal_driver/inc
    ./cmsis/include
    ./cmsis/Device/PY32F0xx/Include
)

# set compile definitions
target_compile_definitions(libraries PUBLIC 
    PY32F002Bx5
    USE_HAL_DRIVER
)
