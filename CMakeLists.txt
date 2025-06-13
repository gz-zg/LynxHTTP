cmake_minimum_required(VERSION 3.10)
project(LynxHTTP LANGUAGES CXX)     # 表示只启用 C++ 编译器

# 设置 C++ 的语言标准
set(CMAKE_CXX_STANDARD 17)          # 指定 C++ 编译标准
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 要求强制使用所设定的标准，不能自动降级
set(CMAKE_CXX_EXTENSIONS OFF)       # 是否允许编译器特有的扩展，设为 OFF 更可移植

# 定义所有静态库的名称
set(libs EventLoop http InetAddress TcpConn TcpServer)

# 循环创建所有静态库
foreach(lib IN LISTS libs)
    add_library(${lib}
        STATIC
            include/LynxHTTP/${lib}.hpp
            src/${lib}.cpp
    )
    target_include_directories(${lib} PUBLIC include)
endforeach()

# 创建main可执行文件
add_executable(main main.cpp)

# 给main可执行文件链接静态库
target_link_libraries(main PRIVATE ${libs})

# tests-client 可执行文件
add_executable(client tests/client.cpp)