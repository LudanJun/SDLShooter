#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
/// 标题开始场景
class SceneTitle : public Scene
{
public:
    // virtual:代表虚函数，子类可以重写  =0 :代表纯虚函数，子类必须实现 override:代表重写父类虚函数
    virtual void init() override;

    virtual void update(float deltaTime) override;

    virtual void render() override;

    virtual void clean() override;

    virtual void handleEvent(SDL_Event *event) override;

private:
    Mix_Music *bgm; // 背景音乐

    float timer = 0.0f; // 计时器
};

#endif // SCENE_TITLE_H