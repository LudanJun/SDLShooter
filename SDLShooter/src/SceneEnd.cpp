#include "SceneEnd.h"
#include "Game.h"
#include <string>
void SceneEnd::init()
{
    if (SDL_IsTextInputActive()) // 如果文本输入处于活动状态
    {
        SDL_StartTextInput(); // 启动文本输入

        if (!SDL_IsTextInputActive()) // 如果启动失败
        {
            SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to start text input: %s", SDL_GetError());
        }
    }
}

void SceneEnd::update(float deltaTime)
{
}

void SceneEnd::render()
{
    if (isTyping == true)

    {
        renderPhase1(); // 渲染阶段1
    }
    else
    {
        renderPhase2(); // 渲染阶段2
    }
}

void SceneEnd::clean()
{
}

void SceneEnd::handleEvent(SDL_Event *event)
{
    if (isTyping == true)
    {
        if (event->type == SDL_TEXTINPUT) // 如果用户输入了文本
        {
            name += event->text.text; // 将输入的文本添加到名字字符串中
        }
        if (event->type == SDL_KEYDOWN) // 如果用户按下了键盘
        {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) // 如果用户按下了回车键
            {
                isTyping = false;    // 停止输入名字
                SDL_StopTextInput(); // 停止文本输入
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) // 如果用户按下了退格键
            {
                name.pop_back(); // 从名字字符串中删除最后一个字符
            }
        }
    }
    else
    {
    }
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "你的得分是: " + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instructionsText = "请输入你的名字, 按回车键确认";
    game.renderTextCentered(scoreText, 0.1, false);
    game.renderTextCentered(gameOver, 0.4, true);
    game.renderTextCentered(instructionsText, 0.6, false);

    if (name != "") // 如果用户已经输入了名字 为空会出问题需要判断
    {
        std::string nameText = "你的名字是: " + name;
        game.renderTextCentered(nameText, 0.8, false);
    }
}

void SceneEnd::renderPhase2()
{
}
