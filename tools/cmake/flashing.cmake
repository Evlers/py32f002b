
#   @brief Create unified flashing target (supports probe-rs/pyocd)
#
#   @usage
#   add_flashing_target(
#       TOOL <probe-rs|pyocd>    # Required: Flashing tool selection
#       TARGET <device>          # Required: Target device name
#       SPEED <khz>              # Required: Communication speed in KHz
#       ELF_FILE <path>          # Required: ELF file to flash
#       [PACK <package>]         # Optional: Required for PYOCD
#   )
#
#   @param TOOL      Flashing tool type (lowercase):
#                   - probe-rs: Use probe-rs toolchain
#                   - pyocd   : Use pyocd toolchain
#   @param TARGET   Target device name (tool-specific format):
#                   - probe-rs: e.g. stm32h750vb
#                   - pyocd   : e.g. STM32H750VB
#   @param SPEED     Communication speed (KHz units for both tools):
#                   - probe-rs: Directly used as KHz (4000 = 4MHz)
#                   - pyocd   : Auto-converted to Hz (4000 → 4,000,000Hz)
#   @param ELF_FILE  Path to ELF file (prefer absolute path)
#   @param PACK      Device pack name (PYOCD only):
#                   - e.g. stm32h7 for STM32H7 series
#
#   @example probe-rs usage
#   add_flashing_target(
#       TOOL probe-rs
#       TARGET stm32h750vb
#       SPEED 4000
#       ELF_FILE ${PROJECT_BINARY_DIR}/firmware.elf
#   )
#
#   @example pyocd usage
#   add_flashing_target(
#       TOOL pyocd
#       TARGET STM32H750VB
#       SPEED 4000
#       ELF_FILE ${PROJECT_BINARY_DIR}/firmware.elf
#       PACK stm32h7
#   )
#
#   @note
#   - Speed parameter uses KHz universally, auto-converted for pyocd
#   - Default probe-rs path: tools/probe-rs/probe-rs.exe
#   - Repeated calls overwrite 'flash' target - use once per project
#   - Parameter names must be uppercase (TOOL/TARGET/etc)
#   - Automatic parameter validation with immediate errors

function(add_flashing_target)
    # define parameter parsing rules
    set(options)
    set(oneValueArgs TOOL TARGET SPEED ELF_FILE PACK)  # all parameters are passed by name.
    set(multiValueArgs)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "" "${oneValueArgs}" "${multiValueArgs}")

    # check the public necessary parameters
    foreach(req IN ITEMS TOOL TARGET SPEED ELF_FILE)
        if(NOT ARG_${req})
            message(FATAL_ERROR "Missing required parameter: ${req}")
        endif()
    endforeach()

    # check for unrecognized parameters
    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unrecognized arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    # flashing the firmware to target
    if(ARG_TOOL STREQUAL "probe-rs")
        if(ARG_PACK)
            message(WARNING "probe-rs ignores PACK parameter")
        endif()

        set(TOOL_PATH tools/probe-rs/probe-rs.exe)
        add_custom_target(flash
            COMMAND ${CMAKE_COMMAND} -E echo "\n"
            COMMAND ${TOOL_PATH} download --chip ${ARG_TARGET} --speed ${ARG_SPEED} ${ARG_ELF_FILE}
            COMMAND ${TOOL_PATH} reset --chip ${ARG_TARGET}
            DEPENDS ${ARG_ELF_FILE}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Flashing firmware to target"
            USES_TERMINAL
            VERBATIM
        )

    elseif(ARG_TOOL STREQUAL "pyocd")
        if(NOT ARG_PACK)
            message(FATAL_ERROR "PYOCD requires PACK parameter")
        endif()

        math(EXPR PYOCD_SPEED "${ARG_SPEED} * 1000")  # KHz -> Hz

        add_custom_target(flash
            COMMAND ${CMAKE_COMMAND} -E echo "\n"
            COMMAND pyocd flash --pack=${ARG_PACK} -t ${ARG_TARGET} -f ${PYOCD_SPEED} ${ARG_ELF_FILE}
            DEPENDS ${ARG_ELF_FILE}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Flashing firmware to target"
            USES_TERMINAL
            VERBATIM
        )

    else()
        message(FATAL_ERROR "Invalid TOOL: ${ARG_TOOL}. Valid options: probe-rs, pyocd")
    endif()
endfunction()
