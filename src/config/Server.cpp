/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Config.hpp"
#include "../../includes/Utils.hpp"

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
    param.push_back("./www");
    processRoot(param);

    this->location["/"] = ft::Config::Server::Location();
    this->default_server = false;
}

void ft::Config::Server::processListen(std::vector<std::string> &param) {
    struct address_port listen;
    std::vector<std::string>::iterator it;
    std::vector<std::string> address_port;
    this->listen.clear();

    it = param.begin();
    it++;
    while (it != param.end()) {
        if (*it == "default_server") {
            this->default_server = true;
            it++;
            continue;
        }
        address_port = utils::split(&*it, ':');
        if (address_port.size() != 2)
            ft::webservEmergError("invalid address and port " + *it);
        listen.address = address_port[0];
        listen.port = std::atoi(address_port[1].c_str());
        if (listen.port == 0)
            ft::webservEmergError("invalid address and port " + address_port[1]);
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
        if (code && (code >= 300 && code <= 599)) {
            error_page.code.insert(code);
        } else
            ft::webservEmergError("value " + *it + " must be between 300 and 599");
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
