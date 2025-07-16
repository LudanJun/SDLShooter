#include "SceneTitle.h"
#include "Game.h"
#include "SceneMain.h"
#include <string>

void SceneTitle::init()
{
    // 载入并播放背景音乐
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr)
    {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);
}

// 更新场景标题
void SceneTitle::update(float deltaTime)
{
    // TODO: 在这里添加更新场景标题的代码
    timer += deltaTime;
    if (timer > 1.0f)
    {
        timer -= 1.0f; // 限制计时器在2秒内循环
    }
}

void SceneTitle::render()
{
    // 渲染标题文字
    std::string titleText = "SDL2太空战机";
    game.renderTextCentered(titleText, 0.3, true);
    // 渲染普通文字

    if (timer < 0.5f) // 在计时器小于1秒时显示提示信息
    {
        std::string instructionsText = "按 J 键开始游戏";
        game.renderTextCentered(instructionsText, 0.7, false);
    }
}

// 清理资源
void SceneTitle::clean()
{

    if (bgm != nullptr)
    {
        // 停止音乐
        Mix_HaltMusic();
        // 释放资源
        Mix_FreeMusic(bgm);
    }
}

// 处理场景标题的事件
void SceneTitle::handleEvent(SDL_Event *event)
{
    // TODO: 处理事件

    if (event->type == SDL_KEYDOWN) // 按下键盘
    {
        if (event->key.keysym.scancode == SDL_SCANCODE_J) // 按下j键
        {
            auto sceneMain = new SceneMain(); // 切换到游戏场景
            game.changeScene(sceneMain);      // 切换场景
        }
    }
}