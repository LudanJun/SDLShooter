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
    // 初始化 加载背景音乐
    bgm = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm == nullptr)
    {
        // 如果加载失败，打印错误信息
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load background music: %s", Mix_GetError());
    }
    // 播放 背景音乐
    Mix_PlayMusic(bgm, -1);

    // 读取音效资源
    sounds["player_shoot"] = Mix_LoadWAV("assets/sound/laser_shoot4.wav"); // 玩家射击音效
    sounds["enemy_shoot"] = Mix_LoadWAV("assets/sound/xs_laser.wav");      // 敌军射击音效
    sounds["player_explode"] = Mix_LoadWAV("assets/sound/explosion1.wav"); // 玩家爆炸音效
    sounds["enemy_explode"] = Mix_LoadWAV("assets/sound/explosion3.wav");  // 敌军爆炸音效
    sounds["hit"] = Mix_LoadWAV("assets/sound/eff11.wav");                 // 命中音效
    sounds["get_item"] = Mix_LoadWAV("assets/sound/eff5.wav");             // 获取道具音效
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

    // 初始化敌机子弹模版
    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    // 获取敌机子弹图片的宽和高
    SDL_QueryTexture(projectileEnemyTemplate.texture, nullptr, nullptr, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= 4;  // 敌机子弹宽度
    projectileEnemyTemplate.height /= 4; // 敌机子弹高度

    // 初始化爆炸模版
    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    // 获取爆炸图片的宽和高
    SDL_QueryTexture(explosionTemplate.texture, nullptr, nullptr, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height; // 爆炸总帧数
    explosionTemplate.width = explosionTemplate.height;                                // 爆炸宽度=高度

    // 初始化生命道具模版
    itemLifeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    // 获取生命道具图片的宽和高
    SDL_QueryTexture(itemLifeTemplate.texture, nullptr, nullptr, &itemLifeTemplate.width, &itemLifeTemplate.height);
    itemLifeTemplate.width /= 4;  // 生命道具宽度
    itemLifeTemplate.height /= 4; // 生命道具高度
}

// 2.渲染函数
void SceneMain::render()
{
    // 渲染玩家发射的子弹
    renderPlayerProjectiles();
    // 渲染敌人发射的子弹
    renderEnemyProjectiles();

    // 渲染玩家飞机
    if (isDead == false)
    {
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height,
        };
        // 渲染玩家飞机
        SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
    }

    // 渲染敌人飞机
    renderEnemies();
    // 渲染生命道具
    renderItems();
    // 渲染爆炸效果 让爆炸效果显示在最上面
    renderExplosions();
}
// 3. 更新
void SceneMain::update(float deltaTime)
{
    // TODO: 在这里添加更新逻辑
    // 更新场景中的物体、角色等
    keyboardControl(deltaTime);

    updatePlayerProjectiles(deltaTime); // 更新玩家发射的子弹
    updateEnemyProjectiles(deltaTime);  // 更新敌人发射的子弹
    spawnEnemy();                       // 生成敌人
    updateEnemies(deltaTime);           // 更新敌人
    updatePlayer();                     // 更新玩家飞机
    updateExplosions();                 // 更新爆炸效果
    updateItems(deltaTime);             // 更新物品道具
}

// 处理事件
void SceneMain::handleEvent(SDL_Event *event)
{

    // TODO: 处理事件
}

