#include "LynxHTTP/TcpConn.hpp"
#include <cstdint>
#include <sys/epoll.h>

TcpConn::TcpConn(int conn_fd, EventLoop& evloop)
    : conn_fd_(conn_fd), evloop_(evloop) 
{
    io_handler_ = [this](uint32_t events)->void { this->handle_io(events); };
    evloop_.add_event(conn_fd_, EPOLLIN | EPOLLOUT | EPOLLRDHUP, &io_handler_);
}             

TcpConn::~TcpConn()
{
    evloop_.del_event(conn_fd_);
    ::close(conn_fd_);
}  

void TcpConn::handle_io(uint32_t events)
{
    if(events & EPOLLIN)
        handle_read();
    if(events & EPOLLOUT)
        handle_write();
    if(events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
        close();
}

void TcpConn::handle_read()
{

}

void TcpConn::handle_write()
{

}

void TcpConn::close()
{

}