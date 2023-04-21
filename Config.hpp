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
    Config() {}
    ~Config() {}
    class Server;
    std::map<std::string, std::vector<Server> > server;
    void parse(std::string path);
    void parseServer(std::ifstream &file);
    void parseLocation(std::ifstream &file, std::string &location_line, Server &server);
};  // class Config

class Config::Server {
 public:
    Server();
    ~Server() {};

    std::string getRawPort() {
        std::stringstream ss;
        ss << listen.front().port;
        return ss.str();
    };
    class Location;
    std::map<std::string, void(ft::Config::Server::*)(std::vector<std::string> &)> params;
    std::map<std::string, Location> location;
    struct address_port {
        std::string address;
        u_int16_t port;
    };
    std::vector<address_port> listen;
    std::vector<std::string> server_name;
    struct error_page_t {
        std::vector<u_int16_t> code;
        std::string path;
    };
    error_page_t error_page;
    size_t client_max_body_size;
    std::string root;
    std::vector<std::string> index;

    void processListen(std::vector<std::string> &params);
    void processServerName(std::vector<std::string> &param);
    void processErrorPage(std::vector<std::string> &param);
    void processClientMaxBodySize(std::vector<std::string> &param);
    void processRoot(std::vector<std::string> &param);
    void processIndex(std::vector<std::string> &param);
 private:
};  // Config::Server

class Config::Server::Location {
 public:
    Location();
    ~Location() {};

    void processAutoindex(std::vector<std::string> &param);
    void processUri(std::vector<std::string> &param);
    std::map<std::string, void(ft::Config::Server::Location::*)(std::vector<std::string> &)> params;

    std::string uri;
    bool autoindex;
 private:
};  // class Config::Server::Location

}  // namespace ft

#endif  // CONFIG_HPP_
