#include "SceneMain.h"
#include <SDL.h>
#include <SDL_image.h>

// 构造函数
SceneMain ::SceneMain()
{
}
// 析构函数，用于释放ScreneMain对象所占用的资源
SceneMain ::~SceneMain()
{
}

// 初始化函数
void SceneMain::init()
{
    // TODO: 添加初始化代码
    // player->texture = IMG_LoadTexture(renderer, "assets/player.png");
}

void SceneMain::update()
{
    // TODO: 在这里添加更新逻辑

    // 更新场景中的物体、角色等
}
// 渲染函数
void SceneMain::render()
{

    // TODO: 添加渲染代码
}
// 处理事件
void SceneMain::handleEvent(SDL_Event *event)
{

    // TODO: 处理事件
}

// 清空屏幕
void SceneMain::clean()
{

    // 清空屏幕的具体实现
}