#include "LynxHTTP/TcpConn.hpp"

TcpConn::TcpConn(int conn_fd, EventLoop& evloop)
    : conn_fd_(conn_fd), evloop_(evloop) 
{

}             

TcpConn::~TcpConn()
{
    ::close(conn_fd_);
}  