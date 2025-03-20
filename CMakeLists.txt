cmake_minimum_required(VERSION 3.13)

# include cmake modules
include(tools/cmake/kconfig.cmake)
include(tools/cmake/flashing.cmake)

# set toolchain(must be before the project is called)
set(CMAKE_TOOLCHAIN_FILE ./tools/cmake/toolchain.cmake)

# generate compile commands to enable IDEs to provide code completion, go to definition, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set projects
project(py32f002b)
enable_language(ASM)
enable_language(C)

# add kconfig function and generate default config header file
configure_kconfig(Kconfig sdkconfig ${CMAKE_SOURCE_DIR}/sdkconfig.h ${CMAKE_BINARY_DIR}/kconfig_list sdkconfig)
include_directories(${CMAKE_SOURCE_DIR})

# add probe-rs flash target
add_flashing_target(
    TOOL probe-rs
    TARGET PY32F002Bx5
    SPEED 1000
    ELF_FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
)

# add pyocd flash target
# add_flashing_target(
#     TOOL pyocd
#     TARGET PY32F002Bx5
#     SPEED 1000
#     ELF_FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
#     PACK board/misc/Puya.PY32F0xx_DFP.1.2.0.pack
# )

# add source files
file(GLOB_RECURSE ALL_SOURCE_FILES
    ./app/*.c
    ./board/*.c
    ./board/arch/*.c
    ./board/arch/startup_py32f002b_gcc.s
    ./board/ports/kservice/*.c
)

# set include path
include_directories(
    ./app
    ./board
    ./board/ports/kservice
    ./board/config
)

# set compile and linker flags
set(MCPU "-mcpu=cortex-m0plus -mthumb")
set(MFLOAT_ABI "-mfloat-abi=soft")
set(CMAKE_C_FLAGS "${MCPU} ${MFLOAT_ABI} -std=gnu11 --specs=nano.specs -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs -Wl,--gc-sections,--print-memory-usage,-Map=${CMAKE_PROJECT_NAME}.map")

# add executable
add_executable(${CMAKE_PROJECT_NAME}.elf ${ALL_SOURCE_FILES})
target_compile_options(${CMAKE_PROJECT_NAME}.elf PRIVATE -DNDEBUG -Os -g)
target_link_options(${CMAKE_PROJECT_NAME}.elf PRIVATE -T../board/linker_scripts/py32f002bx5.ld)

# add components and libraries
add_subdirectory(libraries)
add_subdirectory(components)
target_link_libraries(${CMAKE_PROJECT_NAME}.elf PRIVATE
    libraries
    components
)

# generate the binary file
add_custom_target(${CMAKE_PROJECT_NAME}.bin ALL ${CMAKE_OBJCOPY}
                    -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
                    DEPENDS ${CMAKE_PROJECT_NAME}.elf)

# generate the hex file
add_custom_target(${CMAKE_PROJECT_NAME}.hex ALL ${CMAKE_OBJCOPY}
                    -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex
                    DEPENDS ${CMAKE_PROJECT_NAME}.elf)

# echo the size infomation
add_custom_target(size ALL ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf
                    DEPENDS ${CMAKE_PROJECT_NAME}.elf
                    COMMENT "Size of ${CMAKE_PROJECT_NAME}.elf" VERBATIM)
