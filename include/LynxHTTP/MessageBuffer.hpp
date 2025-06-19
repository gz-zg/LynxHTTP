#pragma once

#include <vector>
#include <cstdint>

class MessageBuffer{
public:
    MessageBuffer();
    explicit MessageBuffer(std::size_t size);
    // 禁止拷贝
    MessageBuffer(const MessageBuffer&) = delete;
    MessageBuffer& operator=(const MessageBuffer&) = delete;
    // 允许移动
    MessageBuffer(MessageBuffer&& other);
    MessageBuffer& operator=(MessageBuffer&& other);

    uint8_t* get_read_ptr();
    uint8_t* get_write_ptr();
    std::size_t get_free_size();
    std::size_t get_used_size();
    void read_completed(std::size_t size);
    void write_completed(std::size_t size);
    std::pair<uint8_t*, std::size_t> get_all_data();
    std::pair<uint8_t*, std::size_t> get_data_until_CRLF(); // CRLF -> \r\n
    void normalize();
    void ensure_free_size(std::size_t size);
    void write_to_buffer(uint8_t* data, std::size_t size);
    int recv(int fd, int* err);

private:
    std::vector<uint8_t> buffer_;
    std::size_t rpos_;
    std::size_t wpos_;
};