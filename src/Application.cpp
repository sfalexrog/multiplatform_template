#include "Application.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <GLES2/gl2.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
void emscriptenLoopTrampoline(void* arg)
{
    Application *appPtr = reinterpret_cast<Application*>(arg);
    appPtr->loopOnce();
}

#endif

Application::Application(int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL initialized");
    window = SDL_CreateWindow(argv[0], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 800, SDL_WINDOW_OPENGL);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window created");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    ctx = SDL_GL_CreateContext(window);
    if (!ctx) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create context!");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, ctx);
    ImGui_ImplOpenGL3_Init();

}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Deleting OpenGL context");
    SDL_GL_DeleteContext(ctx);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Destroying window");
    SDL_DestroyWindow(window);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Exiting");
    SDL_Quit();
}

int Application::run()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Entering main loop");
#ifdef __EMSCRIPTEN__
    // Emscripten needs special treatment - it wants a "main loop" function that it will call itself
    emscripten_set_main_loop_arg(&emscriptenLoopTrampoline, this, -1, true);
#else
    // We perform a "main loop" calling here
    while (!shouldQuit) {
        loopOnce();
    }
#endif
    return 0;
}

void Application::loopOnce()
{
    SDL_Event ev;
    ImGuiIO& io = ImGui::GetIO();
    while(SDL_PollEvent(&ev)) {
        ImGui_ImplSDL2_ProcessEvent(&ev);
        if (ev.type == SDL_QUIT) {
            shouldQuit = true;
        }
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}
