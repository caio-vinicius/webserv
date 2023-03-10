/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <set>
#include "Config.hpp"
#include <poll.h>

namespace ft {

class Server {
 public:
    Server(void);
    Server(Config config);
    virtual ~Server();
    void run();
    int getPort();

 private:
    Config _config;
    std::set<int> _sockets;
    struct pollfd *_pfds;

}; // class Server

} // namespace ft

#endif
