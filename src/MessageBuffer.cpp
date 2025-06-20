#include "LynxHTTP/MessageBuffer.hpp"
#include <sys/uio.h>
#include <cstring>
#include <cerrno>

MessageBuffer::MessageBuffer() 
    : rpos_(0), wpos_(0)
{
    buffer_.resize(4096);
}

MessageBuffer::MessageBuffer(std::size_t size) 
    : rpos_(0), wpos_(0)
{
    buffer_.resize(size);
}

MessageBuffer::MessageBuffer(MessageBuffer&& other)
    : buffer_(std::move(other.buffer_)), rpos_(other.rpos_), wpos_(other.wpos_)
{
    other.rpos_ = 0;
    other.wpos_ = 0;
}

MessageBuffer& MessageBuffer::operator=(MessageBuffer&& other) 
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

uint8_t* MessageBuffer::get_read_ptr()
{
    return buffer_.data() + rpos_;
}

uint8_t* MessageBuffer::get_write_ptr()
{
    return buffer_.data() + wpos_;
}

std::size_t MessageBuffer::get_free_size()
{
    return buffer_.size() - wpos_;
}

std::size_t MessageBuffer::get_used_size()
{
    return wpos_ - rpos_; 
}

void MessageBuffer::read_completed(std::size_t size)
{
    rpos_ += size;
}

void MessageBuffer::write_completed(std::size_t size)
{
    wpos_ += size;
}

std::pair<uint8_t*, std::size_t> MessageBuffer::get_all_data()
{
    std::pair<uint8_t*, std::size_t> res = {get_read_ptr(), get_used_size()};
    read_completed(get_used_size());
    return res;
}

std::pair<uint8_t*, std::size_t> MessageBuffer::get_data_until_CRLF() // CRLF -> \r\n
{
    std::pair<uint8_t*, std::size_t> res = {nullptr, 0};
    for(std::size_t i = rpos_; i < wpos_ - 1; i++)
    {   
        if(buffer_[i] == '\r' && buffer_[i + 1] == '\n')
        {
            res = {get_read_ptr(), i - rpos_ + 2}; // 数据长度为 i - rpos_ + 2, 包含 \r\n
            read_completed(i - rpos_ + 2);
            return res;
        }
    }
    return res;
}

void MessageBuffer::normalize()
{
    if(rpos_ > 0)
    {
        memmove(buffer_.data(), get_read_ptr(), get_used_size());
        wpos_ = get_used_size();
        rpos_ = 0;
    }
}

void MessageBuffer::ensure_free_size(std::size_t size)
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

void MessageBuffer::write_to_buffer(uint8_t* data, std::size_t size)
{
    if(size > 0)
    {
        ensure_free_size(size);
        memcpy(get_write_ptr(), data, size); // 当 100% 确定内存不重叠时才使用memcpy,否则使用memmove
        write_completed(size);
    }
}

int MessageBuffer::recv(int fd, int* err)
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