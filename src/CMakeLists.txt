# ImGui implementation goes first
add_library(imgui_impl STATIC
    imgui_impl/imgui_impl_opengl3.cpp
    imgui_impl/imgui_impl_sdl.cpp
)

target_link_libraries(imgui_impl PUBLIC imgui::imgui_core SDL2::SDL2)
target_compile_definitions(imgui_impl PUBLIC IMGUI_IMPL_OPENGL_ES2 IMGUI_IMPL_OPENGL_LOADER_CUSTOM)
target_include_directories(imgui_impl PUBLIC imgui_impl)

add_library(imgui::imgui_impl ALIAS imgui_impl)

# Android build will use the library (patched a bit by SDL)
if (ANDROID)
    add_library(main SHARED main.cpp)
else()
    add_executable(main main.cpp)
endif()

target_sources(main PRIVATE
    Application.cpp)

target_link_libraries(main PUBLIC
    SDL2::SDL2
    GLES::GLES3
    imgui::imgui_core
    imgui::imgui_impl
)

if (ANDROID)
    target_link_libraries(main PUBLIC dl)
endif()

target_compile_features(main PUBLIC cxx_std_11)
# Emscripten is hard :(
if(EMSCRIPTEN)
    message(STATUS "Building for Emscripten")
    set_target_properties(main PROPERTIES SUFFIX ".html")
endif()
