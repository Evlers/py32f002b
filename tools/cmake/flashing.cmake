
function(add_flashing_target target speed elf_file)
    set(PROBERS_TOOLS ../tools/probe-rs/probe-rs.exe)
    # flashing the firmware to target
    add_custom_target(flash COMMAND ${CMAKE_COMMAND} -E echo "\n"
                        COMMAND ${PROBERS_TOOLS} download --chip ${target} --speed ${speed} ${elf_file}
                        COMMAND ${PROBERS_TOOLS} reset --chip ${target}
                        DEPENDS ${elf_file}
                        COMMENT "Flashing firmware to target"
                        USES_TERMINAL VERBATIM)
endfunction()
