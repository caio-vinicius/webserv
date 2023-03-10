/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Server.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <map>
#include <errno.h>
#include <iterator>
#include <sstream>

int strToInt(std::string str) {
    int num;

    std::stringstream ss(str);
    ss >> num;

    return num;
}

ft::Server::Server(void) {}


ft::Server::Server(Config config): _config(config) {
    std::cout << "Server created" << std::endl;
    std::map<std::string, ft::Config::Server>::iterator it\
         = this->_config.server.begin();

    size_t size = this->_config.server.size();
    struct sockaddr_in *server_addr = new sockaddr_in[size];
    pollfd *fds = new pollfd[size];
    size_t i = 0;
    int client_fd1, client_fd2;
    char   buffer[1024];
    int ret;

    std::map<string, ft::Config::Server>::iterator its = config.server.begin();
    for (; its != config.server.end(); its++) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Erro ao abrir o socket");
            exit(1);
        }
        it->second.sockfd = sockfd;
        fds[i].fd = sockfd;
        fds[i].events = POLLIN;
        this->_sockets.insert(sockfd);
        int opt;
        if (setsockopt(it->second.sockfd,
            SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {

            perror("Erro ao abrir o setsockopt");
            exit(1);
        }
        memset(&server_addr[i], 0, sizeof(sockaddr_in));
        server_addr[i].sin_family = AF_INET;
        server_addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        int port = strToInt(its->second.params["port"]);
        std::cout << port << "  |  " << its->second.params["port"] <<  std::endl;
        server_addr[i].sin_port = htons(port);
        if(bind(it->second.sockfd, (struct sockaddr *)&server_addr[i],
            sizeof(server_addr[i]))) {  
            perror("Erro ao ligar o socket");
            exit(1);
        }
        if (listen(it->second.sockfd, 5)) {
            perror("Erro ao ligar o listen");
            exit(1);
        }
        std::cout << fds[i].fd << " " << port << std::endl;
        i++;
    }

    std::cout << "." << std::endl;
    while (true) {
        ret = poll(fds, size, -1);
        if (ret == -1) {
            std::cout << "Error" << std::endl;
        } else if (ret == 0) {
            std::cout << "Timeout" << std::endl;
        }
        socklen_t clilen;
        if (fds[0].revents & POLLIN) {
            std::cout << "POLLIN 0" << std::endl;
            client_fd1 = accept(fds[0].fd, (struct sockaddr *)&server_addr[0], &clilen);
            if (client_fd1 < 0) {
                std::cout << "banana" << std::endl;
                break ;
            }
            if (client_fd1 < 0) {
                std::cout << "Accept Failed" << std::endl;
            } else {
                recv(client_fd1, buffer, sizeof(buffer), 0);
                std::cout << buffer << std::endl;
                send(client_fd1, buffer, sizeof(buffer), 0);
            }
        }
        if (fds[1].revents & POLLIN) {
            std::cout << "POLLIN 1" << std::endl;
            client_fd2 = accept(fds[1].fd, (struct sockaddr *)&server_addr[1], &clilen);;
            if (client_fd2 < 0) {
                std::cout << "Accept Failed" << std::endl;
            } else {
                recv(client_fd2, buffer, sizeof(buffer), 0);
                std::cout << buffer << std::endl;
                send(client_fd2, buffer, sizeof(buffer), 0);
            }
        }
        if (fds[2].revents & POLLIN) {
            std::cout << "POLLIN 2" << std::endl;
            client_fd2 = accept(fds[2].fd, (struct sockaddr *)&server_addr[2], &clilen);;
            if (client_fd2 < 0) {
                std::cout << "Accept Failed" << std::endl;
            } else {
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
