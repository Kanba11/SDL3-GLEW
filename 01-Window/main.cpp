#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

// --- 宣言部分 (Prototype) ---
void framebuffer_size_callback(int width, int height);
void processInput(bool* window_loop); 


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    // OpenGL属性設定などは省略（前回と同じ）
    // 2. ウィンドウの作成
    SDL_Window* window = SDL_CreateWindow("OpenGL Window", SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // 3. コンテキストの作成
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_GL_MakeCurrent(window, glContext);

    // GLEWの初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        // 失敗した場合はここでも個別破棄
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool window_loop = true;
    SDL_Event event;

    // --- メインループ ---
    while (window_loop)
    {
        // 1. イベント処理（ここでサイズ変更などを検知して関数を呼ぶ）
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) window_loop = false;
            
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                // ここで自作のコールバック関数を呼ぶ
                framebuffer_size_callback(event.window.data1, event.window.data2);
            }
        }

        // 2. 入力処理
        processInput(&window_loop);

        // 3. レンダリング
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    // --- 終了処理 (ここが今回のポイント) ---
    // 作成した逆の順番で破棄していくのが最も安全です
    std::cout << "Cleaning up..." << std::endl;
    
    SDL_GL_DestroyContext(glContext); // まずOpenGLの脳（Context）を消す
    SDL_DestroyWindow(window);        // 次にキャンバス（Window）を消す
    SDL_Quit();                       // 最後にSDL全体を終了

    return 0;
}

// --- 定義部分 (Implementation) ---

// 入力処理：キーの状態を直接チェックする
void processInput(bool* window_loop)
{
    const bool* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        *window_loop = false; // フラグを書き換えてループを終了させる
    }
}

// サイズ変更処理
void framebuffer_size_callback(int width, int height)
{
    glViewport(0, 0, width, height);
}
