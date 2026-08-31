set (THIRD_PARTIES ${CMAKE_SOURCE_DIR}/filters/third_parties)
set (THIRD_PARTIES_BINARIES ${CMAKE_BINARY_DIR}/filters/third_parties)
set (GPAC_LOCATION ${THIRD_PARTIES}/gpac)
set (GPAC_BINARIES ${THIRD_PARTIES_BINARIES}/gpac_minimal)

set(VERSION "1")

set(USE_DEBUG "-g")
set(USE_OPT "-O3")

if(DEBUG_MODE)
  set(USE_FLAGS "${USE_DEBUG}")
else()
  set(USE_FLAGS "${USE_OPT}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")

add_compile_definitions(GPAC_HAVE_CONFIG_H GPAC_DISABLE_DOC)


include(exports.cmake)

SET(solver_minimal_SRC
  event.c
  filter_register.c
  properties.cpp
  stats.c
  gpac.c
  legacy_abi.c
)

SET(EXTERNAL_RUNTIME
  "['stringToUTF8','UTF8ToString','stackAlloc','addFunction','allocateUTF8OnStack','stackSave','getTempRet0','setTempRet0','FS','cwrap','ccall','setValue','lengthBytesUTF8']"
)

add_executable(solver_minimal_${VERSION} ${solver_minimal_SRC})

SET(JS_RUNTIME
  ${CMAKE_CURRENT_SOURCE_DIR}/loader.js
)

set_target_properties( solver_minimal_${VERSION}
                            PROPERTIES
                                LINK_FLAGS "${USE_FLAGS} -fexceptions -sALLOW_MEMORY_GROWTH -sMAIN_MODULE=2 -sEXPORT_NAME=libgpac -sUSE_ZLIB=1 --no-entry -sMODULARIZE=1 -sEXPORTED_FUNCTIONS=[${EXPORTED_FUNCTIONS}] --extern-post-js=${JS_RUNTIME} -sEXPORTED_RUNTIME_METHODS=${EXTERNAL_RUNTIME} -sERROR_ON_UNDEFINED_SYMBOLS=0 -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE=_emscripten_fs_load_embedded_files -sINITIAL_MEMORY=33554432 -sSTACK_SIZE=5MB -Wl,--export=getTempRet0 -Wl,--export=setTempRet0 -Wl,--export=exit -Wl,--export=__cxa_begin_catch -Wl,--export=__cxa_find_matching_catch_2 -Wl,--export=__cxa_find_matching_catch_3 -Wl,--export=__cxa_throw -Wl,--export=__resumeException -Wl,--export=_Znam -Wl,--export=_ZdaPv -Wl,--export=__cxa_atexit"
                                )



target_link_libraries(solver_minimal_${VERSION}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgpac_static.a
)

target_include_directories(solver_minimal_${VERSION} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
