# Use bundled SDL2 if requested or system SDL2 not found

if (${USE_SYSTEM_SDL2})
    find_library(_SDL2_LIB SDL2)
    find_library(_SDL2MAIN_LIB SDL2main)
    find_path(_SDL2_INCLUDE SDL2/SDL.h)

    if (NOT _SDL2_LIB OR NOT _SDL2MAIN_LIB OR NOT _SDL2_INCLUDE)
        message(FATAL_ERROR "Requested to use system SDL2, but system has no SDL2 installed; retry with USE_SYSTEM_SDL=OFF")
    else()
        message(STATUS "Using system SDL2 (libraries: ${_SDL2_LIB}, ${_SDL2MAIN_LIB}; includes at ${_SDL2_INCLUDE})")
        add_library(SDL2 INTERFACE)
        target_include_directories(SDL2 INTERFACE ${_SDL2_INCLUDE}/SDL2)
        target_link_libraries(SDL2 INTERFACE ${_SDL2_LIB} ${_SDL2MAIN_LIB})
    endif()
else()
    message(STATUS "Using bundled SDL2 libraries")
    if (ANDROID)
        message(STATUS "Android: Using a workaround for libdl.so linkage")
        find_library(ANDROID_DL_LIBRARY NAMES libdl.so dl)
        message(STATUS "ANDROID_DL_LIBRARY set to ${ANDROID_DL_LIBRARY}, hope this will convince SDL2")
#        message(STATUS "Disabling hidapi for Android build (workaround for SDL 2.0.10 build/link issues)")
#        set(HIDAPI OFF)
#        # Create a fake hidapi target
#        add_library(hidapi SHARED "")
    endif()
    add_subdirectory(SDL2)
#    if (TARGET hidapi)
#        get_target_property(_SDL2_LINK_LIBRARIES SDL2 LINK_LIBRARIES)
#        message(STATUS "SDL2 LINK_LIBRARIES: ${_SDL2_LINK_LIBRARIES}")
#        set_target_properties(SDL2 PROPERTIES LINK_LIBRARIES "${_SDL2_LINK_LIBRARIES};hidapi")
#    endif()
endif()

# Add a convenience target
if (EMSCRIPTEN)
    # No point in shared libraries for Emscripten build, right?
    add_library(SDL2::SDL2 ALIAS SDL2-static)
else()
    add_library(SDL2::SDL2 ALIAS SDL2)
    add_library(SDL2::SDL2-static ALIAS SDL2-static)
endif()

# Always use bundled imgui (not that we have any say in the matter...)
# We specify static linkage since imgui has no business being a shared library
add_library(imgui STATIC
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
)

target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
    $<INSTALL_INTERFACE:imgui>
)

add_library(imgui::imgui_core ALIAS imgui)
