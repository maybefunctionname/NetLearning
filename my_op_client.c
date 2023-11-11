#include "lib/mynet.h"
#include "lib/errorlog.h"

const unsigned int OPSZ = 4;

int main (int argc, char* argv[]) {
    int serv_sock;
    struct sockaddr_in serv_addr;
    socklen_t serv_sock_sz;
    if (argc != 3) {
        printf("Usage : %s <IP> <PORT>", argv[0]);
        exit(1);
    }
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");        
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    
    if (connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }
    fputs("Connected........", stdout);
    // 此时已经连接上了，接着就是需要通过控制台输入待传递的数据了
    char buf[BUF_SIZE];
    char opinfo[BUF_SIZE];
    int op_cnt, op;
    fputs("please input the count:", stdout);
    scanf("%d", &op_cnt);
    // 如果已经明确了op_cnt就可以计算出来最终要传递的字节数为 op_cnt*OPSZ+1（操作数）+1（操作符位数）
    int opinfo_size = op_cnt * OPSZ + 1 + 1;
    for(int i = 1; i <= op_cnt; ++i) {
        printf("input the number%d : \n", i);
        scanf("%d", (int*)&opinfo[OPSZ*(i-1)+1]);
    }
    // 清空缓存中的\n
    fgetc(stdin);
    printf("input the op: \n");
    scanf("%c", &opinfo[op_cnt*OPSZ+1]);
    opinfo[0] = (char)op_cnt;
    // 信息组装完毕可以进行发送了
    write(serv_sock, opinfo, opinfo_size);
    printf("the opinfo is : %s", opinfo);
    fputs("the opinfo is send success", stdout);
    // 从客户端获取计算完之后的数据
    int result;
    read(serv_sock, &result, sizeof(result));
    printf("the result is : %d \n", result);
    close(serv_sock);
    return 1;       
}