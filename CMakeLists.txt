cmake_minimum_required(VERSION 3.6)
project(multiplatform)

option(USE_SYSTEM_SDL2 "Use system SDL2 (will result in an error if not found)" OFF)

# Windows doesn't seem to have an rpath equivalent, so we want all of our libraries and executables to be in the same place
if(WINDOWS)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
    set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin)
endif()

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
    if (NOT _INCLUDE_GLESv2 OR NOT _INCLUDE_GLESv3)
        message(FATAL_ERROR "Could not find headers for GLESv2/GLESv3. Please install development libraries for OpenGL ES and/or provide paths for them")
    else()
        message(STATUS "Using GLESv2/GLESv3 headers: ${_INCLUDE_GLESv2};${_INCLUDE_GLESv3}")
    endif()
    if (NOT _LIB_GLESv2)
        message(FATAL_ERROR "Could not find GLESv2 library. Please install GLESv2 for your platform and/or provide paths")
    else()
        message(STATUS "Using GLESv2 libraries: ${_LIB_GLESv2}")
    endif()
    target_include_directories(GLES2 INTERFACE ${_INCLUDE_GLESv2})
    target_include_directories(GLES3 INTERFACE ${_INCLUDE_GLESv3})
    target_link_libraries(GLES2 INTERFACE ${_LIB_GLESv2})
    # FIXME?: Again, GLESv3 seems to be Android-only
    target_link_libraries(GLES3 INTERFACE ${_LIB_GLESv2})
endif()

add_subdirectory(deps)

add_subdirectory(src)
