#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

// Forward Declarations
void framebuffer_size_callback(int width, int height);
void processInput(bool* window_loop); 

// Global constants for the initial window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char* argv[])
{

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

// OpenGL context for macOS
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
            "Triangle", 
            SCR_WIDTH, 
            SCR_HEIGHT, 
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Drawing context
    if (!SDL_GL_MakeCurrent(window, glContext)) {
        std::cerr << "Failed to make context current: " << SDL_GetError() << std::endl;
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Contorol V-Sync
    SDL_GL_SetSwapInterval(1);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }


    // GPU information
    std::cout << "GL_VENDOR: " << (const char*)glGetString(GL_VENDOR) << "\n";
    std::cout << "GL_RENDERER: " << (const char*)glGetString(GL_RENDERER) << "\n";
    std::cout << "GL_VERSION: " << (const char*)glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL     : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    // Main loop
    bool window_loop = true;
    while (window_loop)
    {
        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Window close button
            if (event.type == SDL_EVENT_QUIT) {
                window_loop = false;
            }
            // Exit if the Escape key is pressed
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                window_loop = false;
            }
            // Window resizing
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                framebuffer_size_callback(event.window.data1, event.window.data2);
            }
        }

        // Rendering clear
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Swap the back buffer with the front buffer
        SDL_GL_SwapWindow(window);
    }
    
    // Termination process
    std::cout << "SDL shutdown ..." << std::endl;
    
    // SDL shutdown
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// Input processing
void processInput(bool* window_loop)
{
    const bool* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        *window_loop = false;
    }
}

// Window Resize Logic
void framebuffer_size_callback(int width, int height)
{
    glViewport(0, 0, width, height);
}