// 清理资源
void SceneMain::clean()
{
    for (auto sound : sounds)
    {
        // sound.second 是Mix_Chunk*类型的音效指针 map的value类型
        if (sound.second != nullptr) // 检查音效是否为空
        {
            Mix_FreeChunk(sound.second); // 释放音效资源
        }
    }
    sounds.clear(); // 清空音效列表

    // 循环清理玩家发射的子弹实例
    for (auto &projectile : projectilesPlayer)
    {
        if (projectile != nullptr) // 检查子弹实例是否为空
        {
            delete projectile; // 删除子弹实例
        }
    }
    projectilesPlayer.clear(); // 清空子弹列表

    // 循环清理敌人实例
    for (auto &enemy : enemies)
    {
        if (enemy != nullptr) // 检查敌人实例是否为空
        {
            delete enemy; // 删除敌人实例
        }
    }

    enemies.clear(); // 清空敌人列表

    // 清理敌人子弹
    for (auto &projectile : projectilesEnemy)
    {
        if (projectile != nullptr) // 检查敌人子弹实例是否为空
        {
            delete projectile; // 删除敌人子弹实例
        }
    }
    projectilesEnemy.clear(); // 清空敌人子弹列表

    // 清理爆炸效果
    for (auto &explosion : explosions)
    {
        if (explosion != nullptr) // 检查爆炸效果实例是否为空
        {
            delete explosion; // 删除爆炸效果实例
        }
    }
    explosions.clear(); // 清空爆炸效果列表

    // 清理物品道具
    for (auto &item : items)
    {
        if (item != nullptr) // 检查物品实例是否为空
        {
            delete item; // 删除物品实例
        }
    }
    items.clear(); // 清空物品列表

    // 清理玩家飞机
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture); // 销毁玩家飞机纹理
    }
    // 清理玩家子弹纹理
    if (projectilePlayerTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture); // 销毁子弹纹理
    }
    // 清理敌人飞机纹理
    if (enemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(enemyTemplate.texture); // 销毁敌人飞机纹理
    }

    // 清理敌人子弹纹理
    if (projectileEnemyTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(projectileEnemyTemplate.texture); // 销毁敌人子弹纹理
    }

    // 清理爆炸效果纹理
    if (explosionTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(explosionTemplate.texture); // 销毁爆炸效果纹理
    }

    // 清理生命物品纹理
    if (itemLifeTemplate.texture != nullptr)
    {
        SDL_DestroyTexture(itemLifeTemplate.texture); // 销毁生命物品纹理
    }

    // 清理音乐资源
    if (bgm != nullptr)
    {
        Mix_HaltMusic();    // 停止音乐播放
        Mix_FreeMusic(bgm); // 释放音乐资源
    }
}

// SceneMain类中的keyboardControl函数，用于处理键盘控制
void SceneMain::keyboardControl(float deltaTime)
{
    if (isDead)
    {
        // 如果玩家飞机已经死亡,不再处理键盘输入
        return;
    }
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
// 调用射击函数
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
    // channel -1表示使用任意可用的通道播放音效 0通道专门是玩家发射子弹
    // sounds是一个std::map<std::string, Mix_Chunk *>类型的容
    // 器，用于存储音效资源
    // player_shoot是音效资源的名称，表示玩家射击音效
    // loops -1表示无限循环播放音效 0表示只播放一次
    Mix_PlayChannel(0, sounds["player_shoot"], 0); // 播放玩家射击音效
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
            // SDL_Log("Player projectile removed at position: (%.2f, %.2f)", projectile->position.x, projectile->position.y);
        }
        else
        {
            bool isHit = false;
            // 没有超出屏幕遍历所有地方
            for (auto &enemy : enemies)
            {
                // 敌方飞机矩形区域
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height,
                };
                // 子弹矩形区域
                SDL_Rect projectileRect = {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height,
                };

                if (SDL_HasIntersection(&projectileRect, &enemyRect))
                {
                    // 子弹击中敌人
                    // 先减少敌方血量 要用到projectile->damage
                    // 减少敌人的生命值
                    enemy->currentHealth -= projectile->damage;
                    // 再删除子弹实例
                    delete projectile;
                    // 从子弹列表中删除子弹实例
                    // 删除之后,it会自加
                    it = projectilesPlayer.erase(it);
                    isHit = true; // 标记子弹击中敌人
                    // 播放命中音效
                    Mix_PlayChannel(-1, sounds["hit"], 0);
                    break; // 跳出循环
                }
            }
            // 如果子弹没有击中任何敌人，则继续遍历下一个子弹实例
            if (!isHit)
            {
                ++it;
            }
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

