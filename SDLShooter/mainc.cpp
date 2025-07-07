
// #include <iostream>
// #include <SDL.h>
// #include <SDL_image.h>
// #include <SDL_mixer.h>
// #include <SDL_ttf.h>
// int main(int , char**) {
  
//     std::cout << "Hello, World!" << std::endl;
    
//     /// SDL初始化
//     if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
//         std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
//         return 1;
//     }
//     //创建窗口
//     //SDL_WINDOW_SHOWN：显示窗口
//     SDL_Window *win = SDL_CreateWindow("Hello World", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
   
//     // 创建渲染器
//     //参数: window -显示渲染的窗口。 
//     //index -渲染驱动程序的索引初始化，或-1初始化第一个支持请求的标志。
//     // flags - 0或一个或多个SDL_渲染或一起。
//     // window：窗口 
//     // -1：默认渲染器  
//     // SDL_RENDERER_ACCELERATED：使用硬件加速
//     SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
//     /// SDL_image初始化
//     if((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) )!= (IMG_INIT_PNG | IMG_INIT_JPG)) 
//     {
//         std::cerr << "SDL_image_Init Error: " << IMG_GetError() << std::endl;
//         return 1;
//     }
//     /// 加载图片
//     SDL_Texture *texture = IMG_LoadTexture(renderer, "assets/image/bg.png");
    

//     /// SDL_Mixer初始化
//     /// int frequency, 音频频率
//     /// Uint16 format,  音频格式 
//     /// int channels,  音频通道数
//     /// int chunksize , 音频块大小
//     if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
//         std::cerr << "SDL_Mixer_Init Error: " << Mix_GetError() << std::endl;
//         return 1;
//     }

//     /// 读取音乐
//     Mix_Music *music = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
//     /// 检查音乐是否加载成功
//     if(music == NULL) {
//         std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
//         return 1;
//     }
//     /// 播放音乐
//     /// Mix_Music *music,  音乐
//     /// int loops 循环次数
//     Mix_PlayMusic(music, -1);


//     /// SDL_TTF初始化
//     if(TTF_Init() < 0) {
//         std::cerr << "SDL_TTF_Init Error: " << TTF_GetError() << std::endl;
//         return 1;
//     }
    
//     // 加载字体
//     // 字体大小初始化后就限制不能修改了
//     TTF_Font *font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 16);
    
//     // 检查字体是否加载成功
//     if(font == NULL) {
//         std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
//         return 1;
//     }
//     //创建文本纹理
//     //设置文本颜色
//     SDL_Color color = {255, 0, 0, 255};
//     // TTF_RenderUTF8_Solid 可以显示中文
//     // 字体，文本，颜色
//     SDL_Surface *surface = TTF_RenderUTF8_Solid(font, "Hello, SDL! 游戏图像框架", color);
//     // 创建文本纹理
//     SDL_Texture *texTextture = SDL_CreateTextureFromSurface(renderer, surface); 

//     while (true) {
//         SDL_Event event;
//         if (SDL_PollEvent(&event)) { 
//             if (event.type == SDL_QUIT) {// 退出事件
//                 break;
//             }
//         }
//         // 清屏 
//         SDL_RenderClear(renderer);

//         // 画一个长方形
//         SDL_Rect rect = {100, 100, 200, 200};// x, y, width, height
//         // 设置画笔的颜色 未红色
//         SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//         // 渲染器，长方形
//         SDL_RenderFillRect(renderer, &rect);


//         /// 绘制图片
//         /// 参数:渲染器，纹理，原点，目标矩形
//         /// 第三个参数:原点为NULL，则从左上角开始绘制
//         SDL_Rect dstrect = {200, 200, 200, 200};
//         /// 第四个参数:如何绘制，NULL表示从左上角开始绘制
//         SDL_RenderCopy(renderer, texture, NULL, &dstrect);


//         // 画文本
//         // surface可以拿到文本的宽高尺寸 更方便绘制
//         SDL_Rect textrect = {200, 300, surface->w, surface->h};
//         SDL_RenderCopy(renderer, texTextture, NULL, &textrect); // 画文本
//         // 画完之后再还原画笔颜色
//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//         // 更新屏幕
//         SDL_RenderPresent(renderer);

//      }
//     // 清理图片资源
//     // 释放纹理
//     SDL_DestroyTexture(texture);
//     IMG_Quit();

//     // 清理音频资源
//     // 释放音乐
//     Mix_FreeMusic(music);
//     // 关闭音频设备
//     Mix_CloseAudio();
//     // 退出SDL_Mixer
//     Mix_Quit();

//     //清理字体资源并退出
//     SDL_FreeSurface(surface);// 释放surface 也要释放 因为是指针
//     SDL_DestroyTexture(texTextture);
//     // 关闭字体
//     TTF_CloseFont(font);
//     // 退出SDL_ttf
//     TTF_Quit();

//     //清理并退出
//     SDL_DestroyRenderer(renderer); // 销毁渲染器
//     SDL_DestroyWindow(win); // 销毁窗口
//     SDL_Quit(); // 退出SDL
//     return 0; 
// }
