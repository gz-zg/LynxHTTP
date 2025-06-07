#pragma once


#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>


// ========================================
// 所有类的“前向声明” (解决类的循环依赖问题)
// ========================================

class Socket;
class InetAddress;


// ========================================
// InetAddress 类的定义
// ========================================

class InetAddress {
public:
    InetAddress();                              // 对 sockaddr_ 进行默认初始化
    explicit InetAddress(uint16_t port);        // 将 sockaddr_ 初始化为 IPv4 + 0.0.0.0 + port
    // explicit会禁止"隐式类型转换", 防止隐式转换: int -> uint16_t -> InetAddress 或 uint16_t -> InetAddress

    sockaddr* ptr();                            // 返回 sockaddr_ 的 sockaddr*
    const sockaddr* ptr_const() const;          // 返回 sockaddr_ 的 const sockaddr*
    socklen_t size() const;                     // 返回 sockaddr_ 的 socklen_t

private:
    sockaddr_in sockaddr_;                      // IPv4 地址
};


// ========================================
// TcpServer 类的定义
// ========================================

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


// ========================================
// TcpConnection 类的定义
// ========================================

class TcpConnection {
public:
    TcpConnection();                       
    ~TcpConnection();                                       

private:
    int sockfd_;
};