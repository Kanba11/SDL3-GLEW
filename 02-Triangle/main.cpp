#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

int main()
{
    // SDL3 Initialization
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Specify OpenGL 3.3 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create Window（800x800）
    SDL_Window* window = SDL_CreateWindow("Window", 800, 800, SDL_WINDOW_OPENGL);

    // OpenGL context creation
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // GLEW Initialization 
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    
    // Viewport Settings
    glViewport(0, 0, 800, 800);

    // Set background color and clear
    glClearColor(0.50f, 0.50f, 1.5f, 1.0f);

    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);

    // Display in front buffer
    SDL_GL_SwapWindow(window);

    // Main while loop
    bool running = true;
    SDL_Event event;
    while (running)
    {
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
        }
    }

    // Delete context and window ending the program 
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
