#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include "lib/errorlog.h"

const int BUF_SIZE = 1024;

int main (int argc, char * argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    if (argc != 3) {
        printf("the Usage : %s <IP> <PORT>", argv[0]);
        exit(1);
    }
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    int read_len = read(sock, message, BUF_SIZE-1);
    message[read_len] = '\0';
    
    printf("the message from server is : %s \n", message);
    close(sock);
    }