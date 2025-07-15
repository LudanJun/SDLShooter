#ifndef SCENE_END_H // 头文件保护
#define SCENE_END_H
#include "Scene.h"
#include <string>
// #include <SDL.h>
// #include <SDL_image.h>
// #include <SDL_ttf.h>
// #include <SDL_mixer.h>
class SceneEnd : public Scene
{
public:
    // virtual:代表虚函数，子类可以重写  =0 :代表纯虚函数，子类必须实现 override:代表重写
    virtual void init() override;

    virtual void update(float deltaTime) override;

    virtual void render() override;

    virtual void clean() override;

    virtual void handleEvent(SDL_Event *event) override;

private:
    bool isTyping = true;    // true:正在输入，false:输入完成
    std::string name = "";   // 输入的名字
    float blinkTimer = 1.0f; // 闪烁时间 每秒闪烁1次
    void renderPhase1();
    void renderPhase2();

    void removeLastUTF8Char(std::string &str); // 删除最后一个UTF8字符  std::string &str 引用
};
#endif // SCENE_END_H