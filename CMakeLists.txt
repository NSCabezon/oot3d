cmake_minimum_required(VERSION 3.24)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/ARMCC.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED ENV{ARMCC_PATH})
    message(FATAL_ERROR "please set ARMCC_PATH")
endif()
if(NOT DEFINED ENV{RVCT40INC})
    message(FATAL_ERROR "please set RVCT40INC. Probably to ARMCC_PATH/include/windows")
endif()
if(NOT DEFINED ENV{RVCT40LIB})
    message(FATAL_ERROR "please set RVCT40LIB. Probably to ARMCC_PATH/include/lib")
endif()

find_program(PYTHON_EXE NAMES python3 python REQUIRED)

if(DEFINED ENV{WIBO})
    # Non-Windows host (e.g. macOS + wibo-macos under Rosetta 2, or Linux + wibo):
    # generate tiny shell wrappers that run the Windows RVCT tools through wibo.
    set(WIBO "$ENV{WIBO}")
    # Cross build: stop CMake from injecting host (Darwin) flags such as "-arch arm64" / "-isysroot".
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_OSX_ARCHITECTURES "" CACHE STRING "" FORCE)
    set(CMAKE_OSX_SYSROOT "" CACHE STRING "" FORCE)
    set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_SOURCE_DIR}/tools/macos/rules_override.cmake)
    set(ARMWRAP_DIR "${CMAKE_BINARY_DIR}/armwrap")
    foreach(armtool armasm armar armcc armlink fromelf)
        set(TOOL "$ENV{ARMCC_PATH}/win_32-pentium/${armtool}.exe")
        if(NOT EXISTS "${TOOL}")
            message(FATAL_ERROR "missing ${TOOL}")
        endif()
        configure_file(${CMAKE_SOURCE_DIR}/tools/macos/armwrap.sh.in ${ARMWRAP_DIR}/${armtool} @ONLY)
        file(CHMOD ${ARMWRAP_DIR}/${armtool} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
    endforeach()
    set(ARM_TOOL_DIR "${ARMWRAP_DIR}")
    set(ARM_EXE_SUFFIX "")
else()
    set(ARM_TOOL_DIR "$ENV{ARMCC_PATH}/win_32-pentium")
    set(ARM_EXE_SUFFIX ".exe")
endif()

find_program(ARM_ASM NAMES armasm${ARM_EXE_SUFFIX} PATHS "${ARM_TOOL_DIR}" REQUIRED NO_DEFAULT_PATH)
find_program(ARM_AR NAMES armar${ARM_EXE_SUFFIX} PATHS "${ARM_TOOL_DIR}" REQUIRED NO_DEFAULT_PATH)
find_program(ARM_CC NAMES armcc${ARM_EXE_SUFFIX} PATHS "${ARM_TOOL_DIR}" REQUIRED NO_DEFAULT_PATH)
find_program(ARM_LINK NAMES armlink${ARM_EXE_SUFFIX} PATHS "${ARM_TOOL_DIR}" REQUIRED NO_DEFAULT_PATH)
find_program(ARM_FROMELF NAMES fromelf${ARM_EXE_SUFFIX} PATHS "${ARM_TOOL_DIR}" REQUIRED NO_DEFAULT_PATH)

set(CMAKE_CXX_COMPILER_ID ARMCC)

set(CMAKE_AR ${ARM_AR})
set(CMAKE_C_COMPILER ${ARM_CC})
set(CMAKE_CXX_COMPILER ${ARM_CC})
set(CMAKE_ASM_COMPILER ${ARM_ASM})
set(CMAKE_LINKER ${ARM_LINK})

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_ASM_COMPILER_WORKS TRUE)

if (NOT (CMAKE_SYSTEM_NAME STREQUAL "Windows"))
    # Not on windows but calling windows programs, probably with wine.
    # Windows has a lower command line length limit than Linux so passing a full length link command won't work.
    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS TRUE)

    if (CMAKE_GENERATOR STREQUAL "Ninja")
        # The above doesn't work on ninja so do this instead.
        set(CMAKE_NINJA_FORCE_RESPONSE_FILE ON)

        # Can only turn on response files for all commands so need to set how to handle other command types
        set(CMAKE_C_RESPONSE_FILE_FLAG "--via=")
        set(CMAKE_CXX_RESPONSE_FILE_FLAG "--via=")
        set(CMAKE_ASM_RESPONSE_FILE_FLAG "--via=")
    endif()
endif()

project(oot3d C CXX ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".axf")

add_subdirectory(library)
add_subdirectory(disassembly)

file(GLOB_RECURSE csourcefiles CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE cxxsourcefiles CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

SET(CXX_FLAGS "--apcs=//interwork --cpu=MPCore --fpmode=fast --vfe --no_rtti --no_rtti_data --cpp --arm --force_new_nothrow --signed_chars --multibyte-chars --locale=japanese --data-reorder --split_sections --forceinline -O3 -Otime")
if(WIBO)
    # wibo can't setlocale("japanese"); sources are pure ASCII so this doesn't affect codegen.
    string(REPLACE " --multibyte-chars --locale=japanese" "" CXX_FLAGS "${CXX_FLAGS}")
endif()

# Keep common options independent of the source list: per-source flags rewrite
# flags.make when a TU is added, invalidating every existing object.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} --cpu=MPCore")

SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_SOURCE_DIR}/data/conststring.cpp
    PROPERTIES
    COMPILE_FLAGS
    "--no_data_reorder"
    )

add_executable(oot3d ${csourcefiles} ${cxxsourcefiles})

