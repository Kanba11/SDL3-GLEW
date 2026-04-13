#include<filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

    // Vertex and RGB data
    GLfloat vertexData[] = {
    // Position (x, y, z)   // Color (r, g, b)
     -0.5f,  -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Lower left
     -0.5f,   0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Upper left
      0.5f,   0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Upper right
      0.5f,  -0.5f, 0.0f,    1.0f, 1.0f, 0.0f,  1.0f, 0.0f  // Lower right
    };

    // Index data for two triangles
    GLuint indices[] = {
        0, 2, 1, 
        0, 3, 2 
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
    SDL_Window* window = SDL_CreateWindow("Organizing", 800, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    glViewport(0, 0, 800, 800);

    // Path to shaders file
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

    // Generates VAO and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates VBO and links it to vertices
    VBO VBO1(vertexData, sizeof(vertexData));
    // Generates EBO and links it to indices
    EBO EBO1(indices, sizeof(indices));

    
    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // Configure vertex color attribute (location = 1)
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    std::string parentDir = fs::current_path().string();
    std::string texPath = "/Resources/";

    std::string fullPath = parentDir + texPath + "crate.png";
    std::cout << "Loading texture from: " << fullPath << std::endl;

    // Texture
    Texture crate((parentDir + texPath + "crate.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    crate.texUnit(shaderProgram, "tex0", 0);

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
            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                int w = e.window.data1;
                int h = e.window.data2;
                int s = (w < h) ? w : h;
                glViewport((w - s) / 2, (h - s) / 2, s, s);
            }
        }

        // Specify background color
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        // Clean the back buffer and assign a new color
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader program
        shaderProgram.Activate();
        crate.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        SDL_GL_SwapWindow(window);
    }

    // Release resources
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // SDL Shutdown
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
