#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

    // Vertex and RGB data
    GLfloat vertexData[] = {
    // Position (x, y, z)      // Color (r, g, b)
     0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f, // Upper right (red)
     0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, // Lower right (green)
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, // Lower left (blue)
    -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f  // Upper left (yellow)
    };

    // Index data for two triangles
    GLuint indices[] = {
        0, 1, 3, 
        1, 2, 3 
    };


int main() {

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
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
    SDL_Window* window = SDL_CreateWindow("Organizing", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return -1;
    }

    // Create a context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Drawing a context
    if (!SDL_GL_MakeCurrent(window, glContext)) {
        std::cerr << "Failed to make context current: " << SDL_GetError() << "\n";
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //  Set initialize viewport
    glViewport(0, 0, 800, 600);

    // Path to shaders file
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

    // Generates VAO and binds it
    VAO vao1;
    vao1.Bind();

    // Generates VBO and links it to vertices
    VBO vbo1(vertexData, sizeof(vertexData));
    // Generates EBO and links it to indices
    EBO ebo1(indices, sizeof(indices));

    
    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    // Configure vertex color attribute (location = 1)
    vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    // Window loop
    bool loop = true;
    while (loop) {
        // Event handling
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            // Window close button
            if (e.type == SDL_EVENT_QUIT) loop = false;
            // Exit if the Escape key is pressed
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) loop = false;
            // Window resizing
            if (e.type == SDL_EVENT_WINDOW_RESIZED) glViewport(0, 0, e.window.data1, e.window.data2);
        }

        // Specify background color
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        // Clean the back buffer and assign a new color
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader program
        shaderProgram.Activate();
        // Bind the VAO so OpenGL knows to use it
        vao1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        SDL_GL_SwapWindow(window);
    }

    // Release resources
    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    shaderProgram.Delete();

    // SDL Shutdown
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
