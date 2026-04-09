#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Forward Declarations
void framebuffer_size_callback(int width, int height);
void processInput(bool* window_loop); 

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char* argv[]) {

    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("Triangle", SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    glewExperimental = GL_TRUE;
    glewInit();

    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

    GLfloat vertexData[] = {
    // Position (x, y, z)      // Color (r, g, b)
     0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 
    -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f  
    };

    GLuint indices[] = {
        0, 1, 3, 
        1, 2, 3 
    };

    VAO vao1;
    vao1.Bind();

    VBO vbo1(vertexData, sizeof(vertexData));
    EBO ebo1(indices, sizeof(indices));

    vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    bool window_loop = true;
    while (window_loop) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) window_loop = false;
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                window_loop = false;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) glViewport(0, 0, event.window.data1, event.window.data2);
        }

        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        vao1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    shaderProgram.Delete();

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
