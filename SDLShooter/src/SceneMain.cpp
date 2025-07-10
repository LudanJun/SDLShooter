#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <random>
// 构造函数

SceneMain ::SceneMain() : game(Game::getInstance())
{

    // 调用Game类的getInstance()方法获取Game类的实例
}
// 析构函数，用于释放ScreneMain对象所占用的资源
SceneMain ::~SceneMain()
{
}

// 1.初始化函数
void SceneMain::init()
{
    // 初始化随机数引擎
    std::random_device rd;                                    // 获取随机数种子
    gen = std::mt19937(rd());                                 // 使用随机数种子初始化随机数生成器
    disX = std::uniform_real_distribution<float>(0.0f, 1.0f); // 水平位置随机分布

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

    // 初始化子弹模版
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    // 获取子弹图片的宽和高
    SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= 4;  // 子弹宽度
    projectilePlayerTemplate.height /= 4; // 子弹高度

    // 初始化敌人飞机模版
    // 设置敌机纹理
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
    //  获取敌机图片的宽和高
    SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 4;  // 敌机宽度
    enemyTemplate.height /= 4; // 敌机高度
}
void SceneMain::update(float deltaTime)
{
    // TODO: 在这里添加更新逻辑
    // 更新场景中的物体、角色等
    keyboardControl(deltaTime);

    updatePlayerProjectiles(deltaTime); // 更新玩家发射的子弹

    spawnEnemy();             // 生成敌人
    updateEnemies(deltaTime); // 更新敌人
}

// 2.渲染函数
void SceneMain::render()
{
    // 渲染玩家发射的子弹
    renderPlayerProjectiles();
    // 渲染玩家飞机
    SDL_Rect playerRect = {
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height,
    };
    // 渲染玩家飞机
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);

    // 渲染敌人飞机
    renderEnemies();
}

// 处理事件
void SceneMain::handleEvent(SDL_Event *event)
{

    // TODO: 处理事件
}

// 清空屏幕
void SceneMain::clean()
{
    // 清理容器
    for (auto &projectile : projectilesPlayer)
    {
        if (projectile != nullptr) // 检查子弹实例是否为空
        {
            delete projectile; // 删除子弹实例
        }
    }

    projectilesPlayer.clear(); // 清空子弹列表

    for (auto &enemy : enemies)
    {
        if (enemy != nullptr) // 检查敌人实例是否为空
        {
            delete enemy; // 删除敌人实例
        }
    }

    enemies.clear(); // 清空敌人列表

    // 清理模版
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture); // 销毁玩家飞机纹理
    }
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture); // 销毁子弹纹理
    }
    if (enemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate.texture); // 销毁敌人飞机纹理
    }
}

// SceneMain类中的keyboardControl函数，用于处理键盘控制
void SceneMain::keyboardControl(float deltaTime)
{
    // TODO: 实现键盘控制逻辑
    // 返回Uint8 类型的指针，指向一个数组，该数组包含当前所有按键的状态，如果某个按键被按下，则对应的数组元素为1，否则为0
    // 使用auto 关键字，让编译器自动推导出变量的类型
    auto keyboardState = SDL_GetKeyboardState(NULL); // 获取键盘状态
    if (keyboardState[SDL_SCANCODE_W])
    {
        player.position.y -= deltaTime * player.speed; // 向上移动
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        player.position.y += deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        player.position.x -= deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        player.position.x += deltaTime * player.speed;
    }
    // 限制飞机的移动范围
    if (player.position.x < 0)
        player.position.x = 0;
    // 飞机移动到最右边
    if (player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0)
        player.position.y = 0;
    // 飞机移动到最左边
    if (player.position.y > game.getWindowHeight() - player.height)
    {
        player.position.y = game.getWindowHeight() - player.height;
    }

    /// 控制子弹发射
    if (keyboardState[SDL_SCANCODE_J])
    {
        auto currentTime = SDL_GetTicks(); // 获取当前时间戳
        /// 当前时间 - 玩家上次射击时间 >= 冷却时间
        if (currentTime - player.lastShotTime > player.coolDown) // 检查冷却时间
        {
            shootPlayer();                     // 调用射击函数
            player.lastShotTime = currentTime; // 更新上次射击时间
        }
    }
}

