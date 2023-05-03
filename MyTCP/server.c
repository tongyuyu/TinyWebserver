// TCP通信服务端

#include <stdio.h>
#include <arpa/inet.h>  // 导入套接字socket函数
#include <unistd.h>     // 包含read
#include <string.h>     // 包含strlen
#include <stdlib.h>

int main(){

    // 1.创建socket（用于监听的套接字）
    int lfd = socket(AF_INET, SOCK_STREAM, 0);  // 成功：返回一个用于监听的文件描述符lfd，操作的就是内核缓冲区。失败：-1

    // 判断
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }


    // 2.绑定bind
    struct sockaddr_in saddr;   //创建socket地址
    saddr.sin_family = AF_INET; //协议类型指定为为IPv4
    // inet_pton(AF_INET, "192.168.192.129", saddr.sin_addr.s_addr);     // 将点分十进制字符串转换为网络字节流整数，存储在saddr.sin_addr.s_addr中
    saddr.sin_addr.s_addr = INADDR_ANY; 
    //   INADDR_ANY = 0; 0表示0.0.0.0，表示任意地址，表示客户端任意地址都可以访问客户端，只有客户端可以这样写
    saddr.sin_port = htons(9999);    // 指定端口   htons(9999):把主机字节序，转换为网络字节序
    int ret = bind(lfd, (struct sockaddr *)&saddr,sizeof(saddr));     // 功能：绑定，将fd 和本地的IP + 端口进行绑定

    // 判断
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 监听
    ret = listen(lfd,8);  //    - 功能：监听这个socket上的连接  
    // 判断
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 4. 接收客户端的连接
    struct sockaddr_in clientaddr;
    unsigned int len = sizeof(struct sockaddr_in);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len); //接收客户端连接，默认是一个阻塞的函数，阻塞等待客户端连接. 返回值：成功 ：用于通信的文件描述符; 失败: -1
        // 判断
    if (cfd == -1) {
        perror("accept");
        exit(-1);
    }

    // 到这一步说明有客户端连接进来了，可以打印客户端的信息，客户端信息在clientaddr中

    // 输出客户端信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));  // 将网络字节流整数转换为点分十进制，存储到clientIP中
    unsigned short clientPort = ntohs(clientaddr.sin_port);// 转换端口
    printf("client ip is %s, port is port %d\n", clientIP, clientPort); // 输出IP地址和端口信息

    // 5.通信
    
    char recvBuf[1024] = {0};
    // 获取客户端数据
    while (1) {
        // 走到read继续阻塞，等待客户端发送数据
        int num = read(cfd, recvBuf, sizeof(recvBuf));  // 将文件描述符cfd读入recvBuf
        if (num == -1) {
            perror("read");
            exit(-1);
        } else if (num > 0) {
            printf("recv client data : %s\n", recvBuf);
        } else if (num == 0) {
            // 表示客户端断开连接
            printf("client closed...");
            break; // 如有多个客户端连接进来，不用break
        }

        // 给客户端发送数据
        // char * data = "hello, i am server";
        // 将客户端发送的数据recvBuf返回
        write(cfd, recvBuf, strlen(recvBuf)); // 将data写入文件描述符cfd中

    }  

    // 关闭文件描述符
    close(lfd);
    close(cfd);

    return 0;
}