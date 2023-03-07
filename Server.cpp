/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "Server.hpp"

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>

ft::Server::Server(void) {

}


ft::Server::Server(Config config): _config(config) {
    std::cout << "Server created" << std::endl;
    std::map<std::string, ft::Config::Server>::iterator it = this->_config.server.begin();
    for (; it != this->_config.server.end(); it++) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        it->second.sockfd = sockfd;
        this->_sockets.insert(sockfd);
    }
    int opt;
    setsockopt(it->second.sockfd,
        SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    bind(it->second.sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(it->second.sockfd, 5);
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);
    int newsockfd = accept(it->second.sockfd, (struct sockaddr *)&cli_addr, &clilen);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(newsockfd, buffer, sizeof(buffer), 0);
    send(newsockfd, buffer, sizeof(buffer), 0);
    close(newsockfd);
    close(it->second.sockfd);
    pollfd fds[1];
    fds[0].fd = it->second.sockfd;
    fds[0].events = POLLIN;
    int ret = poll(fds, 1, 1000);
    std::cout << ret << std::endl;
    if (ret == -1) {
        std::cout << "Error" << std::endl;
    }
    else if (ret == 0) {
        std::cout << "Timeout" << std::endl;
    }
    else {
        if (fds[0].revents & POLLIN) {
            std::cout << "POLLIN" << std::endl;
        }
        if (fds[0].revents & POLLOUT) {
            std::cout << "POLLOUT" << std::endl;
        }
    }
}


ft::Server::~Server() {
    std::cout << "Server destroyed" << std::endl;
}
