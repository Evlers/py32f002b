cmake_minimum_required(VERSION 3.13)

# set toolchain(must be before the project is called)
set (CMAKE_TOOLCHAIN_FILE ./tools/cmake/toolchain.cmake)

# generate compile commands to enable IDEs to provide code completion, go to definition, etc.
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set projects
project(py32f002b)
enable_language(ASM)
enable_language(C)

# add source files
file(GLOB_RECURSE ALL_SOURCE_FILES 	
./app/*.c
./board/*.c
./board/arch/*.c
./board/arch/startup_py32f002b_gcc.s
./board/ports/kservice/*.c
./drivers/*.c
./hal/*.c
./utility/*.c
./components/*/*.c
./libraries/hal_driver/src/*.c)

# set include path
include_directories(
./app
./components/log
./components/utils
./components/ringbuffer
./board
./board/ports/kservice
./board/config
./drivers
./hal
./libraries/hal_driver/inc
./libraries/cmsis/include
./libraries/cmsis/Device/PY32F0xx/Include)

# set compile and linker flags
set (MCPU "-mcpu=cortex-m0plus -mthumb")
set (MFLOAT_ABI "-mfloat-abi=soft")
set (CMAKE_C_FLAGS "${MCPU} ${MFLOAT_ABI} -std=gnu11 --specs=nano.specs -ffunction-sections -fdata-sections")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set (CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs -Wl,--gc-sections,--print-memory-usage,-Map=${CMAKE_PROJECT_NAME}.map")

# add executable
add_executable (${CMAKE_PROJECT_NAME}.elf ${ALL_SOURCE_FILES})
target_compile_options (${CMAKE_PROJECT_NAME}.elf PRIVATE -DNDEBUG -Os -g -DUSE_HAL_DRIVER -DPY32F002Bx5)
target_link_options (${CMAKE_PROJECT_NAME}.elf PRIVATE -T../board/linker_scripts/py32f002bx5.ld)

# set output file and tools path
set (EXECUTABLE_OUTPUT_PATH ./bin)
set (LIBRARY_OUTPUT_PATH ./lib)
set (OUTPUT_FILE_PATH_NAME ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME})
set (PROBE-RS_TOOLS ../tools/probe-rs/probe-rs.exe)

# generate the binary file
add_custom_target (${CMAKE_PROJECT_NAME}.bin ALL ${CMAKE_OBJCOPY}
                    -Obinary ${OUTPUT_FILE_PATH_NAME}.elf ${OUTPUT_FILE_PATH_NAME}.bin
                    DEPENDS ${OUTPUT_FILE_PATH_NAME}.elf)

# generate the hex file
add_custom_target (${CMAKE_PROJECT_NAME}.hex ALL ${CMAKE_OBJCOPY}
                    -Oihex ${OUTPUT_FILE_PATH_NAME}.elf ${OUTPUT_FILE_PATH_NAME}.hex
                    DEPENDS ${OUTPUT_FILE_PATH_NAME}.elf)

# echo the size infomation
add_custom_target (size ALL ${CMAKE_SIZE} ${OUTPUT_FILE_PATH_NAME}.elf
                    DEPENDS ${OUTPUT_FILE_PATH_NAME}.elf
                    COMMENT "size of ${CMAKE_PROJECT_NAME}.elf" VERBATIM)

# flashing the firmware to target
add_custom_target(flash COMMAND ${CMAKE_COMMAND} -E echo "\n"
                        COMMAND ${PROBE-RS_TOOLS} download --chip PY32F002Bx5 --speed 500 ${OUTPUT_FILE_PATH_NAME}.elf
                        COMMAND ${PROBE-RS_TOOLS} reset --chip PY32F002Bx5
                        DEPENDS ${OUTPUT_FILE_PATH_NAME}.elf
                        COMMENT "flashing firmware to target..."
                        USES_TERMINAL VERBATIM)
