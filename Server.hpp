/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <poll.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <string>
#include "./Config.hpp"

namespace ft {

extern bool quit;

class Server {
 public:
    typedef int client_fd;

    Server(void);
    explicit Server(Config config);
    virtual ~Server();
    void run();

 private:
    void handleConnection(int client_fd);
    client_fd waitConnections(void);
    std::map<std::string, std::string> loadHeader(char *buffer);
    void loadBody(char *buffer);
    std::string buildResponse(std::string code, std::string phrase);

    Config _config;
    std::vector<int> _sockets;
    std::vector<struct pollfd> _pollfds;
    std::vector<struct sockaddr_in> _sockaddrs;
};  // class Server

}  // namespace ft

#endif  // SERVER_HPP_
