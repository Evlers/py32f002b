
#   @brief Configure Kconfig system and generate build artifacts
#
#   @usage
#   configure_kconfig(
#       kconfig_file       # Input: Path to top-level Kconfig file
#       config_out         # Output: Generated config file (.config)
#       header_out         # Output: C header file with config macros
#       kconfig_list_out   # Output: File listing parsed Kconfig sources
#       configs_in         # Input: Initial configuration file
#   )
#
#   @param kconfig_file     Input Kconfig root file (e.g. Kconfig)
#   @param config_out       Output configuration file path (.config)
#   @param header_out       Output header file path (config.h)
#   @param kconfig_list_out Output Kconfig dependencies list file
#   @param configs_in       Initial configuration input file
#
#   @details
#   This function:
#   1. Generates configuration header if missing
#   2. Imports CONFIG_* variables into CMake cache
#   3. Creates generate_config build target
#   4. Adds menuconfig configuration UI target
#   5. Sets up automatic dependency tracking
#
#   @file_operations
#   - Creates ${header_out} with #define CONFIG_* macros
#   - Generates ${config_out} with key=value pairs
#   - Writes ${kconfig_list_out} with parsed Kconfig files
#   - Stores generation logs in ${CMAKE_BINARY_DIR}/logs/
#
#   @cache_management
#   - Cleans existing CONFIG_* cache variables before import
#   - Stores new CONFIG_* variables as INTERNAL cache entries
#
#   @targets_created
#   1. generate_config: Build target to regenerate configs
#   2. menuconfig: Interactive configuration UI target
#
#   @example
#   configure_kconfig(
#       ${CMAKE_SOURCE_DIR}/Kconfig
#       ${CMAKE_BINARY_DIR}/.config
#       ${CMAKE_BINARY_DIR}/include/config.h
#       ${CMAKE_BINARY_DIR}/kconfig.list
#       ${CMAKE_SOURCE_DIR}/configs/default.config
#   )
#
#   @note
#   - All output files are marked as GENERATED
#   - Requires Python3 and kconfig.py in tools/kconfig/
#   - menuconfig target requires terminal support
#   - Configuration changes trigger CMake reconfigure
#   - Subsequent runs preserve existing .config by default

function(configure_kconfig kconfig_file config_out header_out kconfig_list_out configs_in)
    # parameter check
    foreach(var IN ITEMS kconfig_file config_out header_out kconfig_list_out configs_in)
        if(NOT ${var})
            message(FATAL_ERROR "Missing required parameter: ${var}")
        endif()
    endforeach()

    # python kconfig generate command line
    set(GEN_CMD python3 tools/kconfig/kconfig.py
        ${kconfig_file}
        ${config_out}
        ${header_out}
        ${kconfig_list_out}
        ${configs_in}
    )

    # if the ${header_out} file not exists, generate default config header file
    if(NOT EXISTS "${header_out}")
        string(TIMESTAMP timestamp "%Y%m%d-%H%M%S")
        set(log_file "${CMAKE_BINARY_DIR}/logs/kconfig_gen_${timestamp}.log")
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/logs")

        message(STATUS "Generating default config: ${header_out}")
        execute_process(COMMAND ${GEN_CMD}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_FILE ${log_file}
                        ERROR_FILE ${log_file}
                        RESULT_VARIABLE result)

        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Kconfig generation failed. See log: ${log_file}")
        endif()
    endif()

    # import variables from ${header_out} file
    if(EXISTS "${header_out}")
        message(STATUS "Importing config variables from ${header_out}")

        # clean up old variables
        get_cmake_property(vars CACHE_VARIABLES)
        foreach(var ${vars})
            if(var MATCHES "^CONFIG_")
                unset(${var} CACHE)
            endif()
        endforeach()

        # parse configuration macros
        file(STRINGS "${header_out}" config_lines REGEX "^#define CONFIG_[A-Z0-9_]+")
        foreach(line IN LISTS config_lines)
            string(REGEX MATCH "CONFIG_[A-Z0-9_]+" macro "${line}")
            if(macro)
                string(REGEX REPLACE ".* ([^ ]+)$" "\\1" value "${line}")
                set(${macro} ${value} CACHE INTERNAL "Kconfig variable")
            endif()
        endforeach()
    else()
        message(WARNING "Config header not found: ${header_out}")
    endif()

    # tag build file properties
    set_source_files_properties(
        ${header_out} 
        ${config_out}
        PROPERTIES GENERATED TRUE
    )

    # configuration dependency tracing
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
        ${kconfig_file}
        ${header_out}
        ${config_out}
    )

    # ensure that the ${config_out} file is regenerated when it changes
    add_custom_command( OUTPUT ${header_out}
                        COMMAND ${GEN_CMD}
                        DEPENDS ${kconfig_file} ${configs_in} ${config_out}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        COMMENT "Generating config headers")

    # add auto-build targets (force every build to check dependencies)
    add_custom_target(generate_config ALL DEPENDS ${header_out})

    # add menuconfig target
    add_custom_target(menuconfig
                        COMMAND ${CMAKE_COMMAND} -E env 
                            KCONFIG_CONFIG=${config_out} 
                            menuconfig ${kconfig_file}
                        COMMAND ${CMAKE_COMMAND} -E remove ${header_out} # forced regenerate
                        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target generate_config
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        COMMENT "Running menuconfig and regenerating configs"
                        USES_TERMINAL
                        VERBATIM)
endfunction()
