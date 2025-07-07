#include "Game.h"
#include "SDL.h"

 Game::Game()
{
}

Game::~Game()
{
}

void Game::run()
{
    while (isRunning)
    {
        SDL_Event event;
        handleEvent(event);
  
       currentScene->update(); //   更新当前场景

       // 清空
       SDL_RenderClear(renderer);
       // 渲染当前场景 
       currentScene->render();   
       // 显示更新
       SDL_RenderPresent(renderer);
    }
    
}

// 初始化游戏
void Game::init()
{
    // TODO: 在这里添加初始化游戏的代码
    //SDL 初始化
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {   
        // SDL_LogError
        //参数category 1：日志类别
        //参数char *fmt 2：日志信息
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 初始化失败，设置游戏运行状态为false
    }
    // 创建窗口
    window = SDL_CreateWindow("SDL Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_FULLSCREEN);
    if(window == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not create window! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 创建窗口失败，设置游戏运行状态为false
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not create renderer! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 创建渲染器失败，设置游戏运行状态为false
    }
    
}

// 清理游戏
void Game::clean()
{
    // 清理游戏资源
    SDL_DestroyRenderer(renderer); // 销毁渲染器
    SDL_DestroyWindow(window);// 销毁窗口
    SDL_Quit();// 退出SDL
}

void Game::changeScene(Scene *scene)
{   
     // 当前场景是否为空
    if(currentScene != nullptr)
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

void Game::handleEvent(SDL_Event event)
{
          while (SDL_PollEvent(&event))// 检测事件
        {
            if(event.type ==SDL_QUIT) // 检测退出事件
            {
                isRunning = false;
            }
            currentScene->handleEvent(&event); // 处理事件

        }
}

void Game::update()
{

}

void Game::render()
{

}