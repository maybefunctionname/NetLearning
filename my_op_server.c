#include "lib/mynet.h"
#include "lib/errorlog.h"

const unsigned int OPSZ = 4;

int process(char* buf, int read_len);

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage : %s <PORT>", argv[0]);
        exit(1);
    }
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }
    
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1) {
        error_handling("accept() error");
    }
    printf("client%d is connected\n", clnt_sock);
    char buf[BUF_SIZE];
    // 直接读取client的数据打印到控制台上
    int read_len = read(clnt_sock, buf, BUF_SIZE-1);
    printf("the data length from the client is : %d \n", read_len);
    // 客户端传输的数据已经完全存入了buf中, buf[0:read_len-1]
    int result = process(buf, read_len);
    printf("the result is : %d \n", result);
    write(clnt_sock, &result, sizeof(result));
    close(clnt_sock);
    close(serv_sock);
    return 1;
}

int process(char *buf, int read_len) {
    int result = 0;
    int op_cnt = (int)buf[0];
    char op = buf[read_len-1];
    int nums[BUF_SIZE];
    memcpy(nums, buf+1, read_len-2);
    switch(op) {
        case '+':
            for (int i=0; i<op_cnt; ++i) {
                result+=nums[i];
            }
            break;
    }
    return result;
}

