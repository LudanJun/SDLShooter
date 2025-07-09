#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>

/// @brief 游戏对象基类
struct Player
{
    ///< 游戏对象的纹理 玩具飞机的材质 初始化为nullptr空指针
    SDL_Texture *texture = nullptr;

    ///< 游戏对象的位置 玩具飞机的坐标 初始化为(0, 0)坐标
    SDL_FPoint position = {0, 0};

    ///< 游戏对象的尺寸 玩具飞机的尺寸 初始化为0, 0尺寸
    int width = 0;
    int height = 0;
    int speed = 200;// 玩具飞机的速度 初始化为0
};

#endif // OBJECT_H