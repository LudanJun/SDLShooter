// 主场景

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

// 需要导入场景基类头文件
#include "Scene.h"
// 在这里引用是不想再CMake中添加Object.h的路径了一般只添加.cpp
#include "Object.h"
#include <list>   // 引入 list容器
#include <random> // 引入随机数生成器
class Game;       // 前向声明Game类
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
    //
    void handleEvent(SDL_Event *event) override;

    // 场景清理函数
    void clean() override;

    // 键盘控制函数
    void keyboardControl(float deltaTime);

    // 玩家飞机射击函数
    // 这里的shootPlayer函数是用来处理玩家飞机的射击逻辑
    void shootPlayer();

    /// 更新玩家子弹
    void updatePlayerProjectiles(float deltaTime);

    /// 渲染玩家子弹
    void renderPlayerProjectiles();
    /// 渲染敌人子弹
    void renderEnemyProjectiles();

    /// 生成敌人
    void spawnEnemy();
    /// 更新敌人
    void updateEnemies(float deltaTime);
    /// 更新敌人发射的子弹
    void updateEnemyProjectiles(float deltaTime);
    /// 更新玩家
    void updatePlayer();

    /// 渲染敌人
    void renderEnemies();

    // 敌人飞机射击函数 传入敌机指针
    void shootEnemy(Enemy *enemy);

    // 获取玩家飞机的方向,也就是要发射的子弹方向
    // 传入敌机指针,去计算方向,因为玩家Player在私有成员变量里可以获取到不用传
    // 返回值是Point
    SDL_FPoint getDirection(Enemy *enemy);

    // 敌机爆炸函数
    void enemyExplode(Enemy *enemy);
    // 更新爆炸效果
    void updateExplosions();

    // 渲染爆炸效果
    void renderExplosions();

    // 掉落物品函数
    void dropItem(Enemy *enemy);

    // 更新掉落物品
    void updateItems(float deltaTime);

    // 玩家获取道具
    void playerGetItem(Item *item);

    // 渲染掉落物品
    void renderItems();

private:
    // 需要先在上面声明class Game;

    Game &game; // 游戏对象引用

    // 主场景里的初始化物体

    Player player;                              // 玩家飞机对象
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
};

#endif // SCENE_MAIN_H