# 一.标题
cmake_minimum_required(VERSION 3.10.0)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)

# 二.设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 三.设置编译器
if(MSVC)
    #在windows下这样写
    add_compile_options(/W4) 
else()
    #在Mac和linux下这样写
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 四.设置编译输出目录
#表示在CMakeLists.txt所在的目录下并将编译生成的可执行文件放到该目录下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/opt/homebrew/Cellar/sdl2_image/2.8.8/lib/cmake")

# 五.设置程序名称
#用set来设置变量  TARGET是变量名 
#CMAKE_SYSTEM_NAME是系统名称 Mac下叫Darwin Linux下叫Linux Windows下叫Windows
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# if(APPLE)

# 二.查找并载入CMake预设
# 找到该包   REQUIRED 必须找到
find_package(SDL2 REQUIRED) # 基础包
find_package(SDL2_image REQUIRED) # 图像包
find_package(SDL2_mixer REQUIRED) # 音频包
find_package(SDL2_ttf REQUIRED) # 字体包

# # 额外的头文件路径
# set(SDL_IMAGE_H "/opt/homebrew/Cellar/sdl2_image/2.8.8/include/SDL2")
# include_directories(${SDL_IMAGE_H}) # 添加SDL_image头文件搜索路径

# # # 额外的库文件路径
# set(SDL_IMAGE_L "/opt/homebrew/Cellar/sdl2_image/2.8.8/lib")
# link_directories(${SDL_IMAGE_L}) # 添加SDL_image库搜索路径

 
# include_directories(${SDL2_IMAGE_INCLUDE_DIR})
# target_link_libraries(${TARGET} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY})



# 包含头文件目录
# include_directories(
#     ${SDL2_INCLUDE_DIRS}
#     ${SDL2_IMAGE_INCLUDE_DIRS}
# )
#导入图片库
# set(SDL_Image_H "/opt/homebrew/Cellar/sdl2_image/2.8.8/include/SDL2")
# include_directories(${SDL_Image_H})

# set(SDL_Image_L "/opt/homebrew/Cellar/sdl2_image/2.8.8/lib")
# link_directories(${SDL_Image_L})


 
# elseif(WIN32)
# #如果是windows平台

# endif(UNIX)


# 三.添加可执行文件
# 2.编译 创建可执行文件
add_executable(${TARGET} 
            "src/main.cpp"
            "src/Game.cpp"
            "src/SceneMain.cpp")
# add_executable(${TARGET} 
#            main.cpp)
# 四. 链接库
target_link_libraries(${TARGET} 
 ${SDL2_LIBRARIES}
SDL2_image::SDL2_image
SDL2_mixer::SDL2_mixer
SDL2_ttf::SDL2_ttf
)