#include "LynxHTTP/TcpServer.hpp"
#include <sys/socket.h>
#include <unistd.h>


// 创建一个 TCP + IPv4 的套接字 sockfd_
TcpServer::TcpServer(){
    sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);

    // 给 sockfd_ 设置选项
    int val = 1; // val 仅在第一次设置时有效，后续修改 val 的值不会影响已绑定的套接字选项
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); // 使服务器程序在重启后可以立马绑定到它之前使用的地址[IP:端口]
}

// 释放这个 TCP + IPv4 的套接字 sockfd_
TcpServer::~TcpServer() {
    ::close(sockfd_);
}

// 给 sockfd_ 绑定到一个本地地址(IPv4 + 端口), 并监听来自客户端的 TCP + IPv4 的连接
void TcpServer::bind_listen(const InetAddress& addr) {  // const InetAddress& 只能调用 const 成员函数
    if(::bind(sockfd_, addr.ptr_const(), addr.size()) < 0)
    {
        // [失败处理]之后再写吧
    }
    ::listen(sockfd_, SOMAXCONN);
}

// 从监听套接字 sockfd_ 的监听队列中接收客户端的连接请求,返回与客户端通信的新套接字的文件描述符
int TcpServer::accept(InetAddress* peer_addr) {
    socklen_t peer_addr_len = peer_addr->size();
    int connfd = ::accept(sockfd_, peer_addr->ptr(), &peer_addr_len); 
    return connfd;
}

// 返回 sockfd_
int TcpServer::fd() {
    return sockfd_;
}


// ========================================
// TcpConnection 类中函数的定义
// ========================================