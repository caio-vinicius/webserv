/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef NEWCONFIG_HPP_
#define NEWCONFIG_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace ft {

class NewConfig {
 public:
    NewConfig() {};
    ~NewConfig() {};
    class Server;
    std::map<std::string, Server> server;
    void parse(std::string path);
    void parseServer(std::ifstream &file);
    void parseLocation(std::ifstream &file, std::string &location_line, ft::NewConfig::Server *server);
    void createServerMap(ft::NewConfig::Server &server);
};  // class NewConfig

class NewConfig::Server {
 public:
    Server();
    ~Server() {};

    class Location;
    struct address_port {
        std::string address;
        int port;
    };

    std::vector<std::string> getRawAddress();
 private:
    std::vector<address_port> listen;
    std::vector<std::string> raw_address;
    std::vector<std::string> server_name;
    struct error_page {
        std::vector<int> code;
        std::string path;
    };
    size_t client_max_body_size;
    std::string root;
    std::string index;
    std::map<std::string, Location> location;
    std::map<std::string, void(ft::NewConfig::Server::*)(std::vector<std::string> &)> serverParams;

    //friend class NewConfig;
    void processListen(std::vector<std::string> &params);
    void processServerName(std::vector<std::string> &param);
    void processErrorPage(std::vector<std::string> &param);
    void processClientMaxBodySize(std::vector<std::string> &param);
    void processRoot(std::vector<std::string> &param);
    void processIndex(std::vector<std::string> &param);
};  // NewConfig::Server

class NewConfig::Server::Location {
 public:
    Location();
    ~Location() {};

    std::map<std::string, void(ft::NewConfig::Server::Location::*)(std::vector<std::string> &)> locationParams;
 private:
    std::string uri;
    enum autoindex {on, off};

    void processAutoindex(std::vector<std::string> &param);
    void processUri(std::vector<std::string> &param);
};  // class NewConfig::Server::Location

// external trim
void trim(std::string &str) {
    str.erase(0, str.find_first_not_of("\t\n\v\f\r "));
    str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1);
}

// external split
std::vector<std::string> split(std::string &str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;

    while (getline(iss, token, delimiter))
        result.push_back(token);
    return (result);
}

}  // namespace ft

#endif  // NEWCONFIG_HPP_
