/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Config.hpp"
#include "../../includes/Utils.hpp"

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

void ft::Config::Server::Location::processUri(
    std::vector<std::string> &param) {
    param.pop_back();
    this->uri = param.back();
}

void ft::Config::Server::Location::processAutoindex(
    std::vector<std::string> &param) {
    std::string autoindex;

    autoindex = param.back();
    if (autoindex == "on" && param.size() == 2) {
        this->autoindex = true;
    } else {
        this->autoindex = false;
    }
}
