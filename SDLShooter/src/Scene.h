//场景基类
#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>

/// @brief  场景基类
/// 继承自Scene类的子类必须实现init、update、render、clean和handleEvent方法
/// 这些方法分别用于初始化场景、更新场景状态、渲染场景、清理场景资源和处理事件
/// 通过这种方式，Scene类提供了一个统一的接口，使得不同的场景可以被管理和切换
/// 例如，在游戏中可以有多个场景，如主菜单、游戏进行中、游戏结束等
/// 每个场景都可以继承自Scene类，并实现这些方法
/// 这样，游戏可以通过调用这些方法来管理不同的场景，实现场景的切换和管理
/// 这种设计模式使得代码更加模块化和可维护
/// 通过使用虚函数和纯虚函数，Scene类提供了一个抽象基类
/// 使得子类可以根据具体需求实现不同的场景逻辑
/// 这种设计模式在游戏开发中非常常见，可以提高代码的可读性
/// 和可扩展性
class Scene{
    public:
        // 构造函数，默认初始化
        Scene() = default;
        // 虚析构函数，用于删除Scene对象
        virtual ~Scene() = default;

        // virtual:代表虚函数，子类可以重写  =0 :代表纯虚函数，子类必须实现
        virtual void init() = 0;   

        virtual void update() = 0;
  
        virtual void render() = 0;

        virtual void clean() = 0;

        virtual void handleEvent(SDL_Event* event) = 0;
};

#endif