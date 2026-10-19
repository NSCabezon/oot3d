# Included by CMake after its per-language defaults (CMAKE_USER_MAKE_RULES_OVERRIDE).
# data/oot3d.ld matches objects as "*<name>.o", so force ".o" on the Generic (wibo) cross build,
# even when compiler identification failed (e.g. no FlexLM license available yet).
set(CMAKE_C_OUTPUT_EXTENSION ".o")
set(CMAKE_CXX_OUTPUT_EXTENSION ".o")
set(CMAKE_ASM_OUTPUT_EXTENSION ".o")

# Mirror CMake's Modules/Compiler/ARMCC.cmake rules so the generated build is identical whether or not
# compiler identification succeeded (identification needs a working FlexLM license).
foreach(lang C CXX)
    set(CMAKE_${lang}_OUTPUT_EXTENSION_REPLACE 1)
    set(CMAKE_${lang}_RESPONSE_FILE_LINK_FLAG "--via=")
    set(CMAKE_${lang}_LINK_MODE LINKER)
    set(CMAKE_${lang}_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> <LINK_LIBRARIES> <OBJECTS> -o <TARGET> --list <TARGET_BASE>.map")
    set(CMAKE_${lang}_CREATE_STATIC_LIBRARY "<CMAKE_AR> --create -cr <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_DEPFILE_FLAGS_${lang} "--depend=<DEP_FILE> --depend_single_line --no_depend_system_headers")
endforeach()
