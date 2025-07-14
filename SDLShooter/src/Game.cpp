#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include <SDL.h>       // SDL库
#include <SDL_image.h> // 加载图片
#include <SDL_mixer.h> // 音频
#include <SDL_ttf.h>   // 字体
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
        auto frameStart = SDL_GetTicks(); // 获取开始时间
        SDL_Event event;
        handleEvent(&event);

        update(deltaTime); // 更新游戏状态

        render();                       // 渲染游戏画面
        auto frameEnd = SDL_GetTicks(); // 获取结束时间
        auto diff = frameEnd - frameStart;
        if (diff < frameTime) // 如果时间差小于帧时间
        {
            // 需要等待延时
            SDL_Delay(frameTime - diff);     // 延迟
            deltaTime = frameTime / 1000.0f; // 毫秒转秒
        }
        else
        {
            deltaTime = diff / 1000.0f; // 毫秒转秒
        } // 稳定帧率
    }
}

// 初始化游戏
void Game::init()
{
    frameTime = 1000 / FPS; // 1秒是1000毫秒 / 60帧 = 16.666666666666668毫秒
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
    // SDL_WINDOW_SHOW ：窗口显示 京饿了美CNM配送员 我和我的五个儿子们
    window = SDL_CreateWindow("太空大战初学版", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
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

    // 初始化SDL_mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize! Mix_Error: %s\n", Mix_GetError());
        isRunning = false; // 初始化SDL_mixer失败，设置游戏运行状态为false
    }
    // 打开音频设备
    // 44100：采样率
    // MIX_DEFAULT_FORMAT：音频格式
    // 2：声道数
    // 2048：缓冲区大小
    // 这里的缓冲区大小可以根据需要调整
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not open audio! Mix_Error: %s\n", Mix_GetError());
        isRunning = false; // 初始化SDL_mixer失败，设置游戏运行状态为false
    }
    // 设置音效channel数量
    Mix_AllocateChannels(32);
    // 设置音量 MIX_MAX_VOLUME=128
    Mix_VolumeMusic(MIX_MAX_VOLUME / 6);
    // 设置音效的音量
    // channel:-1表示所有通道 默认8个音效 超过8个音效会覆盖
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);

    // 初始化TTF
    if (TTF_Init() == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        isRunning = false; // 初始化TTF失败，设置游戏运行状态为false
    }

    // 初始化背景卷轴
    // 近处星空背景
    nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
    if (nearStars.texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 初始化背景失败，设置游戏运行状态为false
    }

    // 获取纹理的宽度和高度
    SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
    nearStars.height /= 2; // 将高度缩小一半
    nearStars.width /= 2;  // 将宽度缩小一半

    // 远处星空背景
    farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
    if (farStars.texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture! SDL_Error: %s\n", SDL_GetError());
        isRunning = false; // 初始化背景失败，设置游戏运行状态为false
    }

    // 获取纹理的宽度和高度
    SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
    farStars.speed = 20;  // 远处星空背景滚动速度
    farStars.height /= 2; // 将高度缩小一半
    farStars.width /= 2;  // 将宽度缩小一半

    // 载入字体
    titleFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
    textFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 32);
    if (titleFont == nullptr || textFont == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s\n", TTF_GetError());
        isRunning = false;
    }

    // currentScene = new SceneMain(); // 创建一个新的场景对象
    currentScene = new SceneTitle(); // 创建一个新的场景对象
    currentScene->init();            // 初始化当前场景
}

// 清理游戏
void Game::clean()
{
    if (currentScene != nullptr) // 检查当前场景是否为空
    {
        currentScene->clean(); // 清理当前场景
        delete currentScene;   // 删除当前场景对象
    }
    if (nearStars.texture != nullptr) // 检查近处星空背景纹理是否为空
    {
        SDL_DestroyTexture(nearStars.texture); // 销毁近处星空背景纹理
    }
    if (farStars.texture != nullptr) // 检查远处星空背景纹理是否为空
    {
        SDL_DestroyTexture(farStars.texture); // 销毁远处星空背景纹理
    }
    if (titleFont != nullptr)
    {
        TTF_CloseFont(titleFont); // 关闭标题字体
    }
    if (textFont != nullptr)
    {
        TTF_CloseFont(textFont); // 关闭文本字体
    }

    // 清理SDL_image
    IMG_Quit();
    // 清理SDL_mixer
    Mix_CloseAudio(); // 关闭音频设备
    Mix_Quit();       // 退出SDL_mixer

    // 清理SDL_ttf
    TTF_Quit(); // 退出SDL_ttf

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

void Game::update(float deltaTime)
{
    backgroundUpdate(deltaTime);     // 更新背景滚动
    currentScene->update(deltaTime); //   更新当前场景
}

void Game::render()
{ // 清空
    SDL_RenderClear(renderer);
    // 渲染星空背景
    renderBackground();

    // 渲染当前场景
    currentScene->render();
    // 显示更新
    SDL_RenderPresent(renderer);
}

// 游戏类中的renderTextCentered函数，用于渲染文本
void Game::renderTextCentered(std::string text, float posY, bool isTitle)
{
    // TODO: 实现渲染文本的功能
    SDL_Color textColor = {255, 255, 255, 255}; // 白色
    SDL_Surface *surface;
    if (isTitle)
    {
        surface = TTF_RenderUTF8_Solid(titleFont, text.c_str(), textColor); // // 使用UTF-8编码的字体 中文不会乱码
    }
    else
    {
        surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), textColor);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); // 创建纹理
    int y = static_cast<int>(getWindowHeight() - surface->h) * posY;        // 计算y坐标
    SDL_Rect dstRect = {getWindowWidth() / 2 - surface->w / 2,
                        y,
                        surface->w,
                        surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect); // 渲染纹理
    SDL_DestroyTexture(texture);                       // 销毁纹理
    SDL_FreeSurface(surface);                          // 释放表面
}

// 更新游戏背景
void Game::backgroundUpdate(float deltaTime)
{
    // TODO: 在这里添加更新游戏背景的代码
    nearStars.offset += nearStars.speed * deltaTime; // 更新近处星空背景偏移
    if (nearStars.offset >= 0)                       // 如果偏移量超过高度
    {
        nearStars.offset -= nearStars.height; // 重置偏移量
    }

    farStars.offset += farStars.speed * deltaTime; // 更新远处星空背景偏移
    if (farStars.offset >= 0)                      // 如果偏移量超过高度
    {
        farStars.offset -= farStars.height; // 重置偏移量
    }
}

// 渲染背景
void Game::renderBackground()
{
    // TODO: 实现渲染背景的功能
    // 渲染远处星空背景

    for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height)
    {
        for (int posX = 0; posX < getWindowWidth(); posX += farStars.width)
        {
            SDL_Rect dstRect = {posX, posY, farStars.width, farStars.height}; // 设置渲染区域
            SDL_RenderCopy(renderer, farStars.texture, nullptr, &dstRect);    // 渲染远处星空背景
        }
    }

    // 渲染近处星空背景
    // 这里渲染星空背景需要for循环
    // 在更新的时候已经得到了offset,通过offset得到每个图片的y坐标

    // posY = 0; 从0开始渲染 当posY小于窗口高度时渲染 不再渲染
    for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
    {
        for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
        {
            SDL_Rect dstRect = {posX, posY, nearStars.width, nearStars.height}; // 设置渲染区域
            SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);     // 渲染近处星空背景
        }
    }
}
