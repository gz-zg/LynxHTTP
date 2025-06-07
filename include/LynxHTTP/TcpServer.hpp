#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include "LynxHTTP/InetAddress.hpp"

class TcpServer {
public:
    TcpServer();                                 // 创建一个 TCP + IPv4 的套接字 sockfd_
    ~TcpServer();                                // 释放这个 TCP + IPv4 的套接字 sockfd_

    void bind_listen(const InetAddress& addr);   // 给 sockfd_ 绑定到一个本地地址(IPv4 + 端口), 并监听来自客户端的 TCP + IPv4 的连接
    int accept(InetAddress* peer_addr);          // 从监听套接字 sockfd_ 的监听队列中接收客户端的连接请求,返回与客户端通信的新套接字的文件描述符
    int fd();                                    // 返回 sockfd_

private:
    int sockfd_;                                 // TCP 服务器的 套接字
};