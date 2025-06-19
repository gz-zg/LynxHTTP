#pragma once

#include <bits/types/struct_iovec.h>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <sys/uio.h>
#include <utility>
#include <vector>
#include <cstdint>

class MessageBuffer{
public:
    MessageBuffer() : rpos_(0), wpos_(0)
    {
        buffer_.resize(4096);
    }

    explicit MessageBuffer(std::size_t size) : rpos_(0), wpos_(0)
    {
        buffer_.resize(size);
    }

    // 禁止拷贝
    MessageBuffer(const MessageBuffer&) = delete;
    MessageBuffer& operator=(const MessageBuffer&) = delete;

    // 允许移动
    MessageBuffer(MessageBuffer&& other)
        : buffer_(std::move(other.buffer_)), rpos_(other.rpos_), wpos_(other.wpos_)
    {
        other.rpos_ = 0;
        other.wpos_ = 0;
    }

    MessageBuffer& operator=(MessageBuffer&& other) 
    {
        if(this != &other)
        {
            buffer_ = std::move(other.buffer_);
            rpos_ = other.rpos_;
            other.rpos_ = 0;
            wpos_ = other.wpos_;
            other.wpos_ = 0;
        }
        return *this;
    }

    uint8_t* get_read_ptr()
    {
        return buffer_.data() + rpos_;
    }

    uint8_t* get_write_ptr()
    {
        return buffer_.data() + wpos_;
    }

    std::size_t get_free_size()
    {
        return buffer_.size() - wpos_;
    }

    std::size_t get_used_size()
    {
        return wpos_ - rpos_; 
    }

    void read_completed(std::size_t size)
    {
        rpos_ += size;
    }

    void write_completed(std::size_t size)
    {
        wpos_ += size;
    }

    std::pair<uint8_t*, std::size_t> get_all_data()
    {
        std::pair<uint8_t*, std::size_t> res = {get_read_ptr(), get_used_size()};
        rpos_ = wpos_ = 0;
        return res;
    }

    std::pair<uint8_t*, std::size_t> get_data_until_CRLF() // CRLF -> \r\n
    {
        std::pair<uint8_t*, std::size_t> res = {nullptr, 0};
        for(std::size_t i = rpos_; i < wpos_ - 1; i++)
        {   
            if(buffer_[i] == '\r' && buffer_[i + 1] == '\n')
            {
                res = {get_read_ptr(), i - rpos_}; // 数据长度为 i - rpos_, 不包含 \r\n
                rpos_ = i + 2;
                return res;
            }
        }
        return res;
    }

    void normalize()
    {
        if(rpos_ > 0)
        {
            memmove(buffer_.data(), get_read_ptr(), get_used_size());
            wpos_ = get_used_size();
            rpos_ = 0;
        }
    }

    void ensure_free_size(std::size_t size)
    {
        if(buffer_.size() - get_used_size() < size)
        {
            normalize();
            buffer_.resize(buffer_.size() + std::max(size, buffer_.size() / 2));
        }
        else 
        {
            if(get_free_size() < size)
                normalize();
            else
                return;
        }
    }

    void write_to_buffer(uint8_t* data, std::size_t size)
    {
        if(size > 0)
        {
            ensure_free_size(size);
            memcpy(get_write_ptr(), data, size); // 当 100% 确定内存不重叠时才使用memcpy,否则使用memmove
            write_completed(size);
        }
    }

    int recv(int fd, int* err)
    {
        uint8_t bufferEX[65535];
        struct iovec iov[2];
        iov[0].iov_base = get_write_ptr();
        iov[0].iov_len = get_free_size();
        iov[1].iov_base = bufferEX;
        iov[1].iov_len = sizeof(bufferEX);
        std::size_t n = readv(fd, iov, 2); 

        if(n < 0)
        {
            *err = errno;
            return 0;
        }
        else if(n == 0)
        {
            *err = ECONNRESET;
            return 0;
        }
        else if(n <= get_free_size())
        {
            write_completed(n);
            return n;
        }
        else 
        {
            std::size_t bufferEX_size = n - get_free_size();
            write_completed(get_free_size());
            write_to_buffer(bufferEX, bufferEX_size);
            return n;
        }
    }

private:
    std::vector<uint8_t> buffer_;
    std::size_t rpos_;
    std::size_t wpos_;
};