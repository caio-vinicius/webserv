/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "Server.hpp"

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

ft::Server::Server(void) {
    std::cout << "Server created" << std::endl;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    this->_config.server["localhost:8080"].sockfd = sockfd;
    int opt;
    setsockopt(this->_config.server["localhost:8080"].sockfd,
        SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    bind(this->_config.server["localhost:8080"].sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(this->_config.server["localhost:8080"].sockfd, 5);
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(newsockfd, buffer, sizeof(buffer), 0);
    send(newsockfd, buffer, sizeof(buffer), 0);
    close(newsockfd);
    close(sockfd);
}

ft::Server::~Server() {
    std::cout << "Server destroyed" << std::endl;
}
