// 头文件保护
#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "SDL.h"

/// @brief  游戏类
/// 负责游戏的初始化、运行、事件处理、更新和渲染等功能
/// 通过该类可以创建游戏窗口、渲染游戏场景、处理用户输入等
/// 该类包含了游戏的主循环，负责管理游戏的状态和场景切换
/// 通过该类可以实现游戏的基本功能，如开始游戏、暂停游戏、结束游戏等
/// 该类还可以扩展其他功能，如音频播放、图像加载等
/// 通过使用SDL库，该类可以实现跨平台的游戏开发
/// 该类的设计使得游戏逻辑和渲染逻辑分离，便于维护和扩展
/// 通过继承Scene类，可以实现不同的游戏场景管理
/// 该类提供了一个统一的接口，使得不同的游戏场景可以被管理和切换
/// 例如，在游戏中可以有多个场景，如主菜单、游戏进行中、游戏结束等
/// 每个场景都可以继承自Scene类，并实现init、update、render、clean和handleEvent方法
/// 这样，游戏可以通过调用这些方法来管理不同的场景，实现场景的切换和管理
/// 这种设计模式使得代码更加模块化和可维护
class Game
{
    // 公共接口
public:
    // 单例模式，获取游戏实例 推荐引用 Game &getInstance()
    static Game &getInstance(){
        static Game instance; // 静态实例
        return instance; // 返回实例引用
    };
    // 析构函数
    ~Game();
    // 游戏运行
    // run里面进行游戏主循环
    void run();
    // 游戏初始化
    void init();
    // 游戏清除
    void clean();
    // 场景切换
    void changeScene(Scene *scene);
    // 处理事件
    void handleEvent(SDL_Event *event);
    // 更新游戏状态
    void update();
    // 渲染游戏画面
    void render();

private:
    // 构造函数
    Game();
    //删除拷贝与构造函数
    Game(const Game&) = delete ; // 禁止拷贝构造函数
    Game& operator=(const Game&) = delete; // 禁止赋值构造函数
    // 私有成员
    bool isRunning = true;            // 游戏是否运行
    Scene *currentScene = nullptr;    // 保存当前游戏场景  在 run函数中切换
    SDL_Window *window = nullptr;     // 游戏窗口对象 后面常用
    SDL_Renderer *renderer = nullptr; // 渲染器对象 后面常用
    // 窗口大小以后也可能用到 把保存成成员变量
    int windowWidth = 600;  // 窗口宽度
    int windowHeight = 800; // 窗口高度
};

#endif