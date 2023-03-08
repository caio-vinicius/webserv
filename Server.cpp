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

    size_t size = this->_config.server.size();
    struct sockaddr_in *server_addr = new sockaddr_in[size];
    pollfd *fds = new pollfd[size];
    size_t i = 0;
    int client_fd1, client_fd2;
    char   buffer[1024];
    int ret;

    int port = 8080;
    for (; i < 2; it++) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        it->second.sockfd = sockfd;
        fds[i].fd = sockfd;
        fds[i].events = POLLIN;
        this->_sockets.insert(sockfd);
        int opt;
        setsockopt(it->second.sockfd,
            SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        memset(&server_addr[i], 0, sizeof(sockaddr_in));
        server_addr[i].sin_family = AF_INET;
        server_addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr[i].sin_port = htons(port);
        bind(it->second.sockfd, (struct sockaddr *)&server_addr[i], sizeof(server_addr[i]));
        listen(it->second.sockfd, 5);
        std::cout << fds[i].fd << " " << port << std::endl;
        port++;
        i++;
    }

    std::cout << "." << std::endl;
    while (true) {
        ret = poll(fds, size, -1);
        if (ret == -1) {
            std::cout << "Error" << std::endl;
        }
        else if (ret == 0) {
            std::cout << "Timeout" << std::endl;
        }
        if (fds[0].revents & POLLIN) {
            std::cout << "POLLIN 0" << std::endl;
            client_fd1 = accept(fds[0].fd, (struct sockaddr *)&server_addr[0], (socklen_t *)&server_addr[0]);
            if (client_fd1 < 0)
                std::cout << "Accept Failed" << std::endl;
            else {
                recv(client_fd1, buffer, sizeof(buffer), 0);
                std::cout << buffer << std::endl;
                send(client_fd1, buffer, sizeof(buffer), 0);
            }
        }
        if (fds[1].revents & POLLIN) {
            std::cout << "POLLIN 1" << std::endl;
            client_fd2 = accept(fds[1].fd, (struct sockaddr *)&server_addr[1], (socklen_t *)&server_addr[1]);
            if (client_fd2 < 0)
                std::cout << "Accept Failed" << std::endl;
            else {
                recv(client_fd2, buffer, sizeof(buffer), 0);
                std::cout << buffer << std::endl;
                send(client_fd2, buffer, sizeof(buffer), 0);
            }
        }
    }
}


ft::Server::~Server() {
    std::cout << "Server destroyed" << std::endl;
}
