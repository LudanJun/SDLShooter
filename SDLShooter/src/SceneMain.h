// 主场景

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

// 需要导入场景基类头文件
#include "Scene.h"
// 在这里引用是不想再CMake中添加Object.h的路径了一般只添加.cpp
#include "Object.h"
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
    void update() override;
    // 场景渲染函数
    void render() override;
    //
    void handleEvent(SDL_Event *event) override;
    // 场景清理函数
    void clean() override;

private:
    // 主场景里的初始化物体
    Player *player; // 玩具飞机对象
};

#endif // SCENE_MAIN_H