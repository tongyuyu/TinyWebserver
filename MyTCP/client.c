// TCP通信客户端

#include <stdio.h>
#include <arpa/inet.h>  // 导入套接字socket函数
#include <unistd.h>     // 包含read
#include <string.h>     // 包含strlen
#include <stdlib.h>

int main(){

    // 1.创建套接字socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // 判断】
    if (fd == -1){
        perror("socket");
        exit(-1);
    }

    // 2.连接服务器端
    struct sockaddr_in serveraddr;    // 创建服务器端sock地址
    serveraddr.sin_family = AF_INET;    //协议类型指定为为IPv4
    inet_pton(AF_INET, "192.168.192.129", &serveraddr.sin_addr.s_addr); //将点分十进制字符串转换为网络字节流整数，存储在serveraddr.sin_addr.s_addr中
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)); //功能： 客户端连接服务器  sockfd : 用于通信的文件描述符 - addr : 客户端要连接的服务器的地址信息 - addrlen : 第二个参数的内存大小

    // 判断】
    if (ret == -1){
        perror("connect");
        exit(-1);
    }

    // 到此处已成功连接

    // 3.通信

    char recvBuf[1024] = {0};
    char data[1024];
    while (1) {
        // 给服务器端发送数据
        // char * data = scanf("请输入要发送的内容：");
        // write(fd, data, strlen(data)); // 将data写入文件描述符fd中


        
        printf("请输入要发送的内容：");
        scanf("%s", data);
        write(fd, (void*)data, strlen(data) + 1);

        // sleep(1);

        // 读取服务器端发送过来的数据
        
        // 走到read继续阻塞，等待客户端发送数据
        int len = read(fd, recvBuf, sizeof(recvBuf));  // 将文件描述符fd读入recvBuf
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            printf("recv server data : %s\n", recvBuf);
        } else if (len == 0) {
            // 表示服务器端断开连接
            printf("server closed...");
            break;
        }

    }

    

    // 关闭连接
    close(fd);


    return 0;
}