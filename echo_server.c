#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "lib/errorlog.h"

const int BUF_SIZE = 1024;

int main (int argc , char* argv[]) {
    // 服务端套接字，自动创建的套接字
    int serv_sock, clnt_sock;
    // 服务端、客户端地址
    struct sockaddr_in serv_addr, clnt_addr;
    // 传输的消息
    char* message = "hello world!";
    if (argc != 2) {
        printf("the Usage : %s <PORT>", argv[0]);
        exit(1);
    }
    // 创建套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }
    // 初始化服务端地址
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // 将本机NIC对应的32bit形式的IP转化为网络字节序(一般都是大端序) 0x1234 大端序：0x1234 小端序：0x3412
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    // 将服务端套接字绑定服务端的地址端口
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    // 开始监听，服务端套接字才真正成为服务端套接字
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    // 调用accept时，阻塞当前线程, 客户端实际连接着服务端时，返回客户端的套接字
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1) {
        error_handling("accept() error");
    }
    printf("Connecting .......");
    // 往客户端套接字里写入数据
    write(clnt_sock, message, strlen(message));
    close(clnt_sock);
    close(serv_sock);    
}