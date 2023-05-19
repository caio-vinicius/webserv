/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Config.hpp"
#include "../../includes/Utils.hpp"

ft::Config::Server::Location::Location() {
    this->params["uri"] = &Location::processUri;
    this->params["allowed_methods"] = &Location::processAllowedMethods;
    this->params["autoindex"] = &Location::processAutoindex;

    std::vector<std::string> param;
    param.push_back("uri");
    param.push_back("/");
    processUri(param);

    param.clear();
    param.push_back("allowed_methods");
    param.push_back("GET");
    param.push_back("POST");
    param.push_back("DELETE");
    processAllowedMethods(param);

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

void ft::Config::Server::Location::processAllowedMethods(std::vector<std::string> &param) {
    this->allowedMethods.clear();

    for(size_t i = 1; i < param.size(); i++) {
        if (param.at(i).compare("GET") == 0 ||
            param.at(i).compare("POST") == 0 ||
            param.at(i).compare("DELETE") == 0)
            this->allowedMethods.insert(param.at(i));
        else
            ft::webservEmergError("Method " + param.at(i) +
                " is not allowed, only GET, POST and DELETE are allowed");
    }
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
