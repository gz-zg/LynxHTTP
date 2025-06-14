#include "LynxHTTP/EventLoop.hpp"
#include <functional>
#include <iostream>
#include <unistd.h>

#define MAX_EVENTS 1024 // 宏定义写在cpp文件中是因为 MAX_EVENTS 这个宏不需要对外公开,只有当宏需要对外(调用接口者)公开时才将宏定义写在hpp文件中

EventLoop::EventLoop()
{
    epfd_ = ::epoll_create1(0);
}

EventLoop::~EventLoop()
{
    ::close(epfd_);
}

void EventLoop::add_event(int fd, uint32_t events, void* ptr) // 传入的 events 作用是什么？真的是事件吗？
{
    epoll_event ev;
    ev.events = events;
    ev.data.ptr = ptr;
    if(::epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) // ev 是在 ::epoll_ctl 函数内部进行了拷贝吗？
    {
        std::cerr << "epoll_ctl add error: " << errno << std::endl;
    }
}

void EventLoop::mod_event(int fd, uint32_t events, void* ptr)
{
    epoll_event ev;
    ev.events = events;
    ev.data.ptr = ptr;
    if(::epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1)
    {
        std::cerr << "epoll_ctl mod error: " << errno << std::endl;
    }
}

void EventLoop::del_event(int fd)
{           
    if(::epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, 0) == -1)
    {
        std::cerr << "epoll_ctl del error: " << errno << std::endl;
    }
}

void EventLoop::run()
{
    epoll_event events[MAX_EVENTS];
    while(true)
    {
        int nev = ::epoll_wait(epfd_, events, MAX_EVENTS, 1000); // 当设置了timeout的时候，在timeout期间即timeout后分别是什么情况？
        if(nev == -1)
        {
            std::cerr << "epoll_wait error: " << errno << std::endl;
        }
        else
        {
            for(int i = 0; i < nev; i++)
            {
                auto handler = static_cast<std::function<void(uint32_t)>*>(events[i].data.ptr);
                (*handler)(events[i].events);
            }
        }
    }
}