/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>



int strToInt(std::string str) {
    int num;

    std::stringstream ss(str);
    ss >> num;

    return num;
}


int main(int argc, char **argv) {
    std::string header("HTTP/1.1 200 OK\n");

    header = header + "Content-Type:text/html\n";

    header = header + "Content-Length: 16\n\n hello \n";

    std::cout << header << std::endl;

    std::map<std::string, std::string> conf;

    const char* configFilePath = "./etc/conf.conf";
    if (argc > 2) {
        exit(EXIT_FAILURE);
    } else if (argc != 1) {
        configFilePath = argv[1];
    }
    std::ifstream configFile(configFilePath);

    if (!configFile.is_open()) {
        exit(EXIT_FAILURE);
    }
    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string configName;
        std::string configValue;

        if (lineStream >> configName >> configValue) {
            conf[configName] = configValue;
        }
    }
    std::cout << conf["listen"] << std::endl;
    std::cout << conf["bind"] << std::endl;

    int sockfd, newsockfd, n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[256];
    int port = strToInt(conf["listen"]);
    std::cout << port << std::endl;

    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao abrir o socket");
        exit(1);
    }

    int opt;
    if (setsockopt(sockfd, SOL_SOCKET,
                SO_REUSEADDR | SO_REUSEPORT, &opt,
                sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // Configura o endereço do servidor
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(conf["bind"].c_str());
    serv_addr.sin_port = htons(port);

    // Liga o socket ao endereço e porta
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao ligar o socket");
        exit(1);
    }

    // Escuta por conexões
    listen(sockfd, 5);

    printf("Servidor aguardando conexões na porta %d...\n", port);

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
        n = send(newsockfd, header.c_str(), 67, 0);
        if (n < 0) {
           perror("Erro ao escrever no socket");
           exit(1);
        }


        // Fecha o socket da conexão
        close(newsockfd);
    }

    // Fecha o socket do servidor
    close(sockfd);
    return 0;
}
