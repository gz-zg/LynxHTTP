#include "LynxHTTP/http_request_parser.hpp"
#include <iostream>

int main() {
    std::string raw_request = 
        "POST /submit HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "name=test123";

    HttpRequest req = parse_http_request(raw_request);
    std::cout << "Method: " << req.method << "\nPath: " << req.path << "\nVersion: " << req.version << '\n';
    std::cout << "Header:" << std::endl;
    for(auto i : req.headers){
        std::cout << "\t" << i.first << ": " << i.first << std::endl;
    }
    std::cout << "Body:" << std::endl;
    std::cout << "\t" << req.body << std::endl;
}