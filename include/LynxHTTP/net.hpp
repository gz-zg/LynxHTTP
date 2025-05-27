#pragma once


#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>


// ========================================
// 所有类的“前向声明” (解决类的循环依赖问题)
// ========================================
class Socket;
class InetAddress;
// class Channel;
// class EventLoop;
// class Poller;
// class Acceptor;
// class Connection;


// ========================================
// Socket类的定义
// ========================================
class Socket {
public:
    Socket();
    explicit Socket(int fd);  // explicit会禁止"隐式类型转换", 防止 int 被隐式转换为 Socket
    ~Socket();

    void setsockopt(int level, int optname, int optval);
    void bind(const InetAddress& addr);
    void listen();
    int accept(InetAddress* peer_addr);
    // void set_non_blocking();
    int fd() const;
    void close();

private:
    int sockfd_;
};


// ========================================
// InetAddress类的定义
// ========================================
class InetAddress {
public:
    InetAddress();
    explicit InetAddress(uint16_t port);

    const sockaddr* sockaddr_ptr_const() const;
    sockaddr* sockaddr_ptr();
    socklen_t length() const;

private:
    sockaddr_in sockaddr_;
};

// // ========================================
// // Channel类的定义
// // ========================================
// #include <sys/epoll.h>
// #include <functional>

// class Channel {
// public:
//     Channel(EventLoop* loop, int fd);
//     void set_read_callback(std::function<void()> cb);
//     void set_write_callback(std::function<void()> cb);
//     void handle_event(uint32_t events);
//     void enable_reading();
//     int fd() const;
//     uint32_t events() const;

// private:
//     EventLoop* loop_;
//     int fd_;
//     uint32_t events_;
//     std::function<void()> read_callback_;
//     std::function<void()> write_callback_;
// };

// // ========================================
// // EventLoop类的定义
// // ========================================
// class EventLoop {
// public:
//     EventLoop();
//     ~EventLoop();

//     void loop();
//     void update_channel(Channel* channel);

// private:
//     bool quit_;
//     std::unique_ptr<Poller> poller_;
// };

// // ========================================
// // Poller类的定义
// // ========================================
// class Poller {
// public:
//     Poller();
//     ~Poller();

//     std::vector<Channel*> poll(int timeout_ms);
//     void update_channel(Channel* channel);

// private:
//     int epoll_fd_;
//     std::unordered_map<int, Channel*> channels_;
// };

// // ========================================
// // Acceptor类的定义
// // ========================================
// class Acceptor {
// public:
//     Acceptor(EventLoop* loop, const InetAddress& listen_addr);
//     void set_new_conn_callback(std::function<void(int)> cb);
//     void listen();

// private:
//     Socket listen_sock_;
//     Channel channel_;
//     std::function<void(int)> new_conn_cb_;
// };

// // ========================================
// // Connection类的定义
// // ========================================
// class Connection {
// public:
//     Connection(EventLoop* loop, int sockfd);
//     ~Connection();

//     void handle_read();
//     void handle_write();
//     int fd() const;

// private:
//     Socket socket_;
//     Channel channel_;
// };
