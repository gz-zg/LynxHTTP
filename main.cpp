#include <iostream>
#include "LynxHTTP/TcpServer.hpp"

int main() 
{
    TcpServer server;
    server.start(6666, [](std::shared_ptr<TcpConn> conn)->void {
        std::cout << "成功建立了一个新的连接" << std::endl;
        conn->set_read_cb([conn]()->void {
            std::cout << "Received: " << conn->get_all_data() << std::endl;
            conn->send("HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!", 52);
        });
    });

    return 0;
}