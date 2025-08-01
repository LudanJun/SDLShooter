#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
/// @brief 物品类型枚举
/// C++11引入的枚举类 推荐加入关键字class
enum class ItemType
{
    Life,   ///< 生命值物品
    Shield, ///< 护盾物品
    Time    ///< 时间物品
    // Speed,  ///< 速度物品
    // Damage, ///< 攻击力物品
    // Score,  ///< 分数物品
    // Unknown ///< 未知物品类型
};

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
    int maxHealth = 3;       ///< 玩家飞机的最大生命值
    Uint32 coolDown = 200;   ///< 玩家飞机的冷却时间  200ms射击一次
    Uint32 lastShotTime = 0; ///< 玩家飞机上次射击的时间戳
};
// 敌机
struct Enemy
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 150;
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
    int speed = 600;
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

/// @brief 爆炸结构体
struct Explosion
{
    SDL_Texture *texture = nullptr; ///< 爆炸的纹理
    SDL_FPoint position = {0, 0};   ///< 爆炸的位置
    int width = 0;                  ///< 爆炸的宽度
    int height = 0;                 ///< 爆炸的高度
    int currentFrame = 0;           ///< 当前帧索引
    int totalFrame = 0;             ///< 总帧数
    Uint32 startTime = 0;           ///< 爆炸开始时间戳
    Uint32 FPS = 10;                ///< 每秒执行10帧
};

/// @brief  物品结构体
/// 物品可以是生命值、护盾、时间等
struct Item
{
    SDL_Texture *texture = nullptr; ///< 物品的纹理
    SDL_FPoint position = {0, 0};   ///< 物品的位置
    SDL_FPoint direction = {0, 0};  ///< 物品的方向
    int width = 0;                  ///< 物品的宽度
    int height = 0;                 ///< 物品的高度
    int speed = 200;                ///< 物品的速度
    int bounceCount = 3;            ///< 物品的反弹次数
    ItemType type = ItemType::Life; ///< 物品类型
    Uint32 spawnTime = 0;           ///< 物品生成时间戳
};

/// @brief 背景
struct Background
{
    SDL_Texture *texture = nullptr; ///< 背景的纹理
    SDL_FPoint position = {0, 0};   ///< 背景的位置
    float offset = 0;               ///< 背景的偏移量 对于y轴的偏移量用于滚动背景
    int width = 0;                  ///< 背景的宽度
    int height = 0;                 ///< 背景的高度
    int speed = 30;                 ///< 背景的滚动速度
};

#endif // OBJECT_H