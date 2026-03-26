#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // SDL3: initialize
    // ----------------
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // SDL3: OpenGL context settings
    // -----------------------------
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_SetHint(SDL_HINT_X11_WINDOW_TYPE, "_NET_WM_WINDOW_TYPE_DIALOG");

#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    // SDL3: window creation
    // ---------------------
    SDL_Window* window = SDL_CreateWindow(
        "LearnOpenGL",
        SCR_WIDTH, SCR_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == nullptr)
    {
        std::cout << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // SDL3: OpenGL context creation
    // -----------------------------
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_GL_MakeCurrent(window, glContext);

    // GLEW: load all OpenGL function pointers
    // ----------------------------------------
    glewExperimental = GL_TRUE; // Core Profileに必要
    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW: "
                  << glewGetErrorString(glewErr) << std::endl;
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // render loop
    // -----------
    bool running = true;
    while (running)
    {
        // input / イベント処理
        // --------------------
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                int w, h;
                SDL_GetWindowSizeInPixels(window, &w, &h);
                glViewport(0, 0, w, h);
            }
        }

        // カラーバッファをクリアする色を設定 (R, G, B, A) ※値は 0.0〜1.0
        glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
        // カラーバッファを上で設定した色でクリア（塗りつぶす）
        glClear(GL_COLOR_BUFFER_BIT);
        // SDL3: swap buffers
        // ------------------
        SDL_GL_SwapWindow(window);
    }

    // SDL3: terminate, clearing all previously allocated resources.
    // -------------------------------------------------------------
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
