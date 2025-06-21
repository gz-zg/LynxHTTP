#pragma once // 编译器指令,防止头文件被重复包含

#include <string>
#include <map>

// HTTP请求结构体-定义
struct HttpRequest
{
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

// HTTP请求解析函数-声明
HttpRequest parse_http_request(const std::string& raw_request);




            // const std::string html_body =
            //     "<!DOCTYPE html>\r\n"
            //     "<html>\r\n"
            //     "<head><title>Test Page</title></head>\r\n"
            //     "<body><h1>Hello from LynxHTTP</h1></body>\r\n"
            //     "</html>\r\n";
        
            // std::string response = 
            //     "HTTP/1.1 200 OK\r\n"
            //     "Content-Type: text/html; charset=UTF-8\r\n"
            //     "Content-Length: " + std::to_string(html_body.size()) + "\r\n"
            //     "Connection: close\r\n"
            //     "\r\n" +
            //     html_body;

            // conn->send(response.data(), response.size());