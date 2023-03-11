/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "Server.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <map>
#include <errno.h>
#include <vector>
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

    size_t size = this->_config.server.size();

    this->_sockets.reserve(size);
    this->_pollfds.reserve(size);
    this->_sockaddrs.reserve(size);

    int client_fd;
    char   buffer[1024];
    int ret;
    int sockfd;
    struct sockaddr_in server_addr;

    std::map<string, ft::Config::Server>::iterator it = config.server.begin();
    for (; it != config.server.end(); it++) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Erro ao abrir o socket");
            exit(1);
        }
        int opt;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("Erro ao abrir o setsockopt");
            exit(1);
        }
        memset(&server_addr, 0, sizeof(sockaddr_in));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(it->second.params["bind"].c_str());
        server_addr.sin_port = htons(strToInt(it->second.params["port"]));
        if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
            perror("Erro ao ligar o socket");
            exit(1);
        }
        if (listen(sockfd, 5)) {
            perror("Erro ao ligar o listen");
            exit(1);
        }
        std::cout << "Server listening on port " << it->second.params["port"] << std::endl;
        std::cout << "Server listening on bind " << it->second.params["bind"] << std::endl;
        std::cout << "Server fd is " << sockfd << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        this->_sockaddrs.push_back(server_addr);
        this->_sockets.push_back(sockfd);
    }

    // criando o que o poll precisa para funcionar
    std::vector<int>::iterator it2 = this->_sockets.begin();
    for (; it2 != this->_sockets.end(); it2++) {
        pollfd fd;
        fd.fd = *it2;
        fd.events = POLLIN;
        std::cout << "fd.fd: " << fd.fd << "" <<std::endl;
        this->_pollfds.push_back(fd);
    }

    std::cout << "." << std::endl;
    while (true) {
        ret = poll(&this->_pollfds[0], size, -1);
        std::cout << "poll" << std::endl;
        if (ret == -1) {
            std::cout << "Error" << std::endl;
        } else if (ret == 0) {
            std::cout << "Timeout" << std::endl;
        }
        socklen_t clilen;
        size_t i = 0;
        while(i < size) {
            if (this->_pollfds[i].revents & POLLIN) {
                std::cout << "POLLIN" << " " << i << std::endl;
                clilen = sizeof(this->_sockaddrs[i]);
                client_fd = accept(this->_sockets[i], (struct sockaddr *)&this->_sockaddrs[i], &clilen);
                if (client_fd < 0) {
                    perror("Erro ao aceitar o cliente");
                    exit(1);
                } else {
                    std::cout << "Cliente conectado" << std::endl;
                    if (recv(client_fd, buffer, sizeof(buffer), 0) < 0) {
                        perror("Erro ao receber a mensagem");
                        exit(1);
                    }
                    send(client_fd, buffer, sizeof(buffer), 0);
                }
            } // end if
        } // end while
    } // end while
}


ft::Server::~Server() {
    std::cout << "Server destroyed" << std::endl;
}
