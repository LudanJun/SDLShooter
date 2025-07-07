// 头文件保护
#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "SDL.h"

class Game
{
    public:// 公共接口    
        // 构造函数
        Game();
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
        void handleEvent(SDL_Event event);
        // 更新游戏状态
        void update();
        // 渲染游戏画面
        void render();
    private: // 私有成员
        bool isRunning = true;// 游戏是否运行
        Scene *currentScene = nullptr ;// 保存当前游戏场景  在 run函数中切换
        SDL_Window *window = nullptr;// 游戏窗口对象 后面常用
        SDL_Renderer *renderer = nullptr;// 渲染器对象 后面常用
    };

#endif