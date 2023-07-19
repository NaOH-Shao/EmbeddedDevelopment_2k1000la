#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // 服务器IP地址，这里用本地回环地址作为示例
#define PORT 8888
#define BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // 创建套接字
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // 将IP地址转换为网络字节序并设置到server_addr中
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // 连接服务器
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server: %s:%d\n", SERVER_IP, PORT);

    // 与服务器进行通信
    while (1)
    {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // 发送消息给服务器
        if (send(client_socket, buffer, strlen(buffer), 0) == -1)
        {
            perror("Sending failed");
            break;
        }

        // 接收服务器回复
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1)
        {
            perror("Receiving failed");
            break;
        }
        else if (bytes_received == 0)
        {
            printf("Server closed the connection\n");
            break;
        }

        // 添加字符串结束符，以防止输出乱码
        buffer[bytes_received] = '\0';
        printf("Received from server: %s", buffer);
    }

    // 关闭客户端套接字
    close(client_socket);
    return 0;
}