/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <poll.h>
#include <arpa/inet.h>
#include <vector>
#include "./Config.hpp"

namespace ft {

class Server {
 public:
    Server(void);
    explicit Server(Config config);
    virtual ~Server();
    void run();

 private:
    Config _config;
    std::vector<int> _sockets;
    std::vector<struct pollfd> _pollfds;
    std::vector<struct sockaddr_in> _sockaddrs;
};  // class Server

}  // namespace ft

#endif  // SERVER_HPP_