// 渲染敌人子弹
void SceneMain::renderEnemyProjectiles()
{
    // TODO: 实现渲染敌人子弹的功能
    for (auto projectile : projectilesEnemy)
    {
        // 计算敌人子弹在屏幕上的位置
        SDL_Rect projectileRect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height,
        };
        // 渲染敌人子弹
        // 计算翻转的角度
        // SDL_FPoint direction = {0, 0}; 这个空间二维向量可以转成角度的
        // 算出来的是弧度 需要*180/M_PI 转成角度
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90;

        // SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &projectileRect);
        // SDL_RenderCopyEx函数用于渲染带有旋转和翻转效果
        // angle: 这里的0.0表示不旋转
        // center: 这里的NULL表示不使用中心点旋转
        // flip: 这里的SDL_FLIP_NONE表示不进行翻转

        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, nullptr, &projectileRect,
                         angle, NULL, SDL_FLIP_NONE); // 渲染敌人子弹
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
    // 获取当前时间
    auto currentTime = SDL_GetTicks();
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
            // SDL_Log("Enemy removed at position: (%.2f, %.2f)", enemy->position.x, enemy->position.y);
        }
        else // 没有超出屏幕判断是否可以发射子弹了
        {
            /// 当前时间- enemy->lastShotTime > 冷却时间
            // 检查敌人是否可以发射子弹
            // 如果玩家没死亡继续发射子弹
            if (currentTime - enemy->lastShotTime > enemy->coolDown && isDead == false)
            {
                shootEnemy(enemy);                 // 调用敌人射击函数 添加音效
                enemy->lastShotTime = currentTime; // 更新敌人上次射击时间
            }
            // 检查敌人是否死亡
            if (enemy->currentHealth <= 0)
            {
                // 敌人死亡 添加音效 和爆炸特效 封装成一个函数
                //  delete enemy;           // 删除敌人实例
                enemyExplode(enemy);    // 调用敌人爆炸函数
                it = enemies.erase(it); // 从敌人列表中删除敌人实例
            }
            else
            {
                // 如果 敌方飞机没有死亡 不需要++it
                ++it; // 移动到下一个敌人实例
            }
        }
    }
}

// 更新敌人子弹
void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    // TODO: 实现更新玩家发射的子弹的逻辑
    // 从容器的起始点 开始遍历 终点是容器的结束点end
    // it是个指针
    auto margin = 32; // 子弹超出屏幕范围时移除
    for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();)
    {
        // 获取当前子弹实例
        auto projectile = *it;
        //
        projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;

        // 移动到下一个子弹实例
        // projectile->position.y > game.getWindowHeight() + margin 子弹超出屏幕的正下方
        // projectile->position.y < -margin 子弹超出屏幕的正上方
        // projectile->position.x < -margin  子弹超出屏幕的左边
        // projectile->position.x > game.getWindowWidth() + margin 子弹超出屏幕的右边
        if (projectile->position.y > game.getWindowHeight() + margin ||
            projectile->position.y < -margin ||
            projectile->position.x < -margin ||
            projectile->position.x > game.getWindowWidth() + margin) // 如果子弹超出屏幕上边界，则删除子弹实例
        {
            delete projectile;
            // it所在的位置删除后会返回一个迭代器指针,指向下一个元素
            it = projectilesEnemy.erase(it); // 从子弹列表中删除子弹实例
            // 判断是否真的删除掉 打印
            // SDL_Log("Player projectile removed at position: (%.2f, %.2f)", projectile->position.x, projectile->position.y);
        }
        else
        {
            // 因为玩家就一个,所以不需要进行遍历操作
            // 敌人子弹矩形
            SDL_Rect projectileRect = {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height,
            };
            // 玩家飞机矩形
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height,
            };
            // 检查敌人子弹是否与玩家飞机发生碰撞 如果玩家死亡不需要碰撞检测
            if (SDL_HasIntersection(&projectileRect, &playerRect) && !isDead)
            {
                // 减少玩家飞机的生命值
                player.currentHealth -= projectile->damage;
                // 如果有碰撞 就把子弹删除
                delete projectile;
                it = projectilesEnemy.erase(it);
                // 播放命中音效
                Mix_PlayChannel(-1, sounds["hit"], 0);
            }
            else
            {
                ++it;
            }
        }
    }
}

