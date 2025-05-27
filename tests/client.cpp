#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void die(const char *msg) // [static]->声明一个静态函数(仅限当前文件可见) [const]->表示die函数内部不可修改指针指向的数组的内容
{
    int err = errno; // [errno]->全局变量,仅在系统调用失败时被设置为具体的错误码(若socket()失败,errno的值会是具体的错误码(如97),而非-1)。
    fprintf(stderr, "[%d] %s\n", err, msg); // [fprintf]->第一个参数必须是文件指针
    abort(); // [abort]->立即终止程序(不执行清理操作),触发核心转储(core dump),便于调试
}

static void msg(const char *msg) 
{
    fprintf(stderr, "msg: %s\n", msg);
}

void do_something(int connfd)
{
    char buf[1024];
    while(1)
    {
        fscanf(stdin, "%1023s", buf); // %s 格式说明符会读取一个字符串,并且会在字符串后添加一个'\0'作为字符串终止符('\0'后续内存可能为未初始化值), %1023s 确保最多读取 1023 个字符
        write(connfd, buf, strlen(buf));
        if(strcmp(buf, "quit") == 0) break;
        else
        {
            int n = read(connfd, buf, sizeof(buf));
            
            write(1, "server: ", 8);
            write(1, buf, n);
            write(1, "\n", 1);
        }
    }
}

int main()
{
    // socket
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    if(connfd < 0){
        die("socket()");
    }

    // connect
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234); // port 
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // IP 127.0.0.1
    // sin_port和sin_addr.s_addr这两个字段在socket API中 必须使用网络字节序(大端序)
    if(connect(connfd, (const struct sockaddr *)&addr, sizeof(addr))){
        die("connect()");
    }

    // do something
    do_something(connfd);

    // close
    close(connfd);

    return 0;
}