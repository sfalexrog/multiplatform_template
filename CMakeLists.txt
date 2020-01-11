cmake_minimum_required(VERSION 3.6)
project(multiplatform)

option(USE_SYSTEM_SDL2 "Use system SDL2 (will result in an error if not found)" OFF)

# Setup some OpenGL libraries while we're here
add_library(GLES2 INTERFACE)
add_library(GLES3 INTERFACE)
add_library(GLES::GLES2 ALIAS GLES2)
add_library(GLES::GLES3 ALIAS GLES3)
if(EMSCRIPTEN)
    # For some reason, linker does accept quoted string as a parameter, but compiler does not.
    target_link_libraries(GLES3 INTERFACE "-s USE_WEBGL2=1")
    #target_compile_options(GLES3 INTERFACE -s USE_WEBGL2=1)
else()
    # Find actual GLESv2/GLESv3 libraries
    find_library(_LIB_GLESv2 GLESv2)
    # FIXME?: mesa does not provide a separate GLESv3 library, but Android does?
    # find_library(_LIB_GLESv3 GLESv3)
    find_path(_INCLUDE_GLESv2 GLES2/gl2.h)
    find_path(_INCLUDE_GLESv3 GLES3/gl3.h)
    target_include_directories(GLES2 INTERFACE ${_INCLUDE_GLESv2})
    target_include_directories(GLES3 INTERFACE ${_INCLUDE_GLESv3})
    target_link_libraries(GLES2 INTERFACE ${_LIB_GLESv2})
    # FIXME?: Again, GLESv3 seems to be Android-only
    target_link_libraries(GLES3 INTERFACE ${_LIB_GLESv2})
endif()

add_subdirectory(deps)

add_subdirectory(src)
