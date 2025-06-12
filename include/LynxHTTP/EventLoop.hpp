#pragma once

#include <sys/epoll.h>

class EventLoop { 
public:
    EventLoop();
    ~EventLoop();

    void add_event(int fd, uint32_t events, void* ptr);
    void mod_event(int fd, uint32_t events, void* ptr);
    void del_event(int fd);
    
    void run();

private:
    int epfd_;
};