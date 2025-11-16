#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
    char const ip[INET_ADDRSTRLEN];
    printf("Введите IP-адрес сервера: ");
    scanf("%s", &ip);
    printf("Стучимся до %s\n", ip);
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Лидский А. А. ККСО-21-24 2 курс [КЛИЕНТ]";
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Ошибка при создании сокета \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    int err;
    if (err = inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("\nНекорректный адрес/Адрес не поддерживается: %s\n", ip);
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nОшибка подключения \n");
        return -1;
    }
  
    // subtract 1 for the null
    // terminator at the end
    send(client_fd, hello, strlen(hello), 0);
    printf("Сообщение отправлено!\n");
    valread = read(client_fd, buffer, 1024 - 1); 
    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}
