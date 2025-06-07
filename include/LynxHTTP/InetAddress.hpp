#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>

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