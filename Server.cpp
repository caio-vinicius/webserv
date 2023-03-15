/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Server.hpp"

#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <map>
#include <vector>
#include <iterator>
#include <sstream>
#include <string>
#include <fstream>

bool ft::quit = false;

int strToInt(std::string str) {
    int num;

    std::stringstream ss(str);
    ss >> num;

    return num;
}

ft::Server::Server(void) {}

ft::Server::Server(Config config): _config(config) {
    std::map<std::string, ft::Config::Server>::iterator it_servers;
    std::vector<int>::iterator it_sockets;
    struct sockaddr_in serv_addr;
    int sockfd;
    int opt;

    std::cout << "Server created" << std::endl;

    this->_sockets.reserve(this->_config.server.size());
    this->_pollfds.reserve(this->_config.server.size());
    this->_sockaddrs.reserve(this->_config.server.size());

    it_servers = config.server.begin();
    for (; it_servers != config.server.end(); it_servers++) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cout << "Error opening socket" << std::endl;
            exit(1);
        }
        fcntl(sockfd, F_SETFL, O_NONBLOCK);
        opt = 1;
        if (setsockopt(sockfd, SOL_SOCKET,
                SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("Erro ao abrir o setsockopt");
            exit(1);
        }
        memset(&serv_addr, 0, sizeof(sockaddr_in));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(
            it_servers->second.params["bind"].c_str());
        serv_addr.sin_port = htons(strToInt(it_servers->second.params["port"]));
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
            perror("Erro ao ligar o socket");
            exit(1);
        }
        if (listen(sockfd, 5)) {
            perror("Erro ao ligar o listen");
            exit(1);
        }
        std::cout << "Server listening on port " << \
            it_servers->second.params["port"] << std::endl;
        std::cout << "Server listening on bind " << \
            it_servers->second.params["bind"] << std::endl;
        std::cout << "Server fd is " << sockfd << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        this->_sockaddrs.push_back(serv_addr);
        this->_sockets.push_back(sockfd);
    }

    // criando o que o poll precisa para funcionar
    it_sockets = this->_sockets.begin();
    for (; it_sockets != this->_sockets.end(); it_sockets++) {
        pollfd fd;
        fd.fd = *it_sockets;
        fd.events = POLLIN;
        std::cout << "fd.fd: " << fd.fd << "" <<std::endl;
        this->_pollfds.push_back(fd);
    }
}

ft::Server::~Server() {
    std::vector<int>::iterator it = this->_sockets.begin();
    for (; it != this->_sockets.end(); it++) {
        close(*it);
    }
    std::cout << "Server destroyed" << std::endl;
}

std::map<std::string, std::string> ft::Server::loadHeader(char *buffer) {
    std::map<std::string, std::string> header;
    std::string line;
    std::string str(buffer);
    std::istringstream ss(str);
    std::string key;
    std::string value;

    std::getline(ss, line);
    header["Request-Line"] = line;

    while (std::getline(ss, line)) {
        if (line.empty()) {
            break;
        }
        key = line.substr(0, line.find(":"));
        value = line.substr(line.find(":") + 2);
        header[key] = value;
    }
    return (header);
}

void ft::Server::loadBody(char *buffer) {
    std::string str(buffer);
    std::istringstream ss(str);
    std::string line;

    while (std::getline(ss, line)) {
        if (line.empty()) {
            break;
        }
    }
}

char *ft::Server::buildResponse(void) {
    std::string status_line = "HTTP/1.1 200 OK\n";
    std::string content_type = "Content-Type: text/html\n";
    std::string content_length = "Content-Length: 12\n";
    std::string body = "\n\nHello World!";
    std::string response = status_line + content_type + content_length + body;
    return (response.c_str());
}

void ft::Server::handleConnection(int client_fd) {
    int ret;
    char buffer[8192];  // 8K buffer
    std::map<std::string, std::string> header;

    if (client_fd < 0) {
        perror("Erro ao aceitar o cliente");
        exit(1);
    } else {
        std::cout << "Cliente conectado" << std::endl;
        ret = recv(client_fd, buffer, sizeof(buffer), 0);
        if (ret < 0) {
            perror("Erro ao receber a mensagem");
            exit(1);
        }
        std::cout << "Mensagem recebida: " << buffer << std::endl;
        buffer[ret] = '\0';
        header = loadHeader(buffer);
        if (header["Request-Line"].find("GET") != std::string::npos) {
            std::cout << "GET" << std::endl;
        } else if (header["Request-Line"].find("POST") != std::string::npos) {
            std::cout << "POST" << std::endl;
            loadBody(buffer);
            // if (header["Content-Length"] == "0") {
        } else if (header["Request-Line"].find("DELETE") != std::string::npos) {
            std::cout << "DELETE" << std::endl;
        } else {
            std::cout << "Method not allowed" << std::endl;
        }
        const char *response;
        response = buildResponse();
        send(client_fd, response, strlen(response), 0);
        close(client_fd);
    }
}

ft::Server::client_fd ft::Server::waitConnections() {
    int ret;
    int client_fd;
    size_t i = 0;
    socklen_t clilen;

    ret = poll(&this->_pollfds[0], this->_config.server.size(), -1);
    if (ret == -1) {
        std::cout << "Error" << std::endl;
    } else if (ret == 0) {
        std::cout << "Timeout" << std::endl;
    }
    while (i < this->_config.server.size()) {
        if (this->_pollfds[i].revents & POLLIN) {
            std::cout << "POLLIN" << " " << i << std::endl;
            clilen = sizeof(this->_sockaddrs[i]);
            client_fd = accept(this->_sockets[i], \
                (struct sockaddr *)&this->_sockaddrs[i], &clilen);
            return (client_fd);
        }
        i++;
    }
    return (0);
}


void handle_signal(int) {
    ft::quit = true;
}

void ft::Server::run() {
    int client_fd;
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);

    while (!ft::quit) {
        client_fd = this->waitConnections();
        if (client_fd > 0) {
            this->handleConnection(client_fd);
        }
    }
    std::cout << "Server stopped" << std::endl;
}
