#include "Game.h"
#include "SceneMain.h"
#include <SDL.h>
#include <SDL_image.h>
Game::Game()
{
}

/// @brief 清理游戏资源
/// 销毁自动调用
Game::~Game()
{
    // 在析构函数里调用
    clean(); // 清理游戏资源
}

void Game::run()
{
    while (isRunning)
    {
        SDL_Event event;
        handleEvent(&event);

        update(); // 更新游戏状态

        render(); // 渲染游戏画面
    }
}

// 初始化游戏
void Game::init()
{
    // TODO: 在这里添加初始化游戏的代码
    // SDL 初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        // SDL_LogError
        // 参数category 1：日志类别
        // 参数char *fmt 2：日志信息
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 初始化失败，设置游戏运行状态为false
    }
    // 创建窗口
    // SDL_WINDOWPOS_CENTERED：窗口位置居中
    // SDL_WINDOW_SHOW ：窗口显示
    window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not create window! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 创建窗口失败，设置游戏运行状态为false
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not create renderer! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 创建渲染器失败，设置游戏运行状态为false
    }
    // 初始化SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        isRunning = false; // 初始化SDL_image失败，设置游戏运行状态为false
    }

    currentScene = new SceneMain(); // 创建一个新的场景对象
    currentScene->init();           // 初始化当前场景
}

// 清理游戏
void Game::clean()
{
    if (currentScene != nullptr)
    {
        currentScene->clean(); // 清理当前场景
        delete currentScene;   // 删除当前场景对象
    }
    // 清理SDL_image
    IMG_Quit();
    // 清理游戏资源
    SDL_DestroyRenderer(renderer); // 销毁渲染器
    SDL_DestroyWindow(window);     // 销毁窗口
    SDL_Quit();                    // 退出SDL
}

void Game::changeScene(Scene *scene)
{
    // 当前场景是否为空
    if (currentScene != nullptr)
    {
        // 清理当前场景
        currentScene->clean();
        // 删除当前场景
        delete currentScene;
    }
    // 设置当前场景
    currentScene = scene;
    // 初始化当前场景
    currentScene->init();
}

void Game::handleEvent(SDL_Event *event)
{
    while (SDL_PollEvent(event)) // 检测事件
    {
        if (event->type == SDL_QUIT) // 检测退出事件
        {
            isRunning = false;
        }
        currentScene->handleEvent(event); // 处理事件
    }
}

void Game::update()
{
    currentScene->update(); //   更新当前场景
}

void Game::render()
{ // 清空
    SDL_RenderClear(renderer);
    // 渲染当前场景
    currentScene->render();
    // 显示更新
    SDL_RenderPresent(renderer);
}