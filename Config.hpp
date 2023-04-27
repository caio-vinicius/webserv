/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace ft {

class Config {
 public:
    class Server;

    Config() {}
    ~Config() {}
    typedef std::map<std::string, std::vector<Server> >::iterator iterator;
    typedef std::map<std::string, std::vector<Server> > servers;
    servers server;

    void parse(std::string path);
    void parseServer(std::ifstream &file);
    void parseLocation(std::ifstream &file,
        std::string &location_line, Server &server);
};  // class Config

class Config::Server {
 public:
    Server();
    ~Server() {}

    std::map<std::string, void(ft::Config::Server::*)
        (std::vector<std::string> &)> params;
    void processListen(std::vector<std::string> &params);
    void processServerName(std::vector<std::string> &param);
    void processErrorPage(std::vector<std::string> &param);
    void processClientMaxBodySize(std::vector<std::string> &param);
    void processRoot(std::vector<std::string> &param);
    void processIndex(std::vector<std::string> &param);

    class Location;
    struct address_port {
        std::string address;
        u_int16_t port;
    };

    struct error_page_t {
        std::vector<u_int16_t> code;
        std::string path;
    };

    std::map<std::string, Location> location;
    std::vector<address_port> listen;
    typedef std::vector<address_port> listens;
    std::vector<std::string> server_name;
    error_page_t error_page;
    size_t client_max_body_size;
    std::string root;
    std::vector<std::string> index;
    int fd;
    bool default_server;
};  // Config::Server

class Config::Server::Location {
 public:
    Location();
    ~Location() {}

    void processAutoindex(std::vector<std::string> &param);
    void processUri(std::vector<std::string> &param);
    std::map<std::string, void(ft::Config::Server::Location::*)
        (std::vector<std::string> &)> params;

    std::string uri;
    bool autoindex;
};  // class Config::Server::Location

}  // namespace ft

#endif  // CONFIG_HPP_
