#include "LynxHTTP/http.hpp"
#include <sstream>
#include <algorithm>

void HttpRequest::parse_http_request(const std::string& raw_request) {
    std::istringstream stream(raw_request);
    std::string line;
    
    // 解析请求行
    if (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // 移除回车符
        }
        std::istringstream request_line(line);
        request_line >> method_ >> path_ >> version_;
    }
    
    // 解析头部字段
    while (std::getline(stream, line)) {
        if (line.empty() || line == "\r") break; // 空行结束头部
        if (line.back() == '\r') line.pop_back();
        
        auto pos = line.find(':');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            trim(name);
            trim(value);
            
            // 头部名称转为小写
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            headers_[name] = value;
        }
    }
    
    // 解析请求体（如果需要）
    std::string body((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    body_ = body;
}

// 辅助函数：去除字符串首尾空白
void HttpRequest::trim(std::string& s) 
{
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);
}