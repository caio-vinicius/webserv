#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888

int main()
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[256];

    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao abrir o socket");
        exit(1);
    }

    // Configura o endereço do servidor
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    // Conecta ao servidor
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao conectar ao servidor");
        exit(1);
    }

    printf("Conexão estabelecida com sucesso\n");

    // Envia uma mensagem ao servidor
    n = send(sockfd, "Mensagem enviada pelo cliente", 30, 0);
    if (n < 0) {
        perror("Erro ao escrever no socket");
        exit(1);
    }

    // Lê a resposta do servidor
    memset(buffer, 0, sizeof(buffer));
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        perror("Erro ao ler do socket");
        exit(1);
    }

    printf("Resposta do servidor: '%s'\n", buffer);

    // Fecha o socket do cliente
    close(sockfd);

    return 0;
}
