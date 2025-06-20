#pragma once

#include "LynxHTTP/EventLoop.hpp"
#include "LynxHTTP/MessageBuffer.hpp"
#include <cstddef>
#include <functional>
#include <string>

class TcpConn {
public:
    TcpConn(int conn_fd, EventLoop& evloop);  
    ~TcpConn();

    void set_read_cb(std::function<void()> read_cb);
    std::string get_all_data();
    std::string get_data_until_CRLF();
    int send(const char* data, size_t size);

private:
    bool closed_;
    int conn_fd_;
    EventLoop& evloop_;

    std::function<void(uint32_t)> io_handler_;
    void handle_io(uint32_t events);
    
    void handle_read();
    MessageBuffer input_buffer_;
    std::function<void()> read_cb_;
    
    void handle_write();
    std::string output_buffer_;
    
    void close();
};