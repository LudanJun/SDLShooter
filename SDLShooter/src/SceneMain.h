//主场景

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

// 需要导入场景基类头文件
#include "Scene.h"

// 定义一个名为ScreneMain的类，继承自Scene类
class ScreneMain : public Scene{
public:
    // 构造函数
    ScreneMain();
    // 析构函数
    ~ScreneMain();

    // 场景初始化函数
    void init() override;
    // 场景更新函数
    void update() override;
    // 场景渲染函数 
    void render() override;
    //
    void handleEvent(SDL_Event* event) override;
    // 场景清理函数
    void clean() override;
    
private:

};

#endif // SCENE_MAIN_H