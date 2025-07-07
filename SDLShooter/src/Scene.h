//场景基类
#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>

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