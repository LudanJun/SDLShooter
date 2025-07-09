#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>

// 构造函数

SceneMain ::SceneMain() : game(Game::getInstance())
{

    // 调用Game类的getInstance()方法获取Game类的实例
}
// 析构函数，用于释放ScreneMain对象所占用的资源
SceneMain ::~SceneMain()
{
}
void SceneMain::update(float deltaTime)
{
    // TODO: 在这里添加更新逻辑

    // 更新场景中的物体、角色等

    keyboardControl(deltaTime);
}
// 1.初始化函数
void SceneMain::init()
{
    // TODO: 添加初始化代码
    // Game::getInstance() 获取game单例实例
    // getRenderer() 获取渲染器
    // 把Game::getInstance()当成成员变量使用
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");

    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", IMG_GetError());
    }

    // 跟着图片大小来显示玩家飞机大小
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width = player.width / 4;                                  // 玩具飞机的宽度除以6
    player.height = player.height / 4;                                // 玩具飞机的高度除以6
    player.position.x = game.getWindowWidth() / 2 - player.width / 2; // 居中显示
    player.position.y = game.getWindowHeight() - player.height - 20;  // 底
}
// 2.渲染函数
void SceneMain::render()
{
    // TODO: 添加渲染代码
    // 玩家飞机的矩形
    SDL_Rect playerRect = {
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height,
    };
    // 渲染玩家飞机
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
}

// 处理事件
void SceneMain::handleEvent(SDL_Event *event)
{

    // TODO: 处理事件
}

// 清空屏幕
void SceneMain::clean()
{
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture); // 销毁玩家飞机纹理
    }

    // 清空屏幕的具体实现
}

// SceneMain类中的keyboardControl函数，用于处理键盘控制
void SceneMain::keyboardControl(float deltaTime)
{
    // TODO: 实现键盘控制逻辑
    // 返回Uint8 类型的指针，指向一个数组，该数组包含当前所有按键的状态，如果某个按键被按下，则对应的数组元素为1，否则为0
    // 使用auto 关键字，让编译器自动推导出变量的类型
  auto keyboardState = SDL_GetKeyboardState(NULL); // 获取键盘状态
    if (keyboardState[SDL_SCANCODE_W]){
        player.position.y -= deltaTime* player.speed; // 向上移动
    }
    if (keyboardState[SDL_SCANCODE_S]){
        player.position.y += deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_A]){
        player.position.x -= deltaTime* player.speed;
    }
    if (keyboardState[SDL_SCANCODE_D]){
        player.position.x += deltaTime* player.speed;
    }
    //限制飞机的移动范围
    if (player.position.x < 0) player.position.x = 0;
    //飞机移动到最右边
    if (player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0) player.position.y = 0;
    //飞机移动到最左边
    if (player.position.y > game.getWindowHeight() - player.height){
        player.position.y = game.getWindowHeight() - player.height;
    }

    /// 控制子弹发射
    if (keyboardState[SDL_SCANCODE_J]){
        auto currentTime = SDL_GetTicks(); // 获取当前时间戳
        /// 当前时间 - 玩家上次射击时间 >= 冷却时间
        if (currentTime - player.lastShotTime >= player.coolDown) // 检查冷却时间
        {
            shootPlayer(); // 调用射击函数
            player.lastShotTime = currentTime; // 更新上次射击时间
        }
    }
 }

 void SceneMain::shootPlayer()
 {
 }
