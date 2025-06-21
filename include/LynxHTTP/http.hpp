#pragma once // 编译器指令,防止头文件被重复包含

#include <string>
#include <map>

// HTTP请求结构体-定义
class HttpRequest{
public:
    void parse_http_request(const std::string& raw_request);
private:
    void trim(std::string& s);

public:
    std::string method_;
    std::string path_;
    std::string version_;
    std::map<std::string, std::string> headers_;
    std::string body_;
};