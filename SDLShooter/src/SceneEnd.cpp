#include "SceneEnd.h"
#include "SceneMain.h"
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
    blinkTimer -= deltaTime;
    if (blinkTimer <= 0)
    {
        blinkTimer += 1.0f;
    }
}

void SceneEnd::render()
{
    if (isTyping)

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
    if (isTyping)
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
                if (name == "")
                {
                    name = "无名氏";

                } // 输入的名字与得分插入到排行榜中
                game.insertLeaderBoard(game.getFinalScore(), name);
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) // 如果用户按下了退格键
            {
                // name.pop_back(); // 从名字字符串中删除最后一个字符
                removeLastUTF8Char(name);
            }
        }
    }
    else
    {
        if (event->type == SDL_KEYDOWN) // 如果用户按下了键盘
        {
            if (event->key.keysym.scancode == SDL_SCANCODE_J) // 如果用户按下了回车键
            {

                // 引入主场景后 调用切换场景函数
                auto sceneMain = new SceneMain();

                game.changeScene(sceneMain);
            }
        }
    }
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "你的得分是: " + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instructionsText = "请输入你的名字, 按回车键确认:";
    game.renderTextCentered(scoreText, 0.1, false);
    game.renderTextCentered(gameOver, 0.4, true);
    game.renderTextCentered(instructionsText, 0.6, false);

    if (name != "") // 如果用户已经输入了名字 为空会出问题需要判断
    {
        // std::string nameText = "你的名字是: " + name;
        SDL_Point p = game.renderTextCentered(name, 0.8, false);
        if (blinkTimer < 0.5)
        {
            game.renderTextPos("_", p.x, p.y);
        }
    }
    else
    {
        if (blinkTimer < 0.5)
        {
            game.renderTextCentered("_", 0.8, false);
        }
    }
}

void SceneEnd::renderPhase2()
{

    game.renderTextCentered("得分榜", 0.1, true); // 渲染得分榜
    auto posY = 0.2 * game.getWindowHeight();     // 渲染位置
    auto i = 1;                                   // 计数器
    for (auto item : game.getLeaderBoard())       // 遍历得分榜
    {

        std::string name = std::to_string(i) + "." + item.second; // 获取名字
        std::string score = std::to_string(item.first);           // 获取分数
        game.renderTextPos(name, 100, posY);                      // 渲染名字
        game.renderTextPos(score, 100, posY, false);              // 渲染分数
        posY += 50;
        i++; // 更新位置
    }
    if (blinkTimer < 0.5)
    {
        game.renderTextCentered("按 J 键重新开始游戏", 0.85, false);
    }
}

// 删除最后一个UTF-8字符
void SceneEnd::removeLastUTF8Char(std::string &str)
{
    if (str.empty())
        return;
    // TODO: 实现删除最后一个UTF-8字符的功能
    auto lastchar = str.back();
    // 如果最高位为1 中文字符的后续字节
    if ((lastchar & 0b10000000) == 0b10000000)
    { // 删除一个字符
        str.pop_back();
        // 判断是否是中文字符的第一个字节
        while ((str.back() & 0b11000000) != 0b11000000)
        {
            str.pop_back();
        }
    }
    str.pop_back(); // 删除最后一个字符
}
