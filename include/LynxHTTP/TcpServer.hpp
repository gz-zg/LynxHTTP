#pragma once

#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
#include "LynxHTTP/EventLoop.hpp"

class TcpServer {
public:
    TcpServer();                                        // 创建一个 TCP + IPv4 的套接字 sockfd_
    ~TcpServer();                                       // 释放这个 TCP + IPv4 的套接字 sockfd_

    void start(int port);

private:
    EventLoop evloop_;                                   // TCP 服务器的 事件循环
    int listen_fd_;                                      // TCP 服务器的 套接字
    std::function<void(uint32_t)> accept_handler_;       // TCP 服务器的 accept处理器, 它内部封装了当前类中的 handle_accept 成员函数
    
    void handle_accept(uint32_t events);
};