#include "SceneTitle.h"
#include "Game.h"
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

void SceneTitle::update(float deltaTime)
{
}

void SceneTitle::render()
{
    // 渲染标题文字
    std::string titleText = "SDL2太空战机";
    game.renderTextCentered(titleText, 0.3, true);
    // 渲染普通文字
    std::string instructionsText = "按 J 键开始游戏";
    game.renderTextCentered(instructionsText, 0.7, false);
}

void SceneTitle::clean()
{
}

void SceneTitle::handleEvent(SDL_Event *event)
{
}
