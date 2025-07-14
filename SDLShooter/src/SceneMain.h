// 主场景

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

// 需要导入场景基类头文件
#include "Scene.h"
// 在这里引用是不想再CMake中添加Object.h的路径了一般只添加.cpp
#include "Object.h"
#include <list>        // 引入 list容器
#include <random>      // 引入随机数生成器
#include <map>         // 引入 map容器
#include <SDL.h>       // 引入SDL库
#include <SDL_mixer.h> // 引入SDL_mixer库
#include <SDL_ttf.h>   // 引入SDL_ttf库

class Game; // 前向声明Game类
// 定义一个名为ScreneMain的类，继承自Scene类
class SceneMain : public Scene
{
public:
    // 构造函数
    SceneMain();
    // 析构函数
    ~SceneMain();

    // 场景初始化函数
    void init() override;
    // 场景更新函数
    void update(float deltaTime) override;
    // 场景渲染函数
    void render() override;
    void handleEvent(SDL_Event *event) override;
    // 场景清理函数
    void clean() override;

private:
    // 需要先在上面声明class Game;

    // Game &game; // 游戏对象引用

    // 主场景里的初始化物体

    Player player;         // 玩家飞机对象
    Mix_Music *bgm;        // 背景音乐对象
    SDL_Texture *uiHealth; // UI血条图标纹理
    TTF_Font *scoreFont;   // 字体对象
    int score = 0;         // 得分

    bool isDead = false;                        // 玩家飞机是否死亡
    std::mt19937 gen;                           // 随机数生成器
    std::uniform_real_distribution<float> disX; // 水平位置随机分布

    // 创建每个物体的模版
    Enemy enemyTemplate;                       // 创建每个敌人飞机模板对象
    ProjectilePlayer projectilePlayerTemplate; // 子弹模板对象 提前初始化好
    ProjectileEnemy projectileEnemyTemplate;   // 敌人飞机子弹模板对象
    Explosion explosionTemplate;               // 爆炸模板对象
    Item itemLifeTemplate;                     // 生命道具模板对象

    // 创建每个物体的容器
    std::list<Enemy *> enemies;                      // 敌人飞机列表 敌人类型的指针列表
    std::list<ProjectilePlayer *> projectilesPlayer; // 玩家子弹容器
    std::list<ProjectileEnemy *> projectilesEnemy;   // 敌人子弹容器
    std::list<Explosion *> explosions;               // 爆炸容器
    std::list<Item *> items;                         // 道具物品容器 所有物品都可以放在这个容器里
    std::map<std::string, Mix_Chunk *> sounds;       // 存储音效容器

    ///--- 渲染 ---///
    void renderPlayerProjectiles(); // 渲染玩家子弹

    void renderEnemies(); // 渲染敌人

    void renderEnemyProjectiles(); // 渲染敌人子弹

    void renderItems(); // 渲染掉落物品

    void renderUIHealth(); // 渲染UI血条

    void renderExplosions(); // 渲染爆炸效果

    ///--- 更新 ---///
    void updateEnemies(float deltaTime); // 更新敌人

    void updateEnemyProjectiles(float deltaTime); // 更新敌人发射的子弹

    void updatePlayer(); // 更新玩家

    void updatePlayerProjectiles(float deltaTime); // 更新玩家子弹

    void updateExplosions(); // 更新爆炸效果

    void updateItems(float deltaTime); // 更新掉落物品

    void keyboardControl(float deltaTime); // 键盘控制函数

    ///--- 其他 ---///
    void shootPlayer(); // 玩家飞机射击函数

    void spawnEnemy(); /// 生成敌人

    void shootEnemy(Enemy *enemy); // 敌人飞机射击函数 传入敌机指针

    SDL_FPoint getDirection(Enemy *enemy); // 获取玩家飞机的方向,也就是要发射的子弹方向

    void enemyExplode(Enemy *enemy); // 敌机爆炸函数

    void dropItem(Enemy *enemy); // 掉落物品函数

    void playerGetItem(Item *item); // 玩家获取道具
};

#endif // SCENE_MAIN_H