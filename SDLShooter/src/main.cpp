#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Game.h"
int main(int, char **)
{ // 创建游戏对象
    // 单例模式获取游戏实例
    Game &game = Game::getInstance();
    game.init(); // 初始化游戏
    game.run();  // 运行游戏

    return 0;
}