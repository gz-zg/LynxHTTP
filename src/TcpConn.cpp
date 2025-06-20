#include "LynxHTTP/TcpConn.hpp"
#include <cerrno>
#include <cstdint>
#include <sys/epoll.h>
#include <sys/socket.h>
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

void TcpConn::set_read_cb(std::function<void()> read_cb)
{
    read_cb_ = read_cb;
}

std::string TcpConn::get_all_data()
{
    auto data = input_buffer_.get_all_data();
    std::string result((char*)data.first, data.second);
    return result;
}

std::string TcpConn::get_data_until_CRLF()
{
    auto data = input_buffer_.get_data_until_CRLF();
    std::string result((char*)data.first, data.second);
    return result;
}

int TcpConn::send(const char* data, size_t size)
{
    output_buffer_.append(data, size);
    return size;
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
    int err = 0;
    int n = input_buffer_.recv(conn_fd_, &err);
    if(n > 0)
    {
        if(read_cb_)
            read_cb_();
    }
    else if(n == 0 || (n < 0 && err != EAGAIN && err != EWOULDBLOCK)) // 这是什么情况?
    {
        close();
    }
}

void TcpConn::handle_write()
{
    int n = ::send(conn_fd_, output_buffer_.data(), output_buffer_.size(), MSG_NOSIGNAL);
    if(n > 0)
    {
        output_buffer_.erase(0, n);
    }
    else if(n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) // 这是什么情况?
    {
        close();
    }
}

void TcpConn::close()
{
    if(closed_ == false)
    {
        evloop_.del_event(conn_fd_);
        ::close(conn_fd_);
        conn_fd_ = -1;
        closed_ = true;
    }
}