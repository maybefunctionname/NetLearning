#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include "lib/errorlog.h"

const unsigned int BUF_SIZE = 1024;
const unsigned int RLT_SIZE = 4;
const unsigned int OPSZ = 4;

int main(int argc, char* argv[]) {
    int sock;
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;
    struct sockaddr_in serv_addr;
    if (argc != 3) {
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        exit(1);
    }
    sock=socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");    
    }   
    else {
        puts("Connected..............");
    }
    fputs("Operand count :", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0]=(char)opnd_cnt;
    
    for (int i=0; i < opnd_cnt; ++i) {
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i*OPSZ+1]);
    }
    // 调用fgetc函数删掉缓冲中的字符\n
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
    printf("the opmsg is : %s", opmsg);
    write(sock, opmsg, opnd_cnt*OPSZ+2);
    read(sock, &result, RLT_SIZE);
    printf("Operation result: %d \n", result);
    close(sock);
    return 0;
}