#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>

// 构造函数

SceneMain ::SceneMain() : game(Game::getInstance())
{

    // 调用Game类的getInstance()方法获取Game类的实例
}
// 析构函数，用于释放ScreneMain对象所占用的资源
SceneMain ::~SceneMain()
{
}
void SceneMain::update()
{
    // TODO: 在这里添加更新逻辑

    // 更新场景中的物体、角色等
}
// 1.初始化函数
void SceneMain::init()
{
    // TODO: 添加初始化代码
    // Game::getInstance() 获取game单例实例
    // getRenderer() 获取渲染器
    // 把Game::getInstance()当成成员变量使用
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");

    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", IMG_GetError());
    }

    // 跟着图片大小来显示玩家飞机大小
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width = player.width / 4;                                  // 玩具飞机的宽度除以6
    player.height = player.height / 4;                                // 玩具飞机的高度除以6
    player.position.x = game.getWindowWidth() / 2 - player.width / 2; // 居中显示
    player.position.y = game.getWindowHeight() - player.height - 20;  // 底
}
// 2.渲染函数
void SceneMain::render()
{
    // TODO: 添加渲染代码
    // 玩家飞机的矩形
    SDL_Rect playerRect = {
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height,
    };
    // 渲染玩家飞机
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
}

// 处理事件
void SceneMain::handleEvent(SDL_Event *event)
{

    // TODO: 处理事件
}

// 清空屏幕
void SceneMain::clean()
{
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture); // 销毁玩家飞机纹理
    }

    // 清空屏幕的具体实现
}