void SceneMain::shootPlayer()
{
    // 在这里实现发射子弹的逻辑
    auto projectile = new ProjectilePlayer(projectilePlayerTemplate); // 创建新的子弹实例

    // 设置子弹的位置
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2; // 子弹位置居中
    projectile->position.y = player.position.y;                                            // 子弹从玩家位置发射

    // 将子弹添加到子弹列表中
    // 这里的projectilesPlayer是一个std::list<ProjectilePlayer *>类型的
    // 列表，用于存储玩家飞机射出的子弹实例
    // 每当玩家飞机射击时，会创建一个新的子弹实例，并将其
    // 添加到子弹列表中
    // 在游戏更新和渲染过程中，可以遍历子弹列表，对每个
    // 子弹实例进行更新和渲染操作
    // 这样可以实现子弹的动态管理和渲染效果
    // 使用列表容器可以方便地添加、删除和遍历子弹实例
    // 这样可以提高代码的可读性和可维护性
    // 同时也可以提高游戏性能，避免频繁的内存分配和释放操作
    // 注意：在实际游戏中，可能还需要处理子弹的碰撞检测、生命周期等逻辑
    projectilesPlayer.push_back(projectile);
}

// 更新玩家发射的子弹
void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    // TODO: 实现更新玩家发射的子弹的逻辑
    // 从容器的起始点 开始遍历 终点是容器的结束点end
    // it是个指针
    int margin = 32; // 子弹超出屏幕范围时移除
    for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
    {
        auto projectile = *it; // 获取当前子弹实例
        // 子弹朝上 -=
        projectile->position.y -= projectile->speed * deltaTime; // 更新子弹位置

        // 移动到下一个子弹实例
        if (projectile->position.y + margin < 0) // 如果子弹超出屏幕上边界，则删除子弹实例
        {
            delete projectile;
            // it所在的位置删除后会返回一个迭代器指针,指向下一个元素
            it = projectilesPlayer.erase(it); // 从子弹列表中删除子弹实例
            // 判断是否真的删除掉 打印
            SDL_Log("Player projectile removed at position: (%.2f, %.2f)", projectile->position.x, projectile->position.y);
        }
        else
        {
            ++it;
        }
    }
}

// 渲染玩家发射的子弹
void SceneMain::renderPlayerProjectiles()
{

    for (auto projectile : projectilesPlayer)
    {
        // 计算子弹在屏幕上的位置
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height,
        };
        // 渲染子弹
        SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &projectileRect);
    }
}

// 在SceneMain类中定义一个名为spawnEnemy的函数
void SceneMain::spawnEnemy()
{
    // TODO: 在这里添加代码，用于生成敌人
    if (disX(gen) > 1 / 60.0f)
    {
        return; // 如果生成的随机数大于1/60秒，则不生成敌人
    }
    // 创建一个新的敌人实例
    // 使用enemyTemplate作为模板来创建新的敌人实例
    // 这里的enemyTemplate是一个Enemy类型的对象，包含了敌人的纹理、位置、尺寸和速度等属性
    // 通过使用enemyTemplate作为模板，可以快速创建新的敌人实例
    Enemy *enemy = new Enemy(enemyTemplate);
    enemy->position.x = disX(gen) * (game.getWindowWidth() - enemy->width); // 随机生成敌人的水平位置
    enemy->position.y = -enemy->height;                                     // 设置敌人的初始位置在屏幕上方
    enemies.push_back(enemy);                                               // 将新创建的敌人实例添加到敌人列表中
}

// 更新敌人
void SceneMain::updateEnemies(float deltaTime)
{
    // TODO: 在这里添加更新敌人的代码
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto enemy = *it; // 获取当前敌人实例
        // 更新敌人的位置
        enemy->position.y += enemy->speed * deltaTime; // 敌人向下移动

        // 检查敌人是否超出屏幕范围
        if (enemy->position.y > game.getWindowHeight())
        {
            delete enemy;           // 删除敌人实例
            it = enemies.erase(it); // 从敌人列表中删除敌人实例
            SDL_Log("Enemy removed at position: (%.2f, %.2f)", enemy->position.x, enemy->position.y);
        }
        else
        {
            ++it; // 移动到下一个敌人实例
        }
    }
}

// 渲染敌人
void SceneMain::renderEnemies()
{
    // TODO: 实现渲染敌人的代码
    for (auto enemy : enemies)
    {
        // 计算敌人在屏幕上的位置
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height,
        };
        // 渲染敌人
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}
