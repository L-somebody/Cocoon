# 指定CMake最低版本
cmake_minimum_required(VERSION 3.15)

# 命名项目以及版本
project(Tutorial VERSION 1.0)

# 指定c++标准版本
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# 通过接口库设置C++标准
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# 使用生成器表达式记录使用编译器的类型
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# 适配编译器选项
target_compile_options(tutorial_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
    )

# 配置头文件
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 添加子文件夹
add_subdirectory(MathFunctions)

# 指定生成可执行文件的名称和相关源文件
add_executable(Tutorial tutorial.cpp)

# 指定目标文件链接的库
target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)

# 指定目标文件应从哪里寻找include文件
target_include_directories(Tutorial PUBLIC 
                          "${PROJECT_BINARY_DIR}"
                          )

# 安装配置
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
    DESTINATION include
    )