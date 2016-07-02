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

add_library(GLL SHARED ${SOURCES} ${HEADERS})
add_library(GLL-static STATIC ${SOURCES} ${HEADERS})

target_include_directories(GLL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_include_directories(GLL-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(GLL PUBLIC ${OPENGL_LIBRARY})
target_link_libraries(GLL-static INTERFACE ${OPENGL_LIBRARY})

target_compile_definitions(GLL-static PUBLIC -DGLL_STATIC)
target_compile_definitions(GLL PRIVATE -DGLL_EXPORT)

if (WIN32)
    set_target_properties(GLL PROPERTIES DEBUG_POSTFIX -d)
    set_target_properties(GLL-static PROPERTIES DEBUG_POSTFIX -d)
    set_target_properties(GLL-static PROPERTIES OUTPUT_NAME GLL-static)
else ()
    set_target_properties(GLL-static PROPERTIES OUTPUT_NAME GLL)
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
    TARGETS GLL GLL-static
    EXPORT GLL-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(EXPORT GLL-targets DESTINATION lib/GLL)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/GLL" DESTINATION include)
install(FILES "${CMAKE_SOURCE_DIR}/cmake/GLL-config.cmake" DESTINATION lib/GLL)

