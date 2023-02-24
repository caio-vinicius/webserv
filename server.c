/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080

int main()
{

    int sockfd, newsockfd, n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[256];

    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao abrir o socket");
        exit(1);
    }

    //fcntl(sockfd, F_SETFL, O_NONBLOCK);

    // Configura o endereço do servidor
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.1");
    serv_addr.sin_port = htons(PORT);

    // Liga o socket ao endereço e porta
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao ligar o socket");
        exit(1);
    }

    // Escuta por conexões
    listen(sockfd, 5);

    printf("Servidor aguardando conexões na porta %d...\n", PORT);

    // Loop principal
    while (1) {
        // Aceita uma nova conexão
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Erro ao aceitar a conexão");
            exit(1);
        }

        printf("Nova conexão estabelecida\n");

        // Lê dados do cliente
        memset(buffer, 0, sizeof(buffer));
        n = recv(newsockfd, buffer, sizeof(buffer), 0);
        if (n < 0) {
            perror("Erro ao ler do socket");
            exit(1);
        }

        printf("Mensagem recebida: %s\n", buffer);

        // Envia uma resposta ao cliente
        //n = send(newsockfd, "HTTP/1.1 200 OK\n\n", 17, 0);
        //if (n < 0) {
        //    perror("Erro ao escrever no socket");
        //    exit(1);
        //}

        // Fecha o socket da conexão
        close(newsockfd);
    }

    // Fecha o socket do servidor
    close(sockfd);

    return 0;
}
