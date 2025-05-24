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