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

void loadRequestLine(std::istringstream &ss,
                     ft::Server::headerType &header) {
    std::string requestLine;

    std::getline(ss, requestLine);
    requestLine = utils::removeChr(requestLine, '\r');
    std::vector<std::string> v = utils::split(&requestLine, SP);
    header["Method"] = v.at(0);
    header["Uri"] = v.at(1).substr(0, v.at(1).find("?"));
    if (v.at(1).find("?") != std::string::npos) {
        header["Query-String"] = v.at(1).substr(v.at(1).find("?") + 1);
    }
    header["Version"] = v.at(2);
}

void loadHeader(std::istringstream &ss,
                ft::Server::headerType &header) {
    std::string line;
    std::string key;
    std::string value;

    while (std::getline(ss, line)) {
        if (line.empty() || line == "\r") {
            break;
        }
        key = line.substr(0, line.find(":"));
        value = line.substr(line.find(":") + 2);
        value = utils::removeChr(value, '\r');
        header[key] = value;
    }
}

bool isChunkedRequest(ft::Server::headerType &header) {
    if (header.at("Method").compare("POST") == 0
        && header.count("Content-Length") == 0
        && header.count("Transfer-Encoding") != 0
        && header.at("Transfer-Encoding").compare("chunked") == 0) {
        return true;
    }
    return false;
}

std::string getBodySize(ft::Server::bodyType &body) {
    std::stringstream ss;

    ss << body.size();
    return ss.str();
}

void loadChunkedBody(std::istringstream &ss,
              ft::Server::bodyType &body) {
    std::string line;
    std::string buffer;
    std::string hex;
    size_t bufferSize;

    std::getline(ss, line);
    std::getline(ss, line, '\0');

    while (!line.empty()) {
        hex = line.substr(0, line.find_first_of(CRLF));
        line.erase(0, (hex.size() + 2));
        bufferSize = utils::hexToDec(hex);
        if (bufferSize == 0)
            break;
        buffer.append(line.substr(0, bufferSize));
        line.erase(0, (bufferSize + 2));
    }

    body = buffer;
}

void loadBody(std::istringstream &ss,
              ft::Server::bodyType &body) {
    std::string line;

    std::getline(ss, line);
    body = line;
}

void loadRequest(char *buffer,
                 ft::Server::headerType &header,
                 ft::Server::bodyType &body) {
    std::istringstream ss(buffer);

    loadRequestLine(ss, header);
    loadHeader(ss, header);
    if (isChunkedRequest(header)) {
        loadChunkedBody(ss, body);
        header["Content-Length"] = getBodySize(body);
    } else {
        loadBody(ss, body);
    }
}

void ft::Server::handleConnection(int client_fd, int server_fd) {
    int ret;
    char buffer[8192];  // 8K buffer
    std::map<std::string, std::string> header;
    std::string body;
    ft::Config::Server  *server;

    if (client_fd < 0) {
        perror("Erro ao aceitar o cliente");
        exit(1);
    } else {
        std::cout << "Getting Request" << std::endl;
        ret = recv(client_fd, buffer, sizeof(buffer), 0);
        if (ret < 0) {
            perror("Erro ao receber a mensagem");
            exit(1);
        }
        if (ret == 0)
            return;
        buffer[ret] = '\0';
        loadRequest(buffer, header, body);

        server = getServer(server_fd, &header);
        Method *req = ft::Method::getRequest(header["Method"]);
        std::string res = req->buildResponse(header, body, server);
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
        fd.events = POLLIN | POLLOUT;
        pfds.push_back(fd);
    }
    ret = poll(&pfds[0], pfds.size(), -1);
    if (ret == -1) {
        std::cout << "Error" << std::endl;
    } else if (ret == 0) {
        std::cout << "Timeout" << std::endl;
    }
    for (size_t i = 0; i < pfds.size(); i++) {
        if (pfds[i].revents & POLLIN || pfds[i].revents & POLLOUT) {
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

        std::cout << "Server Listening on Port: " << port << std::endl;
        sockets.push_back(socket_fd);
        for (size_t i = 0; i < it->second.size(); i++) {
            this->_config.server.at(it->first).at(i).fd = socket_fd;
        }
    }
    return (sockets);
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
