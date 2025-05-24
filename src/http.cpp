#include "LynxHTTP/http.hpp" // 开头一般要引入对应的头文件
#include <iterator>
#include <sstream>

// HTTP请求解析函数-定义
HttpRequest parse_http_request(const std::string& raw_request)
{
    HttpRequest request;
    std::istringstream stream(raw_request);  // istringstream只能从流中读取内容, stringstream可读可写
    std::string line;

    // 1. 解析请求行
    if (std::getline(stream, line))
    {
        std::istringstream request_line(line);
        request_line >> request.method >> request.path >> request.version;
    }

    // 2. 解析Header
    while (std::getline(stream, line))
    {
        if (line == "\r") break;
        
        auto colon_pos = line.find(':');
        if (colon_pos != std::string::npos) 
        {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            // 去除前后空格
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t\r") + 1);
            request.headers[key] = value;
        }
    }

    // 3. 解析Body
    std::string body((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    request.body = body;

    return request;
}