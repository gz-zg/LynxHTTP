#pragma once

#include "LynxHTTP/EventLoop.hpp"
#include <cstdint>
#include <sys/epoll.h>
#include <functional>
#include <unistd.h>

class TcpConn {
public:
    TcpConn(int conn_fd, EventLoop& evloop);  
    ~TcpConn();

private:
    bool closed_;
    int conn_fd_;
    EventLoop& evloop_;
    std::function<void(uint32_t)> io_handler_;
    void handle_io(uint32_t events);
    void handle_read();
    void handle_write();
    void close();
};