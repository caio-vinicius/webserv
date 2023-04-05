/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <poll.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "./Config.hpp"
#include "./Utils.hpp"


ft::Config::Config(std::string const filename) : _filename(filename) {
    std::map<std::string, std::string> conf;

    std::ifstream configFile(this->_filename.c_str());

    if (!configFile.is_open()) {
        exit(-1);
    }
    this->load(configFile);
    std::cout << "Config file loaded" << std::endl;
}

ft::Config::~Config() {}

ft::Config::Location    ft::Config::locationContext(std::ifstream &configFile) {
    std::string line, key, value;
    ft::Config::Location   location = ft::Config::Location();

    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        if (!line.empty()) {
            if (line.find("}") != std::string::npos) {
                return location;
            } else {
                lineStream >> key >> value;
                value = remove_chr(value, ';');
                // checar se param é válido
                location.params[key] = value;
            }
        }
    }
    std::cout << "ERROR!! Missing }:" << line << std::endl;
    exit(-1);
}

ft::Config::Server     ft::Config::serverContext(std::ifstream &configFile) {
    std::string line, context, path, key, value;
    ft::Config::Server    server = ft::Config::Server();
    Location    location = Location();

    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        if (!line.empty()) {
            if (line.find("location") != std::string::npos) {
                lineStream >> context >> path;
                server.location[path] = locationContext(configFile);
            } else if (line.find("}") != std::string::npos) {
                return server;
            } else {
                lineStream >> key >> value;
                // checar se param é válido
                value = remove_chr(value, ';');
                server.params[key] = value;
            }
        }
    }
    std::cout << "ERROR!! Missing }:" << line << std::endl;
    exit(-1);
}
void    ft::Config::httpContext(std::ifstream &configFile) {
    std::string line, key, value;;
    ft::Config::Server  server = Server();


    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        if (!line.empty()) {
            if (line.find("server") != std::string::npos) {
                server = serverContext(configFile);
                this->server[server.params["listen"]] = server;
            } else if (line.find("}") != std::string::npos) {
                return;
            } else {
                lineStream >> key >> value;
                // checar se param é válido
                value = remove_chr(value, ';');
                this->params[key] = value;
            }
        }
    }
    std::cout << "ERROR!! Missing }:" << line << std::endl;
    exit(-1);
}

void ft::Config::load(std::ifstream &configFile) {
    std::string line;

    while (std::getline(configFile, line)) {
        if (!line.empty()) {
            if (line.find("http") == std::string::npos) {
                std::cout << "ERROR!! On line:" << line << std::endl;
                return;
            }
            httpContext(configFile);
        }
    }
    return;
}

ft::Config::Server const *ft::Config::getServer(std::string const host) const {
    std::map<std::string, ft::Config::Server>::const_iterator it_server;

    it_server = this->server.begin();
    for (; it_server != this->server.end(); it_server++) {
        if (!it_server->first.compare(host)) {
            return (&it_server->second);
        }
    }
    return NULL;
}

ft::Config::Location const *ft::Config::getLocation(std::string uri,
    const ft::Config::Server &server) const {
    std::map<std::string, ft::Config::Location>::const_iterator it_location;

    it_location = server.location.begin();
    for (; it_location != server.location.end(); it_location++) {
        if (!it_location->first.compare(uri)) {
            return (&it_location->second);
        }
    }
    return NULL;
}
