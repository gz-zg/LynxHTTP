#include "LynxHTTP/net.hpp"
#include <sys/socket.h>
#include <unistd.h>

// ========================================
// Socket类中函数的定义
// ========================================
Socket::Socket(){
    sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int fd):sockfd_(fd) {}

Socket::~Socket() {
    ::close(sockfd_);
}

void Socket::setsockopt(int level, int optname, int optval)
{
    int val = optval; // [val]->仅第一次设置有效，后续修改 val 不会影响已绑定的套接字选项
    ::setsockopt(sockfd_, level, optname, &val, sizeof(val));
}

void Socket::bind(const InetAddress& addr) {
    ::bind(sockfd_, addr.sockaddr_ptr_const(), addr.length());
}

void Socket::listen() {
    ::listen(sockfd_, SOMAXCONN);
}

int Socket::accept(InetAddress* peer_addr)
{
    socklen_t addr_len = sizeof(sockaddr_in);
    int connfd = ::accept(sockfd_, peer_addr->sockaddr_ptr(), &addr_len); // [accept]->用于从监听套接字的监听队列中接收客户端的连接请求,返回与客户端通信的新套接字的文件描述符。
    return connfd;
}

int Socket::fd() const {
    return sockfd_;
}

void Socket::close() {
    ::close(sockfd_);
}

// ========================================
// InetAddress类中函数的定义
// ========================================
InetAddress::InetAddress() {
    sockaddr_ = {};
}

InetAddress::InetAddress(uint16_t port)
{
    sockaddr_.sin_family = AF_INET;                             // IPv4
    sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);    // IP 0.0.0.0       
    sockaddr_.sin_port = htons(port);
}

const sockaddr* InetAddress::sockaddr_ptr_const() const {
    return (const sockaddr *)(&sockaddr_);
}

sockaddr* InetAddress::sockaddr_ptr() {
    return (sockaddr *)(&sockaddr_);
}

socklen_t InetAddress::length() const {
    return sizeof(sockaddr_);
}