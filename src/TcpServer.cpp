#include "LynxHTTP/TcpServer.hpp"
#include <cerrno>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>
#include "LynxHTTP/InetAddress.hpp"
#include <iostream>
#include "LynxHTTP/TcpConn.hpp"

// 创建一个 TCP + IPv4 的套接字 listen_fd_
TcpServer::TcpServer()
{
    listen_fd_ = -1;
}

// 释放这个 TCP + IPv4 的套接字 listen_fd_
TcpServer::~TcpServer() 
{
    if(listen_fd_ != -1)
    {
        ::close(listen_fd_);
    }
}

void TcpServer::start(int port)
{
    listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    
    // 给 listen_fd_ 设置选项
    int val = 1; // val 仅在第一次设置时有效，后续修改 val 的值不会影响已绑定的套接字选项
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); // 使服务器程序在重启后可以立马绑定到它之前使用的地址[IP:端口]

    // 给 listen_fd_ 绑定到一个本地地址(IPv4 + 端口), 并监听来自客户端的 TCP + IPv4 的连接
    InetAddress addr(port);
    if(::bind(listen_fd_, addr.ptr_const(), addr.size()) < 0)
    {
        // [失败处理]之后再写吧
    }
    ::listen(listen_fd_, SOMAXCONN);

    // 给 listen_fd_ 封装一个函数作为回调函数
    accept_handler_ = [this]()->void { this->handle_accept(); };
    
    // 将 listen_fd_ 添加到 事件循环 中
    evloop_.add_event(listen_fd_, EPOLLIN, &accept_handler_);

    // 运行 evloop_ 这个事件循环
    evloop_.run();

    std::cout << "Server started on port: " << port << std::endl;
}

void TcpServer::handle_accept()
{
    // 从监听套接字 listen_fd_ 的监听队列中接收客户端的连接请求,返回与客户端通信的新套接字的文件描述符
    InetAddress addr;
    socklen_t addr_len = sizeof(addr);
    int conn_fd = ::accept4(listen_fd_, addr.ptr(), &addr_len, SOCK_NONBLOCK);
    if(conn_fd == -1)
    {
        std::cerr << "accept4 error" << errno << std::endl;
        return;
    }

    // 创建 TcpConn 对象
    auto conn = std::make_shared<TcpConn>(conn_fd, evloop_);
    
}