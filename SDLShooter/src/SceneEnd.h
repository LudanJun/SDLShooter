#ifndef SCENE_END_H // 头文件保护
#define SCENE_END_H
#include "Scene.h"
// #include <SDL.h>
// #include <SDL_image.h>
// #include <SDL_ttf.h>
// #include <SDL_mixer.h>
class SceneEnd : public Scene
{

    // virtual:代表虚函数，子类可以重写  =0 :代表纯虚函数，子类必须实现 override:代表重写
    virtual void init() override;

    virtual void update(float deltaTime) override;

    virtual void render() override;

    virtual void clean() override;

    virtual void handleEvent(SDL_Event *event) override;
};
#endif // SCENE_END_H