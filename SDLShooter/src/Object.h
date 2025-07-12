#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>

/// @brief 游戏对象基类
struct Player
{
    ///< 游戏对象的纹理 玩家飞机的材质 初始化为nullptr空指针
    SDL_Texture *texture = nullptr;

    ///< 游戏对象的位置 玩家飞机的坐标 初始化为(0, 0)坐标
    SDL_FPoint position = {0, 0};

    ///< 游戏对象的尺寸 玩家飞机的尺寸 初始化为0, 0尺寸
    int width = 0;
    int height = 0;
    int speed = 300;         // 玩家飞机的速度 初始化为0
    int currentHealth = 3;   ///< 玩家飞机的当前生命值
    Uint32 coolDown = 200;   ///< 玩家飞机的冷却时间  500ms射击一次
    Uint32 lastShotTime = 0; ///< 玩家飞机上次射击的时间戳
};
// 敌机
struct Enemy
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int currentHealth = 2;   ///< 敌机的当前生命值
    Uint32 coolDown = 2000;  ///< 敌机飞机的冷却时间  1000ms射击一次
    Uint32 lastShotTime = 0; ///< 敌机飞机上次射击的时间戳
};

// 子弹类
struct ProjectilePlayer
{
    ///< 子弹的纹理
    SDL_Texture *texture = nullptr;

    ///< 子弹的位置
    SDL_FPoint position = {0, 0};

    ///< 子弹的尺寸
    int width = 0;
    int height = 0;
    ///< 子弹的速度
    int speed = 400;
    int damage = 1; ///< 子弹的伤害值 攻击力
};

// 敌机子弹
struct ProjectileEnemy
{
    ///< 子弹的纹理
    SDL_Texture *texture = nullptr;

    ///< 子弹的位置
    SDL_FPoint position = {0, 0};

    /// 子弹运行方向
    SDL_FPoint direction = {0, 0}; // 默认向下

    ///< 子弹的尺寸
    int width = 0;
    int height = 0;
    ///< 子弹的速度
    int speed = 400;
    int damage = 1; ///< 子弹的伤害值 攻击力
};

#endif // OBJECT_H