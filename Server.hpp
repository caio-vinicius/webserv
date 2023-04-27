/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <poll.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "./Config.hpp"
#include "./Utils.hpp"

namespace ft {

extern bool quit;

class Server {
 public:
    typedef int client_fd;
    typedef std::map<std::string, std::string> header_type;

    Server(void);
    explicit Server(Config config);
    virtual ~Server();
    void run();

 private:
    void waitConnections(int *client_fd, int *server_fd);
    void handleConnection(int client_fd, int server_fd);
    header_type loadHeader(char *buffer);
    void loadBody(char *buffer);
    ft::Config::Server *getServer(int server_fd, header_type *header);
    std::string buildResponse(std::string code, std::string phrase);
    std::string getAddressByName(std::string name);
    std::vector<int> createSockets(Config::servers server);

    Config _config;
    std::vector<int> _sockets;
};  // class Server

}  // namespace ft

#endif  // SERVER_HPP_
