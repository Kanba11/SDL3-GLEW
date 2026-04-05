#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

// Forward Declarations
void framebuffer_size_callback(int width, int height);
void processInput(bool* window_loop); 

// Global constants for the initial window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex Shader
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
// Fragment Shader
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.95f, 0.2f, 1.0f);\n"
    "}\n\0";


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

    // set initial viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Stores whether shader compile/program link succeeded
    GLint success;
    // Buffer to store shader/program error messages (up to 512 chars)    
    char infoLog[512];

    // Create a vertex shader object
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach vertex shader source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    // Compile the vertex shader
    glCompileShader(vertexShader);
    // Check compile status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create a fragment shader object
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach fragment shader source code
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    // Compile the fragment shader
    glCompileShader(fragmentShader);
    // Check compile status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create a shader program object
    GLuint shaderProgram = glCreateProgram();
    // Attach the vertex and fragment shader to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Link attached shaders into an executable program
    glLinkProgram(shaderProgram);
    // Check link status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete standalone
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // vertex data
    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f, // Upper right
         0.5f, -0.5f, 0.0f, // Lower right
        -0.5f, -0.5f, 0.0f, // Lower left
        -0.5f,  0.5f, 0.0f  // Upper left
    };

    // Two triangular Index data
    GLuint indices[] = {
        0, 1, 3, 
        1, 2, 3  
    };

    // Create reference containers e Vartex Array Object, the Vertex Buffer Object, and the Element Buffer Object
    GLuint VBO, VAO, EBO;
    // Generate VAO and VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO to record vertex attribute settings
    glBindVertexArray(VAO);

    // Bind VBO as the current array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Introduce the indices into the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Unbind array buffer and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
            // Exit if the Escape key is pressed
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                window_loop = false;
            }
            // Window resizing
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
        }

        // Rendering clear
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader program
        glUseProgram(shaderProgram);
        // Bind the VAO so OpenGL knows to use it
        glBindVertexArray(VAO);
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        SDL_GL_SwapWindow(window);
    }

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Termination process
    std::cout << "SDL shutdown ..." << std::endl;
    
    // SDL Shutdown
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
