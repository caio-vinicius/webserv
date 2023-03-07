/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <vector>
#include "Config.hpp"

namespace ft {

class Server {
 public:
    Server(void);
    Server(Config config);
    virtual ~Server();
    void run();

 private:
    Config _config;
    struct pollfd *pfds;

}; // class Server

} // namespace ft

#endif
