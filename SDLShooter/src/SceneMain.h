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
    /// 渲染敌人
    void renderEnemies();

    // 敌人飞机射击函数 传入敌机指针
    void shootEnemy(Enemy *enemy);

    // 获取玩家飞机的方向,也就是要发射的子弹方向
    // 传入敌机指针,去计算方向,因为玩家Player在私有成员变量里可以获取到不用传
    // 返回值是Point
    SDL_FPoint getDirection(Enemy *enemy);

    // 敌机爆炸函数
    void enemyExplosion(Enemy *enemy);

private:
    // 需要先在上面声明class Game;
    // 游戏对象引用
    Game &game;

    // 主场景里的初始化物体
    // 玩家飞机对象
    Player player;

    std::mt19937 gen;                           // 随机数生成器
    std::uniform_real_distribution<float> disX; // 水平位置随机分布

    // 创建每个敌人飞机模板对象
    Enemy enemyTemplate;
    // 子弹模板对象 提前初始化好
    // 这里的projectilePlayerTemplate是一个子弹模板对象，用于创建子弹实例
    // 通过提前初始化好子弹模板对象，可以在游戏中快速创建子弹实例
    // 这样可以避免每次射击都重新加载纹理，提高性能
    // 这里的子弹模板对象是一个结构体，包含了子弹的纹理、位置、尺寸和速度等属性
    ProjectilePlayer projectilePlayerTemplate;
    // 敌人飞机子弹模板对象
    ProjectileEnemy projectileEnemyTemplate;

    // 敌人飞机列表
    std::list<Enemy *> enemies; // 敌人类型的指针列表
    // 玩家子弹容器
    // 子弹列表
    // 这里的projectilesPlayer是一个子弹列表，用于存储玩家飞机射出的子弹实例
    // 通过使用列表容器，可以方便地管理和遍历子弹实例
    // 这样可以实现子弹的创建、更新和渲染等功能
    // 每当玩家飞机射击时，会创建一个新的子弹实例
    // 并将其添加到子弹列表中
    // 在游戏更新和渲染过程中，可以遍历子弹列表，对每个子弹实例进行更新和渲染操作
    // 这样可以实现子弹的动态管理和渲染效果
    std::list<ProjectilePlayer *> projectilesPlayer;
    // 敌人子弹容器
    std::list<ProjectileEnemy *> projectilesEnemy;
};

#endif // SCENE_MAIN_H