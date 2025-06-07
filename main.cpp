#include "LynxHTTP/http.hpp"
#include "LynxHTTP/net.hpp"
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void do_something(int connfd)
{
    char buf[1024];
    while(1)
    {
        int n = read(connfd, buf, sizeof(buf));
        /*
            [read()]->原始字节操作,仅将数据以二进制字节流形式读取到缓冲区，不做任何额外处理(包括不添加'\0')。read的第三个参数指定最大可读取的字节数,write的第三个参数指定实际写入的字节数。
            
            read()系统调用返回0的唯一条件是: 检测到文件描述符(如套接字)的 EOF(End of File),即对方已关闭连接或到达文件末尾。
            1. 如果read()的文件描述符指向的是套接字,当客户端调用close()关闭连接时，服务器端的read()会返回0，表示对方已发送EOF信号。
            2. 如果read()的文件描述符指向的是文件，当读取到文件末尾时也会返回0。

            当文件中只有 1 个字节，read(fd, buf, 2) 第一次调用会返回 1（实际读取的字节数），而非 0，read 仅当文件指针已到达文件末尾且无数据可读时，返回 0。
        */
        
        buf[n] = '\0';
        if(strcmp(buf, "quit") == 0) break;
        write(connfd, buf, n);

        write(1, "client: ", 8);
        write(1, buf, n);
        write(1, "\n", 1);
    }
}

int main() 
{
    TcpServer test_sock;
    InetAddress test_addr(1234);
    test_sock.bind(test_addr);
    test_sock.listen();
    
    std::cout << "TCP服务器已启动..." << std::endl;


    while(1)
    {
        InetAddress peer_addr;
        int connfd = test_sock.accept(&peer_addr);
        //TcpServer peer_sockfd(connfd);
        //do_something(peer_sockfd.fd());
    }
    

    return 0;
}