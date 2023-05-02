/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>

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


ft::Server::Server(Config config): _config(config) {
    std::cout << "Server created" << std::endl << std::endl;

    ft::Config::servers server = config.server;
    this->_sockets = this->createSockets(server);
}

ft::Server::~Server() {
    std::vector<int>::iterator it = this->_sockets.begin();
    for (; it != this->_sockets.end(); it++) {
        close(*it);
    }
    std::cout << "Server destroyed" << std::endl;
}

void handle_signal(int) {
    ft::quit = true;
}

void ft::Server::run() {
    int client_fd, server_fd;
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);

    while (!ft::quit) {
        this->waitConnections(&client_fd, &server_fd);
        if (client_fd > 0) {
            this->handleConnection(client_fd, server_fd);
        }
    }
    std::cout << "Server stopped" << std::endl;
}


void ft::Server::handleConnection(int client_fd, int server_fd) {
    int ret;
    char buffer[8192];  // 8K buffer
    std::map<std::string, std::string> header;
    ft::Config::Server  *server;

    if (client_fd < 0) {
        perror("Erro ao aceitar o cliente");
        exit(1);
    } else {
        std::cout << "Getting response" << std::endl;
        ret = recv(client_fd, buffer, sizeof(buffer), 0);
        if (ret < 0) {
            perror("Erro ao receber a mensagem");
            exit(1);
        }
        if (ret == 0)
            return;
        buffer[ret] = '\0';
        header = loadHeader(buffer);

        server = getServer(server_fd, &header);
        Method *req = ft::Method::getRequest(header["Method"]);
        std::string res = req->buildResponse(header, server);
        send(client_fd, res.c_str(), res.size(), 0);
        close(client_fd);
    }
}

void    ft::Server::waitConnections(int *client_fd, int *server_fd) {
    int ret;

    std::vector<pollfd> pfds;
    for (std::vector<int>::iterator it_sockets = this->_sockets.begin();
        it_sockets != this->_sockets.end(); it_sockets++) {
        pollfd fd;
        fd.fd = *it_sockets;
        fd.events = POLLIN;
        pfds.push_back(fd);
    }
    ret = poll(&pfds[0], pfds.size(), -1);
    if (ret == -1) {
        std::cout << "Error" << std::endl;
    } else if (ret == 0) {
        std::cout << "Timeout" << std::endl;
    }
    for (size_t i = 0; i < pfds.size(); i++) {
        if (pfds[i].revents & POLLIN) {
            sockaddr_in address;
            socklen_t address_len = sizeof(address);
            *server_fd = this->_sockets[i];
            *client_fd = accept(this->_sockets[i],
                reinterpret_cast<sockaddr *>(&address), &address_len);
            return;
        }
    }
}

std::string ft::Server::getAddressByName(std::string name) {
    struct addrinfo hints, *res;
    int status;
    char ipstr[INET6_ADDRSTRLEN];
    std::string ip;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(name.c_str(), NULL, &hints, &res)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        exit(1);
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    void *addr = &(ipv4->sin_addr);
    inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
    ip = ipstr;
    freeaddrinfo(res);
    return (ip);
}

bool ft::quit = false;

ft::Server::Server(void) {}

std::vector<int> ft::Server::createSockets(Config::servers server) {
    std::vector<int> sockets;
    for (Config::iterator it = server.begin() ; it != server.end(); it++) {
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            std::cout << "Error opening socket" << std::endl;
            exit(1);
        }
        fcntl(socket_fd, F_SETFL, O_NONBLOCK);
        int opt = 1;
        if (setsockopt(socket_fd, SOL_SOCKET,
                SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("Erro ao abrir o setsockopt");
            exit(1);
        }

        struct sockaddr_in serv_addr;
        std::string listen_t = utils::getAddress(it->first);
        int port = utils::getPort(it->first);

        memset(&serv_addr, 0, sizeof(sockaddr_in));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(
            this->getAddressByName(listen_t).c_str());
        serv_addr.sin_port = htons(port);
        if (bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
            perror("Erro ao ligar o socket");
            exit(1);
        } else if (listen(socket_fd, 5)) {
            perror("Erro ao ligar o listen");
            exit(1);
        }
        sockets.push_back(socket_fd);
        for (size_t i = 0; i < it->second.size(); i++) {
            this->_config.server.at(it->first).at(i).fd = socket_fd;
        }
    }
    return (sockets);
}

void requestLine(std::istringstream &ss,
    std::map<std::string, std::string> *header) {
    std::string request_line;
    std::string line;

    std::getline(ss, request_line);
    request_line = utils::remove_chr(request_line, '\r');
    std::vector<std::string> vec = utils::split(&request_line,  ' ');
    (*header)["Method"] = vec.at(0);
    (*header)["Uri"] = vec.at(1);
    (*header)["Version"] = vec.at(2);
}


ft::Server::header_type ft::Server::loadHeader(char *buffer) {
    ft::Server::header_type header;
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
        value = utils::remove_chr(value, '\r');
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

ft::Config::Server *ft::Server::getServer(int server_fd,
    std::map<std::string, std::string> *header) {
    ft::Config::iterator    it;
    if (!header->count("Host") || header->at("Host").empty()) {
        return NULL;
    }

    for (it = this->_config.server.begin();
        it != this->_config.server.end(); it++) {
        if (it->second.at(0).fd == server_fd)
            break;
    }

    if (header->at("Host").compare(it->first) && it->second.size() > 1) {
        for (size_t i = 0; i < it->second.size(); i++) {
            if (std::find(it->second.at(i).server_name.begin(),
                it->second.at(i).server_name.end(),
                header->at("Host")) != it->second.at(i).server_name.end())
                return (&it->second.at(i));
        }
    }

    // Lógica para Default_Server
    return (&it->second.at(0));
}
