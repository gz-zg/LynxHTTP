#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "LynxHTTP/TcpServer.hpp"
#include "LynxHTTP/http.hpp"

namespace fs = std::filesystem;

int main() 
{
    int cnt = 0;
    
    TcpServer server;
    server.start(7777, [&cnt](std::shared_ptr<TcpConn> conn)->void {
        std::cout << "成功建立了一个新的连接: " << cnt << std::endl;
        cnt++;
    
        conn->set_read_cb([conn]()->void {
            // 使用绝对路径指定web根目录
            const std::string web_root = "/root/LynxHTTP/web"; // Web根目录
            
            // 获取并解析HTTP请求
            std::string request_data = conn->get_all_data();
            HttpRequest request;
            request.parse_http_request(request_data);
            
            // 获取请求路径并构造文件路径
            std::string request_path = request.path_;
            if (request_path == "/") {
                request_path = "/index.html";
            }
            std::cout << request_path << std::endl;
            
            // 直接构造绝对路径
            fs::path full_path = fs::path(web_root) / request_path.substr(1);
            
            // 检查文件是否存在
            if (!fs::exists(full_path)) {
                // 路径无效，返回403禁止访问
                std::string forbidden = 
                    "HTTP/1.1 403 Forbidden\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 13\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "403 Forbidden";
                conn->send(forbidden.data(), forbidden.size());
                return;
            }
            
            // 尝试打开文件
            std::ifstream file(full_path, std::ios::in | std::ios::binary);
            if (!file.is_open()) {
                // 文件不存在，返回404
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

            // 读取文件内容
            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string file_content = buffer.str();
            file.close();

            // 确定内容类型
            std::string content_type = "text/plain";
            if (full_path.extension() == ".html") content_type = "text/html";
            else if (full_path.extension() == ".css") content_type = "text/css";
            else if (full_path.extension() == ".js") content_type = "application/javascript";
            else if (full_path.extension() == ".png") content_type = "image/png";
            else if (full_path.extension() == ".jpg") content_type = "image/jpeg";

            // 构造HTTP响应
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: " + content_type + "\r\n"
                "Content-Length: " + std::to_string(file_content.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" +
                file_content;

            // 发送响应
            conn->send(response.data(), response.size());
        });
    });

    return 0;
}
