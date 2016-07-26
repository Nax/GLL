cmake_minimum_required(VERSION 3.0)
project(GLL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

find_package(OpenGL)

include_directories(src)

file(GLOB_RECURSE HEADERS "include/**/*.h")
file(GLOB GL_HEADERS "include/GLL/GL/*.h")

set(SOURCES src/GLL.c src/GLL_OpenGL.c)

if (WIN32)
    list(APPEND SOURCES src/GLL_Windows.c)
endif()

add_library(gll SHARED ${SOURCES} ${HEADERS})
add_library(gll-static STATIC ${SOURCES} ${HEADERS})

target_include_directories(gll PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_include_directories(gll-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(gll PUBLIC ${OPENGL_LIBRARY})
target_link_libraries(gll-static INTERFACE ${OPENGL_LIBRARY})

target_compile_definitions(gll-static PUBLIC -DGLL_STATIC)
target_compile_definitions(gll PRIVATE -DGLL_EXPORT)

if (WIN32)
    set_target_properties(gll PROPERTIES DEBUG_POSTFIX -d)
    set_target_properties(gll-static PROPERTIES DEBUG_POSTFIX -d)
    set_target_properties(gll-static PROPERTIES OUTPUT_NAME GLL-static)
else ()
    set_target_properties(gll-static PROPERTIES OUTPUT_NAME GLL)
endif ()

add_custom_command(
    OUTPUT
    "${CMAKE_SOURCE_DIR}/include/GLL/GLL_OpenGL.h"
    "${CMAKE_SOURCE_DIR}/src/GLL_OpenGL.c"
    DEPENDS
    "${CMAKE_SOURCE_DIR}/data/GLL_OpenGL.h.in"
    "${CMAKE_SOURCE_DIR}/data/GLL_OpenGL.c.in"
    "${CMAKE_SOURCE_DIR}/data/OpenGL.txt"
    "${CMAKE_SOURCE_DIR}/tool/generate_opengl.rb"
    COMMAND
    "ruby"
    "${CMAKE_SOURCE_DIR}/tool/generate_opengl.rb"
    "${CMAKE_SOURCE_DIR}/data/OpenGL.txt"
    "${CMAKE_SOURCE_DIR}/data/GLL_OpenGL.h.in"
    "${CMAKE_SOURCE_DIR}/data/GLL_OpenGL.c.in"
    "${CMAKE_SOURCE_DIR}/include/GLL/GLL_OpenGL.h"
    "${CMAKE_SOURCE_DIR}/src/GLL_OpenGL.c"
    COMMENT
    "Regenerating OpenGL function export"
    VERBATIM)

add_custom_command(
    OUTPUT
    "${CMAKE_SOURCE_DIR}/data/OpenGL.txt"
    DEPENDS
    ${GL_HEADERS}
    "${CMAKE_SOURCE_DIR}/tool/extract_opengl.rb"
    COMMAND
    "ruby"
    "${CMAKE_SOURCE_DIR}/tool/extract_opengl.rb"
    "${CMAKE_SOURCE_DIR}/data/OpenGL.txt"
    ${GL_HEADERS}
    COMMENT
    "Extracting OpenGL functions"
    VERBATIM
    )

install(
    TARGETS gll gll-static
    EXPORT gll-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(EXPORT gll-targets DESTINATION cmake)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/GLL" DESTINATION include)
install(FILES "${CMAKE_SOURCE_DIR}/cmake/GLL-config.cmake" DESTINATION cmake)

