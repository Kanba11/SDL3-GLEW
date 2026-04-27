#include<filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

    // Specify screen size
    const unsigned int width = 800;
    const unsigned int height = 800;

    // Vertex, RGB, Texture, and Normal data
    GLfloat vertexData[] = {
    // Position            // Color             // TexCoords  // Normals (面の向き)
    // Bottom face (y = -0.5f)
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f,   0.0f, -1.0f,  0.0f,

    // Top face (y = 0.5f)
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f,   0.0f,  1.0f,  0.0f,

    // Front face (z = 0.5f)
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f,   0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 0.0f,   0.0f,  0.0f,  1.0f,

    // Back face (z = -0.5f)
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 0.0f,   0.0f,  0.0f, -1.0f,

    // Left face (x = -0.5f)
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

    // Right face (x = 0.5f)
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 0.0f,   1.0f,  0.0f,  0.0f
    };

    GLuint indices[] = {
    // Bottom
    0, 1, 2,  0, 2, 3,
    // Top
    4, 6, 5,  4, 7, 6,
    // Front
    8, 9, 10,  8, 10, 11,
    // Back
    12, 14, 13,  12, 15, 14,
    // Left
    16, 18, 17,  16, 19, 18,
    // Right
    20, 21, 22,  20, 22, 23
    };

    GLfloat lightVertices[] = { 
        //     COORDINATES     //
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };

    GLuint lightIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
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
    SDL_Window* window = SDL_CreateWindow("Lighting", width, height, SDL_WINDOW_OPENGL);
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

    SDL_GL_SetSwapInterval(1);

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
    glViewport(0, 0, width, height);

    // Path to shaders file
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

    // Generates VAO and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates VBO and links it to vertices
    VBO VBO1(vertexData, sizeof(vertexData));
    // Generates EBO and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Position (Location 0)
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    // Color (Location 1)
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    // Texture (Location 2)
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    // Normals (Location 3)
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Shader for light cube
    Shader lightShader("Shaders/light.vert", "Shaders/light.frag");
    // Generates Vertex Array Object and binds it
    VAO lightVAO;
    lightVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    // Generates Element Buffer Object and links it to indices
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    // Links VBO attributes such as coordinates and colors to VAO
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // Unbind all to prevent accidentally modifying them
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 1.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);


    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    std::string parentDir = fs::current_path().string();
    std::string texPath = "/Resources/";

    // Texture 
    Texture crate((parentDir + texPath + "crate.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    crate.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.8f, 0.8f, 4.0f));

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
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign a new color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tell OpenGL which Shader Program we want to use
	shaderProgram.Activate();
	// Handles camera inputs
	camera.Inputs(window);
	// Updates and exports the camera matrix to the Vertex Shader
	camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
        // Activate the shader program
        crate.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        lightShader.Activate();
        camera.Matrix(45.0f, 0.1f, 100.0f, lightShader, "camMatrix"); 
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        SDL_GL_SwapWindow(window);
    }

    // Release resources
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    crate.Delete();
    shaderProgram.Delete();
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    lightShader.Delete();

    // SDL Shutdown
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
