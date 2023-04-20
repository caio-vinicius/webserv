/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

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

#include "./Server.hpp"
#include "./Method.hpp"
#include "./Response.hpp"
#include "./Config.hpp"

bool ft::quit = false;

ft::Server::Server(void) {}

ft::Server::Server(Config config): _config(config) {
    std::map<std::string, ft::Config::Server>::iterator it_servers;
    std::vector<int>::iterator it_sockets;
    struct sockaddr_in serv_addr;
    int sockfd;
    int opt;

    std::cout << "Server created" << std::endl << std::endl;

    this->_sockets.reserve(this->_config.server.size());
    this->_pollfds.reserve(this->_config.server.size());
    this->_sockaddrs.reserve(this->_config.server.size());

    it_servers = this->_config.server.begin();
    for (; it_servers != this->_config.server.end(); it_servers++) {
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
            it_servers->second.listen.at(0).address.c_str());
        serv_addr.sin_port = htons((it_servers->second.listen.at(0).port));
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
            perror("Erro ao ligar o socket");
            exit(1);
        }
        if (listen(sockfd, 5)) {
            perror("Erro ao ligar o listen");
            exit(1);
        }
        // std::cout << "Server listening on " << \
        //     it_servers->second.listen.at(0).address;
        // std::cout << ":" << \
        //     it_servers->second.listen.at(0).port << std::endl;
        this->_sockaddrs.push_back(serv_addr);
        this->_sockets.push_back(sockfd);
    }

    // criando o que o poll precisa para funcionar
    it_sockets = this->_sockets.begin();
    for (; it_sockets != this->_sockets.end(); it_sockets++) {
        pollfd fd;
        fd.fd = *it_sockets;
        fd.events = POLLIN;
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

void requestLine(std::istringstream &ss,
    std::map<std::string, std::string> *header) {
    std::string request_line;
    std::string line;

    std::getline(ss, request_line);

    std::vector<std::string> vec = utils::split(request_line,  ' ');

    (*header)["Method"] = vec.at(0);
    (*header)["URI"] = vec.at(1);
    (*header)["Version"] = vec.at(2);
}

std::map<std::string, std::string> ft::Server::loadHeader(char *buffer) {
    std::map<std::string, std::string> header;
    std::string line;
    std::string str(buffer);
    std::istringstream ss(str);
    std::string key;
    std::string value;

    requestLine(ss, &header);
    while (std::getline(ss, line)) {
        if (line.empty()) {
            break;
        }
        key = line.substr(0, line.find(":"));
        value = line.substr(line.find(":") + 2);
        value = utils::remove_chr(value, 13);
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

void ft::Server::handleConnection(int client_fd) {
    int ret;
    char buffer[8192];  // 8K buffer
    std::map<std::string, std::string> header;

    if (client_fd < 0) {
        perror("Erro ao aceitar o cliente");
        exit(1);
    } else {
        std::cout << "Getting response" << std::endl;
        ret = recv(client_fd, buffer, sizeof(buffer), 0);
        std::cout << "ret: " << ret << std::endl;
        std::cout << "buffer: " << buffer << std::endl;
        std::cout << "Response recovered" << std::endl;
        if (ret < 0) {
            perror("Erro ao receber a mensagem");
            exit(1);
        }
        if (ret == 0)
            return;
        buffer[ret] = '\0';
        header = loadHeader(buffer);

        std::cout << header["Uri"] << std::endl << std::endl;

        Method *req = ft::Method::getRequest(header["Method"]);
        ft::Response res = req->buildResponse(header, this->_config);
        send(client_fd, res.message().c_str(), res.message().size(), 0);
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
