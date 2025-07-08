#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Game.h"
int main(int, char **)
{

    Game game;   // 创建游戏对象
    game.init(); // 初始化游戏
    game.run();  // 运行游戏

    return 0;
}