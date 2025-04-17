set (THIRD_PARTIES ${CMAKE_SOURCE_DIR}/filters/third_parties)
set (THIRD_PARTIES_BINARIES ${CMAKE_BINARY_DIR}/filters/third_parties)
set (GPAC_LOCATION ${THIRD_PARTIES}/gpac)
set (GPAC_BINARIES ${THIRD_PARTIES_BINARIES}/gpac_minimal)

set(VERSION "1")

#set(USE_DEBUG "-g")
#set(USE_FLAGS "${USE_DEBUG}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_C_FLAGS "-O3 -Wall -Wextra")

add_compile_definitions(GPAC_HAVE_CONFIG_H GPAC_DISABLE_DOC)


include(exports.cmake)

SET(solver_minimal_SRC
  event.c
  filter_register.c
  properties.cpp
  stats.c
  gpac.c
)

SET(EXTERNAL_RUNTIME
  "['stringToUTF8','UTF8ToString','stackAlloc','addFunction','allocateUTF8OnStack','stackSave','getTempRet0','setTempRet0','FS','cwrap','ccall']"
)

add_executable(solver_minimal_${VERSION} ${solver_minimal_SRC})

SET(JS_RUNTIME
  ${CMAKE_CURRENT_SOURCE_DIR}/loader.js
)

set_target_properties( solver_minimal_${VERSION}
                            PROPERTIES
                                LINK_FLAGS "${USE_FLAGS} -sALLOW_MEMORY_GROWTH -sWASM_BIGINT -s MAIN_MODULE=2 -s EXPORT_NAME=libgpac --no-entry -s MODULARIZE=1 -s EXPORTED_FUNCTIONS=[${EXPORTED_FUNCTIONS}] -s --extern-post-js=${JS_RUNTIME} EXPORTED_RUNTIME_METHODS=${EXTERNAL_RUNTIME} -s ERROR_ON_UNDEFINED_SYMBOLS=0"
                                )



target_link_libraries(solver_minimal_${VERSION}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgpac_static.a
)

target_include_directories(solver_minimal_${VERSION} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
