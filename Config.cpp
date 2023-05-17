/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>
#include <sstream>
#include <cstdlib>

#include "./Config.hpp"
#include "./Utils.hpp"

ft::Config::Server::Server() {
    this->params["listen"] = &Server::processListen;
    this->params["server_name"] = &Server::processServerName;
    this->params["error_page"] = &Server::processErrorPage;
    this->params["client_max_body_size"] = &Server::processClientMaxBodySize;
    this->params["root"] = &Server::processRoot;
    this->params["index"] = &Server::processIndex;

    std::vector<std::string> param;
    param.push_back("listen");
    param.push_back("localhost:4242");
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

    this->location["/"] = ft::Config::Server::Location();
    this->default_server = false;
}

ft::Config::Server::Location::Location() {
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

void ft::Config::Server::processListen(std::vector<std::string> &param) {
    struct address_port listen;
    std::vector<std::string>::iterator it;
    std::vector<std::string> address_port;
    this->listen.clear();

    it = param.begin();
    it++;
    while (it != param.end()) {
        address_port = utils::split(&*it, ':');
        listen.address = address_port[0];
        listen.port = std::atoi(address_port[1].c_str());
        this->listen.push_back(listen);
        it++;
    }
}

void ft::Config::Server::processServerName(std::vector<std::string> &param) {
    std::vector<std::string>::iterator it;
    this->server_name.clear();

    it = param.begin();
    it++;
    while (it != param.end()) {
        this->server_name.push_back(*it);
        it++;
    }
}

void ft::Config::Server::processErrorPage(std::vector<std::string> &param) {
    std::vector<std::string>::iterator it;
    ft::Config::Server::error_page_t error_page;
    error_page.path = param.back();
    param.pop_back();
    it = param.begin();
    it++;
    int code = 0;
    while (it != param.end()) {
        code = std::atoi(it->c_str());
        if (code) {
            error_page.code.insert(code);
        }
        it++;
    }
    this->error_page.push_back(error_page);
}

void ft::Config::Server::processClientMaxBodySize(
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

void ft::Config::Server::processRoot(std::vector<std::string> &param) {
    this->root = param.back();
}

void ft::Config::Server::processIndex(std::vector<std::string> &param) {
    std::vector<std::string>::iterator it;
    this->index.clear();

    it = param.begin();
    it++;
    while (it != param.end()) {
        this->index.push_back(*it);
        it++;
    }
}

void ft::Config::Server::Location::processUri(
    std::vector<std::string> &param) {
    param.pop_back();
    this->uri = param.back();
}

void ft::Config::Server::Location::processAutoindex(
    std::vector<std::string> &param) {
    std::string autoindex;

    autoindex = param.back();
    if (autoindex == "on") {
        this->autoindex = true;
    } else {
        this->autoindex = false;
    }
}

void ft::Config::parseLocation(
    std::ifstream &file,
    std::string &location_line, ft::Config::Server &server) {
    std::string token;
    std::vector<std::string> param;
    std::map
        <std::string,
        void(ft::Config::Server::Location::*)\
        (std::vector<std::string> &)>::iterator it;
    ft::Config::Server::Location current_location = \
        ft::Config::Server::Location();

    utils::trim(&location_line);
    param = utils::split(&location_line, ' ');
    current_location.processUri(param);
    while (std::getline(file, token, '\n')) {
        if (token.find("}") != std::string::npos) {
            server.location[current_location.uri] = current_location;
            break;
        }
        utils::trim(&token);
        param = utils::split(&token, ' ');
        it = current_location.params.find(param[0]);
        if (it != current_location.params.end()) {
            (current_location.*(it->second))(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << \
                param[0] << "'" << std::endl;
        }
    }
}

void ft::Config::parseServer(std::ifstream &file) {
    std::string token;
    ft::Config::Server current_server = ft::Config::Server();
    std::map
        <std::string,
        void(ft::Config::Server::*)(std::vector<std::string> &)>
        ::iterator it;

    std::vector<std::string> param;
    while (getline(file, token, '\n')) {
        param.clear();
        if (token.find("location") != std::string::npos && \
            token.find("{") != std::string::npos) {
            this->parseLocation(file, token, current_server);
            continue;
        }
        if (token.find("}") != std::string::npos) {
            std::vector<Config::Server::address_port>::iterator it;

            it = current_server.listen.begin();
            while (it != current_server.listen.end()) {
                std::stringstream ss;
                std::string port;
                ss << it->port;
                this->server[it->address + ":"
                    + ss.str()].push_back(current_server);
                it++;
            }
            return;
        }
        utils::trim(&token);
        param = utils::split(&token, ' ');
        if (param.size() == 0) {
            continue;
        } else if (param.size() == 1) {
            std::cerr << "webserv: [emerg] syntax error" << std::endl;
            exit(EXIT_FAILURE);
        }
        it = current_server.params.find(param[0]);
        if (it != current_server.params.end()) {
            (current_server.*(it->second))(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << \
                param[0] << "'" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
    std::cerr << "webserv: [emerg] syntax error server not closed" << std::endl;
}

void ft::Config::parse(std::string path) {
    std::ifstream file(path.c_str());

    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }

    std::cout << "Loading Server Config" << std::endl;
    if (file.peek() == std::ifstream::traits_type::eof()) {

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
