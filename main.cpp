#include <iostream>
#include <fstream>
#include <sstream>
#include "LynxHTTP/TcpServer.hpp"

int main() 
{
    int cnt = 0;
    
    TcpServer server;
    server.start(7777, [&cnt](std::shared_ptr<TcpConn> conn)->void {
        std::cout << "成功建立了一个新的连接: " << cnt << std::endl;
        cnt++;
    
        conn->set_read_cb([conn]()->void {
            std::cout << "Received: " << std::endl << conn->get_all_data() << std::endl;

            // 1. 读取 HTML 文件内容
            std::ifstream file("/root/LynxHTTP/web/index.html", std::ios::in | std::ios::binary);
            if (!file.is_open()) {
                // 文件读取失败，返回 404
                std::string not_found = 
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 13\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "404 Not Found";
                conn->send(not_found.data(), not_found.size());
                return;
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string html_body = buffer.str();
            file.close();

            // 2. 构造 HTTP 响应
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "Content-Length: " + std::to_string(html_body.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" +
                html_body;

            // 3. 发送响应
            conn->send(response.data(), response.size());
        });
    });

    return 0;
}