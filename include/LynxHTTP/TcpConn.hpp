#pragma once

#include "LynxHTTP/EventLoop.hpp"
#include <unistd.h>

class TcpConn {
public:
    TcpConn(int conn_fd, EventLoop& evloop);  
    ~TcpConn();

private:
    int conn_fd_;
    EventLoop& evloop_;
};