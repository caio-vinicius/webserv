/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./NewConfig.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>

ft::NewConfig::Server::Server() {
    this->params["listen"] = &Server::processListen;
    this->params["server_name"] = &Server::processServerName;
    this->params["error_page"] = &Server::processErrorPage;
    this->params["client_max_body_size"] = &Server::processClientMaxBodySize;
    this->params["root"] = &Server::processRoot;
    this->params["index"] = &Server::processIndex;
}

ft::NewConfig::Server::Location::Location() {
    this->params["uri"] = &Location::processUri;
    this->params["autoindex"] = &Location::processAutoindex;
}

void ft::NewConfig::Server::processListen(std::vector<std::string> &param) {
    struct address_port listen;
    std::vector<std::string>::iterator it;
    std::vector<std::string> address_port;

    it = param.begin();
    it++;
    while (it != param.end()) {
        address_port = ft::split(*it, ':');
        listen.address = address_port[0];
        listen.port = std::atoi(address_port[1].c_str());
        this->listen.push_back(listen);
        it++;
    }
}

void ft::NewConfig::Server::processServerName(std::vector<std::string> &param) {
    std::vector<std::string>::iterator it;

    it = param.begin();
    it++;
    while (it != param.end()) {
        this->server_name.push_back(*it);
        it++;
    }
}

void ft::NewConfig::Server::processErrorPage(std::vector<std::string> &param) {
    std::vector<std::string>::iterator it;

    this->error_page.path = param.back();
    param.pop_back();
    it = param.begin();
    it++;
    while (it != param.end()) {
        this->error_page.code.push_back(std::atoi(it->c_str()));
        it++;
    }
}

void ft::NewConfig::Server::processClientMaxBodySize(
    std::vector<std::string> &param) {
    std::string body_size;

    body_size = param.back();
    if (body_size.find("k") != std::string::npos) {
        body_size.erase(body_size.find("k"), 1);
        this->client_max_body_size = std::atoi(body_size.c_str()) * 1024;
    } else if (body_size.find("m") != std::string::npos) {
        body_size.erase(body_size.find("m"), 1);
        this->client_max_body_size = std::atoi(body_size.c_str()) * 1024 * 1024;
    } else {
        this->client_max_body_size = std::atoi(body_size.c_str());
    }
}

void ft::NewConfig::Server::processRoot(std::vector<std::string> &param) {
    this->root = param.back();
}

void ft::NewConfig::Server::processIndex(std::vector<std::string> &param) {
    std::vector<std::string>::iterator it;

    it = param.begin();
    it++;
    while (it != param.end()) {
        this->index.push_back(*it);
        it++;
    }
}

void ft::NewConfig::Server::Location::processUri(
    std::vector<std::string> &param) {
    param.pop_back();
    this->uri = param.back();
}

void ft::NewConfig::Server::Location::processAutoindex(
    std::vector<std::string> &param) {
    std::string autoindex;

    autoindex = param.back();
    if (autoindex == "on") {
        this->autoindex = true;
    } else {
        this->autoindex = false;
    }
}

void ft::NewConfig::parseLocation(
    std::ifstream &file,
    std::string &location_line, ft::NewConfig::Server &server) {
    std::string token;
    std::vector<std::string> param;
    std::map
        <std::string,
        void(ft::NewConfig::Server::Location::*)\
        (std::vector<std::string> &)>::iterator it;
    ft::NewConfig::Server::Location current_location = \
        ft::NewConfig::Server::Location();

    ft::trim(location_line);
    param = ft::split(location_line, ' ');
    current_location.processUri(param);
    while (std::getline(file, token, '\n')) {
        if (token.find("}") != std::string::npos) {
            server.location[current_location.uri] = current_location;
            break;
        }
        ft::trim(token);
        param = ft::split(token, ' ');
        it = current_location.params.find(param[0]);
        if (it != current_location.params.end()) {
            (current_location.*(it->second))(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << \
                param[0] << "'" << std::endl;
        }
    }
}

void ft::NewConfig::parseServer(std::ifstream &file) {
    std::string token;
    std::vector<std::string> param;
    ft::NewConfig::Server current_server = ft::NewConfig::Server();
    std::map
        <std::string,
        void(ft::NewConfig::Server::*)(std::vector<std::string> &)>
        ::iterator it;

    while (getline(file, token, '\n')) {
        if (token.find("location") != std::string::npos && \
            token.find("{") != std::string::npos) {
            this->parseLocation(file, token, current_server);
            continue;
        }
        if (token.find("}") != std::string::npos) {
            std::vector<NewConfig::Server::address_port>::iterator it;

            it = current_server.listen.begin();
            while (it != current_server.listen.end()) {
                std::stringstream ss;
                std::string port;
                ss << it->port;
                this->server[it->address + ":" + ss.str()] = current_server;
                it++;
            }
            break;
        }
        ft::trim(token);
        param = ft::split(token, ' ');
        it = current_server.params.find(param[0]);
        if (it != current_server.params.end()) {
            (current_server.*(it->second))(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << \
                param[0] << "'" << std::endl;
        }
    }
}

void ft::NewConfig::parse(std::string path) {
    std::ifstream file(path.c_str());

    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        while (getline(iss, token, '\n')) {
            if (token.find("server") != std::string::npos && \
                token.find("{") != std::string::npos) {
                parseServer(file);
            }
        }
    }
}

int main(void) {
    ft::NewConfig config;
    config.parse("webserv.conf");

//    // nivel 1 config
//
//    // nivel 2 config::server
//
//    config.server["localhost:8080"].listen[0].address; // address of the server
    std::cout << "server: " << config.server["localhost:8080"].listen[0].address << std::endl;
//    config->server["localhost:8084"]->listen[0].address; // address of the server
    std::cout << "server: " << config.server["localhost:8084"].listen[0].address << std::endl;
//    config->server["localhost:8080"]->listen[0].port; // port of the server
    std::cout << "server: " << config.server["localhost:8080"].listen[0].port << std::endl;
//    config->server["localhost:8084"]->listen[0].port; // port of the server
    std::cout << "server: " << config.server["localhost:8084"].listen[0].port << std::endl;
//    config->server["localhost:8080"]->server_name; // server_name of the server
    std::cout << "server: " << config.server["localhost:8080"].server_name[0] << std::endl;
//    config->server["localhost:8080"]->error_page.code; // vector of codes
    std::cout << "server: " << config.server["localhost:8080"].error_page.code[0] << std::endl;
//    config->server["localhost:8080"]->error_page.path; // error page path
    std::cout << "server: " << config.server["localhost:8080"].error_page.path << std::endl;
//    config->server["localhost:8080"]->client_max_body_size; // max body size server
    std::cout << "server: " << config.server["localhost:8080"].client_max_body_size << std::endl;
//    config->server["localhost:8080"]->root; // root path of the server
    std::cout << "server: " << config.server["localhost:8080"].root << std::endl;
//    config->server["localhost:8080"]->index; // index of the server
    std::cout << "server: " << config.server["localhost:8080"].index[0] << std::endl;
//
//    it_server = config->server.begin();
//    ite_server = config->server.end();
//    while (it_server != ite_server) {
//        it_server->second->listen.address; // address of the server
//    }
//
//    // nivel 3 config::server::location
//
//    config->server["localhost:8080"]->location["/"]->uri; // uri of the location
//    config->server["localhost:8080"]->location["/"]->autoindex; // autoindex of the location
//
//    it_location = config->server["localhost:8080"]->location.begin();
//    ite_location = config->server["localhost:8080"]->location.end();
//    while (it_location != ite_location) {
//        it_location->second->uri; // uri of the location
//    }
}
