#include "LynxHTTP/InetAddress.hpp"

// 对 sockaddr_ 进行默认初始化
InetAddress::InetAddress() {
    sockaddr_ = {};
}

// 将 sockaddr_ 初始化为 IPv4 + 0.0.0.0 + port
InetAddress::InetAddress(uint16_t port)
{
    sockaddr_.sin_family = AF_INET;                             // IPv4
    sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);    // IP 0.0.0.0       
    sockaddr_.sin_port = htons(port);
}

// 返回 sockaddr_ 的 sockaddr*
sockaddr* InetAddress::ptr() {
    return reinterpret_cast<sockaddr*>(&sockaddr_);
}

// 返回 sockaddr_ 的 const sockaddr*
const sockaddr* InetAddress::ptr_const() const {
    return reinterpret_cast<const sockaddr*>(&sockaddr_);
}

// 返回 sockaddr_ 的 socklen_t
socklen_t InetAddress::size() const {
    return sizeof(sockaddr_);
}