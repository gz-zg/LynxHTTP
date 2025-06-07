cmake_minimum_required(VERSION 3.10)
project(LynxHTTP LANGUAGES CXX)     # 表示只启用 C++ 编译器

# 设置 C++ 的语言标准
set(CMAKE_CXX_STANDARD 17)          # 指定 C++ 编译标准
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 要求强制使用所设定的标准，不能自动降级
set(CMAKE_CXX_EXTENSIONS OFF)       # 是否允许编译器特有的扩展，设为 OFF 更可移植

# http 静态库模块
add_library(http
STATIC 
    include/LynxHTTP/http.hpp    
    src/http.cpp
)
target_include_directories(http 
PUBLIC 
    include
)

# main 可执行文件
add_executable(main main.cpp)
target_link_libraries(main 
PRIVATE 
    http
    net
)

# tests-client 可执行文件
add_executable(client tests/client.cpp)