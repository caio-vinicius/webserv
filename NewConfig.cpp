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

    std::vector<std::string> param;
    param.push_back("listen");
    param.push_back("localhost:80");
    param.push_back("localhost:8000");
    processListen(param);

    param.clear();
    param.push_back("server_name");
    param.push_back("");
    processServerName(param);

    param.clear();
    param.push_back("client_max_body_size");
    param.push_back("1m");
    processClientMaxBodySize(param);

    param.clear();
    param.push_back("root");
    param.push_back("html");
    processRoot(param);
}

ft::NewConfig::Server::Location::Location() {
    this->params["uri"] = &Location::processUri;
    this->params["autoindex"] = &Location::processAutoindex;

    std::vector<std::string> param;
    param.push_back("uri");
    param.push_back("/");
    processUri(param);

    param.clear();
    param.push_back("autoindex");
    param.push_back("off");
    processAutoindex(param);
}

void ft::NewConfig::Server::processListen(std::vector<std::string> &param) {
    struct address_port listen;
    std::vector<std::string>::iterator it;
    std::vector<std::string> address_port;
    this->listen.clear();

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
    this->server_name.clear();

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
    this->index.clear();

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
            return;
        }
        ft::trim(token);
        param = ft::split(token, ' ');
        if (param.size() == 0) {
            continue;
        }
        it = current_server.params.find(param[0]);
        if (it != current_server.params.end()) {
            (current_server.*(it->second))(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << \
                param[0] << "'" << std::endl;
        }
    }
    std::cerr << "webserv: [emerg] syntax error server not closed" << std::endl;
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
