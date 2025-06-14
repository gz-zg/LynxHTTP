#include "LynxHTTP/TcpConn.hpp"
#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>

TcpConn::TcpConn(int conn_fd, EventLoop& evloop)
    : closed_(false), conn_fd_(conn_fd), evloop_(evloop)
{
    io_handler_ = [this](uint32_t events)->void { this->handle_io(events); };
    evloop_.add_event(conn_fd_, EPOLLIN | EPOLLOUT | EPOLLRDHUP, &io_handler_);
}             

TcpConn::~TcpConn()
{
    close();
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
    if(closed_ == false)
    {
        evloop_.del_event(conn_fd_);
        ::close(conn_fd_);
        closed_ = true;
    }
}