#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SMTP_IP "163.xx.xx.xx"
#define SMTP_PORT 25
#define BUF 1024

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in smtp_addr;
    smtp_addr.sin_family = AF_INET;
    smtp_addr.sin_port = htons(SMTP_PORT);
    inet_pton(AF_INET, SMTP_IP, &smtp_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&smtp_addr, sizeof(smtp_addr)) < 0) {
        perror("connect");
        return -1;
    }

    char buf[BUF];
    // SMTP 流程
    send(sock, "HELO localhost\r\n", 16, 0);
    recv(sock, buf, BUF, 0);

    send(sock, "MAIL FROM:<test@163.com>\r\n", 25, 0);
    recv(sock, buf, BUF, 0);

    send(sock, "RCPT TO:<user@qq.com>\r\n", 23, 0);
    recv(sock, buf, BUF, 0);

    send(sock, "DATA\r\n", 6, 0);
    recv(sock, buf, BUF, 0);

    send(sock, "Subject: 测试邮件\r\n\r\n内容\r\n.\r\n", 30, 0);
    recv(sock, buf, BUF, 0);

    send(sock, "QUIT\r\n", 6, 0);
    close(sock);
    printf("邮件发送完成\n");
    return 0;
}