// 更新玩家
void SceneMain::updatePlayer()
{
    if (isDead)
    {
        return;
    }
    if (player.currentHealth <= 0)
    {
        // TODO: 实现玩家死亡后的逻辑
        // 例如: 游戏结束,切换场景等
        auto currentTime = SDL_GetTicks(); // 获取当前时间戳
        isDead = true;                     // 设置玩家死亡标志
        SDL_Log("Player is dead!");        // 打印玩家死亡信息
        // 创建爆炸特效实例
        auto explosion = new Explosion(explosionTemplate);
        // 设置爆炸位置
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
        explosion->startTime = currentTime;               // 设置爆炸特效的开始时间
        explosions.push_back(explosion);                  // 将爆炸特效添加到爆炸容器中
        Mix_PlayChannel(-1, sounds["player_explode"], 0); // 播放玩家爆炸音效

        // 停止背景音乐
        Mix_HaltMusic(); // 停止背景音乐
        return;          // 直接返回,不再更新玩家飞机
    }
    // 进行玩家飞机与敌机的碰撞检测
    // 玩家生命值-1 ,敌机直接爆炸
    for (auto enemy : enemies)
    {
        // 检查玩家飞机与敌人飞机是否发生碰撞
        SDL_Rect enemyRect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height,
        };
        SDL_Rect playerRect = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height,
        };
        if (SDL_HasIntersection(&playerRect, &enemyRect))
        {
            // 玩家生命值-1 ,敌机直接爆炸
            player.currentHealth -= 1;
            enemy->currentHealth = 0;
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
// 敌人飞机射击函数
void SceneMain::shootEnemy(Enemy *enemy)
{
    // 创建敌机子弹
    auto projectile = new ProjectileEnemy(projectileEnemyTemplate); // 创建新的敌人子弹实例
    // 设置敌人子弹的位置
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;   // 子弹位置居中
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2; // 子弹从敌人位置发射

    projectile->direction = getDirection(enemy); // 获取敌人子弹的方向
    projectilesEnemy.push_back(projectile);      // 将新创建的敌人子弹实例 添加到敌人子弹列表中
    // SDL_Log("Enemy projectile shot from position: (%.2f, %.2f)", projectile->position.x, projectile->position.y);
    // 播放敌人射击音效
    Mix_PlayChannel(-1, sounds["enemy_shoot"], 0); // 播放敌人射击音效
}

// 获取敌人子弹方向
SDL_FPoint SceneMain::getDirection(Enemy *enemy)
{
    // 玩家飞机的中心点 - 敌人飞机的中心点 得到的 x y 矢量就是子弹的方向
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    // 计算方向向量的长度(勾股定理)
    auto length = sqrt(x * x + y * y);
    x /= length; // 归一化 x 分量
    y /= length; // 归一化 y 分量

    return SDL_FPoint{x, y};
}

// 函数：敌人爆炸
// 参数：敌人指针
// 生成爆炸
void SceneMain::enemyExplode(Enemy *enemy)
{
    // TODO: 实现敌人爆炸效果
    // 添加敌机爆炸特效 需要知道敌机位置,从正中间爆炸
    auto currentTime = SDL_GetTicks();                 // 获取当前时间戳
    auto explosion = new Explosion(explosionTemplate); // 创建新的爆炸实例
    // 设置爆炸位置
    // 敌机X位置 + 敌机宽度/2 - 爆炸宽度/2
    // 敌机Y位置 + 敌机高度/2 - 爆炸高度/2
    // 这样可以确保爆炸效果在敌机的中心位置
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    // 把爆炸特效的时间设置为当前时间
    explosion->startTime = currentTime;
    // 将新创建的爆炸实例添加到爆炸容器中
    explosions.push_back(explosion);
    // 播放敌人爆炸音效
    Mix_PlayChannel(-1, sounds["enemy_explode"], 0);
    // 根据概率掉落物品道具 50%概率掉落物品
    // 使用随机数生成器生成一个0到1之间的随机数
    // 如果生成的随机数小于0.5，则掉落物品
    // disX是一个均匀分布的随机数生成器,范围在0.0到1.0之间
    // 通过调用disX(gen)生成一个随机数
    if (disX(gen) < 0.5f)
    {
        // 爆炸后掉落物品
        dropItem(enemy); // 调用掉落物品函数
    }

    // 删除敌人实例
    delete enemy;
}
/// @brief  更新爆炸效果
/// @param deltaTime  时间增量
void SceneMain::updateExplosions()
{
    // 考虑更新当前update结构体里的当前帧指向哪里,
    // 指向的位置会决定渲染的时候贴哪个图片
    // 如果指向的当前值已经超出了最后一帧,
    // 我们需要把整个爆炸结构体给删除掉
    // 需要迭代器的循环
    auto currentTime = SDL_GetTicks(); // 获取当前时间戳
    // 遍历爆炸容器中的所有爆炸特效
    for (auto it = explosions.begin(); it != explosions.end();)
    {
        auto explosion = *it;
        // 更新爆炸特效的当前帧
        // 首选需要计算得到当前时间,与爆炸结构体每一个它的开始时间的差值
        // 乘以爆炸特效的帧率除以1000,得到当前帧数
        // explosion->FPS是每秒的帧数,1000是毫秒转换成秒
        // 计算当前帧数
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
        // 如果当前帧数已经超过了最后一帧,那么就删除这个爆炸特效
        if (explosion->currentFrame >= explosion->totalFrame)
        {
            delete explosion;          // 删除爆炸特效实例
            it = explosions.erase(it); // 从爆炸容器中删除爆炸特效实例
        }
        else
        {
            ++it; // 移动到下一个爆炸特效实例
        }
    }
}
/// @brief  渲染爆炸效果
/// @note   渲染爆炸效果时,需要根据当前帧数来
///         计算爆炸特效的纹理区域,然后渲染到屏幕上
void SceneMain::renderExplosions()
{
    for (auto explosion : explosions)
    { // 首选确定渲染哪个方块
        // 计算爆炸特效的纹理区域
        // explosion->currentFrame * explosion->frameWidth 计算当前帧的纹理区域
        // explosion->currentFrame乘以每一帧的宽度,得到当前帧在纹理中的位置
        SDL_Rect srcRect = {explosion->currentFrame * explosion->width, 0, explosion->width, explosion->height};
        // 计算爆炸特效在屏幕上的位置
        SDL_Rect dstRect = {static_cast<int>(explosion->position.x),
                            static_cast<int>(explosion->position.y),
                            explosion->width,
                            explosion->height};
        // 渲染爆炸特效
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &srcRect, &dstRect);
    }
}
/// @brief 掉落物品
/// @note  当敌人被击败时,有一定概率掉落物品
///        物品可以是生命道具等
///        掉落物品的概率可以通过随机数来控制
/// @param enemy
void SceneMain::dropItem(Enemy *enemy)
{
    auto item = new Item(itemLifeTemplate);                                      // 创建新的物品实例
    item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;   // 设置物品位置
    item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2; // 设置物品位置
    // 不用对direction方向的x,y来进行随机,
    // 可以随机生成一个角度,
    // disX(gen) * 2 * M_PI得到一个0-360的角度
    float angle = disX(gen) * 2 * M_PI; // 随机生成一个角度
    // 计算物品的方向向量
    // 物品的方向向量可以通过角度的余弦和正弦来计算
    // 物品的方向向量是一个二维向量,
    // 其中x分量是角度的余弦值,y分量是角度的正弦值
    // 这样可以确保物品在掉落时沿着随机方向移动
    // 这样算出的x和y 已经是归一化的了
    item->direction.x = cos(angle); // 计算物品的x方向
    item->direction.y = sin(angle); // 计算物品的y方向
    items.push_back(item);          // 将新创建的物品实例添加到物品列表中
    // SDL_Log("Item dropped at position
}
/// brief 更新物品道具
void SceneMain::updateItems(float deltaTime)
{
    for (auto it = items.begin(); it != items.end();)
    {
        auto item = *it; // 获取当前物品实例

        // 更新物品位置
        item->position.x += item->direction.x * item->speed * deltaTime;
        item->position.y += item->direction.y * item->speed * deltaTime;

        // 超出屏幕范围之前处理反弹问题
        // 碰到左边界
        if (item->position.x < 0 && item->bounceCount > 0)
        {
            item->direction.x = -item->direction.x; // 反弹
            item->bounceCount--;                    // 减少反弹计数
        }
        // 碰到右边界
        if (item->position.x + item->width > game.getWindowWidth() && item->bounceCount > 0)
        {
            item->direction.x = -item->direction.x; // 反弹
            item->bounceCount--;                    // 减少反弹计数
        }
        // 碰到上边界
        if (item->position.y < 0 && item->bounceCount > 0 && item->bounceCount > 0)
        {
            item->direction.y = -item->direction.y; // 反弹
            item->bounceCount--;                    // 减少反弹计数
        }
        // 碰到下边界
        if (item->position.y + item->height > game.getWindowHeight() && item->bounceCount > 0)
        {
            item->direction.y = -item->direction.y; // 反弹
            item->bounceCount--;                    // 减少反弹计数
        }

        // 生命周期判断 如果超出屏幕范围则删除
        if (item->position.x + item->width < 0 ||
            item->position.x > game.getWindowWidth() ||
            item->position.y + item->height < 0 ||
            item->position.y > game.getWindowHeight())
        {
            delete item;          // 删除物品实例
            it = items.erase(it); // 从物品列表中删除物品实例
            // SDL_Log("Item removed at position: (%.2f, %.2f)",
        }

        // 玩家的碰撞检测 需要先创建两个SDL_Rect矩形
        else
        {
            // 创建物品矩形
            // SDL_Rect是SDL库中用于表示矩形的结构体
            // 它包含了矩形的x和y坐标,以及宽度和高度
            // 通过SDL_Rect可以方便地进行矩形的碰撞检测和
            SDL_Rect itemRect = {
                static_cast<int>(item->position.x),
                static_cast<int>(item->position.y),
                item->width,
                item->height,
            };
            /// 创建玩家飞机矩形
            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height,
            };
            // 检查物品与玩家飞机是否发生碰撞
            // 玩家死亡不能拾取物品
            if (SDL_HasIntersection(&itemRect, &playerRect) && isDead == false)
            {
                // 如果发生碰撞 玩具获取道具
                // 因为在迭代器里面, 所以只在获取道具函数里处理道具逻辑
                playerGetItem(item);  // 调用玩家获取道具函数
                delete item;          // 删除物品实例
                it = items.erase(it); // 从物品列表中删除物品实例
            }
            else
            {
                ++it; // 移动到下一个物品实例
            }
        }
    }
}

/// @brief 玩家获取道具
/// @param item 物品道具指针
/// @note  玩家获取道具后,可以增加玩家的生命值或其他属性
void SceneMain::playerGetItem(Item *item)
{
    // 检查物品类型是否为生命道具
    if (item->type == ItemType::Life)
    {
        player.currentHealth += 1; // 增加玩家的生命值
        if (player.currentHealth > player.maxHealth)
        {
            // 确保生命值不超过最大值
            player.currentHealth = player.maxHealth;
        }
        SDL_Log("Player gained a life! Current health: %d", player.currentHealth);
    }
    // 拾取物品音效
    Mix_PlayChannel(-1, sounds["get_item"], 0); // 播放拾取物品音效
    // TODO: 可以添加其他物品类型的处理逻辑
    //  例如: 增加分数,增加攻击力等
}
/// @brief  渲染物品道具
void SceneMain::renderItems()
{
    /// 遍历物品列表中的所有物品道具
    for (auto item : items)
    {
        // 计算物品在屏幕上的位置
        SDL_Rect itemRect = {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width,
            item->height,
        };
        // 渲染物品道具
        SDL_RenderCopy(game.getRenderer(), item->texture, nullptr, &itemRect);
    }
}