add_custom_command(
    DEPENDS oot3d ${CMAKE_CURRENT_BINARY_DIR}/oot3d.axf
    COMMAND ${ARM_FROMELF} --bincombined --bincombined_padding=1,0x00 ${CMAKE_CURRENT_BINARY_DIR}/oot3d.axf --output ${CMAKE_CURRENT_BINARY_DIR}/code.bin
    COMMAND ${PYTHON_EXE} -c \"with open('${CMAKE_CURRENT_BINARY_DIR}/code.bin', 'a') as code: code.truncate(0x45B000) \"
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/code.bin
    COMMENT "Creating code.bin..."
)

add_custom_target(
    code
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/code.bin
)

add_custom_target(
    check
    DEPENDS code ${CMAKE_CURRENT_BINARY_DIR}/code.bin
    COMMENT "Checking for match..."
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && md5sum -c ${CMAKE_CURRENT_SOURCE_DIR}/data/checksum.md5
)

set(NON_MATCH_ASM_FILES ${CMAKE_CURRENT_BINARY_DIR}/nonmatch)
set(DISASSEMBLY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/disassembly)

# Extracted ARM code/data stays local. Only span/section metadata is versioned.
file(READ "${CMAKE_SOURCE_DIR}/tools/local_assets.json" LOCAL_ASSET_RECIPES)
set(LOCAL_ASSET_FILES)
string(JSON SPLIT_COUNT LENGTH "${LOCAL_ASSET_RECIPES}" splits)
math(EXPR SPLIT_LAST "${SPLIT_COUNT} - 1")
foreach(INDEX RANGE 0 ${SPLIT_LAST})
    string(JSON SPLIT_NAME GET "${LOCAL_ASSET_RECIPES}" splits ${INDEX} name)
    list(APPEND LOCAL_ASSET_FILES "${CMAKE_SOURCE_DIR}/split/asm/${SPLIT_NAME}.s")
endforeach()
string(JSON DATA_COUNT LENGTH "${LOCAL_ASSET_RECIPES}" data)
math(EXPR DATA_LAST "${DATA_COUNT} - 1")
foreach(INDEX RANGE 0 ${DATA_LAST})
    string(JSON DATA_PATH GET "${LOCAL_ASSET_RECIPES}" data ${INDEX} path)
    list(APPEND LOCAL_ASSET_FILES "${CMAKE_SOURCE_DIR}/${DATA_PATH}")
endforeach()
add_custom_command(
    OUTPUT "${CMAKE_SOURCE_DIR}/split/generated.stamp"
    BYPRODUCTS ${LOCAL_ASSET_FILES}
    COMMAND ${PYTHON_EXE} "${CMAKE_SOURCE_DIR}/tools/regenerate_local_assets.py"
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_SOURCE_DIR}/split/generated.stamp"
    DEPENDS disasm "${DISASSEMBLY_DIR}/split_asm.txt"
            "${CMAKE_SOURCE_DIR}/tools/local_assets.json"
            "${CMAKE_SOURCE_DIR}/tools/regenerate_local_assets.py"
    COMMENT "Regenerating local-only ARM assembly and data tails..."
)
add_custom_target(local_assets DEPENDS "${CMAKE_SOURCE_DIR}/split/generated.stamp")
file(MAKE_DIRECTORY ${NON_MATCH_ASM_FILES})

file(GLOB_RECURSE DISASSEMBLY_FILES CONFIGURE_DEPENDS
     "${DISASSEMBLY_DIR}/asm/*.s"
     "${DISASSEMBLY_DIR}/data/*.s")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${DISASSEMBLY_FILES})

foreach(CPP_FILE ${cxxsourcefiles})
    get_filename_component(FILE_NAME ${CPP_FILE} NAME_WE)
    set(DEP_FILE_LIST ${CPP_FILE})
    file(STRINGS ${CPP_FILE} ASM_REFS REGEX "^[ \t]*GLOBAL_ASM\\(\"")
    foreach(ASM_REF ${ASM_REFS})
        if(ASM_REF MATCHES "GLOBAL_ASM\\(\"([^\"]+)\"\\)")
            get_filename_component(ASM_DEPENDENCY "${DISASSEMBLY_DIR}/${CMAKE_MATCH_1}" ABSOLUTE)
            list(APPEND DEP_FILE_LIST "${ASM_DEPENDENCY}")
        endif()
    endforeach()

    add_custom_command(
        OUTPUT ${NON_MATCH_ASM_FILES}/${FILE_NAME}.s
        COMMAND ${PYTHON_EXE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/preproc.py ${CPP_FILE} ${NON_MATCH_ASM_FILES}/${FILE_NAME}.s "${DISASSEMBLY_DIR}/"
        DEPENDS ${DEP_FILE_LIST}
    )
    target_sources(oot3d PUBLIC ${NON_MATCH_ASM_FILES}/${FILE_NAME}.s)
endforeach()

target_link_libraries(oot3d PUBLIC 3ds_sdk)
set_property(TARGET oot3d APPEND PROPERTY LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/data/oot3d.ld)

target_link_options(oot3d PUBLIC
    --cpu=MPCore --entry=__ctr_start --startup=__ctr_start --library_type=standardlib
    --ref_cpp_init --scanlib --legacyalign
    --tailreorder --remove --datacompressor=off
    --inline --via ${CMAKE_CURRENT_SOURCE_DIR}/data/keep_symbols.txt
    --mangled --symbols --vfemode=force
    --scatter=${CMAKE_CURRENT_SOURCE_DIR}/data/oot3d.ld
    --diag_suppress=6238,6306
    )

target_include_directories(oot3d PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/library/3ds_sdk/include ${CMAKE_SOURCE_DIR}/src/game)

add_dependencies(oot3d disasm local_assets)
