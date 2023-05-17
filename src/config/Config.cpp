/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Config.hpp"
#include "../../includes/Utils.hpp"

void ft::Config::parse(std::string path) {
    std::ifstream file(path.c_str());

    if (!file.is_open())
        ft::webservEmergError("Config file not found");

    std::cout << "Loading Server Config" << std::endl;

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
        if (param.size() == 0)
            continue;
        else if (param.size() == 1)
            ft::webservEmergError("syntax error");
        it = current_server.params.find(param[0]);
        if (it != current_server.params.end())
            (current_server.*(it->second))(param);
        else
            ft::webservEmergError("param not found: '" + param[0] + "'");
    }
    ft::webservEmergError("unexpected end of file, '}' missing");
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
            return;
        }
        utils::trim(&token);
        param = utils::split(&token, ' ');
        it = current_location.params.find(param[0]);
        if (it != current_location.params.end())
            (current_location.*(it->second))(param);
        else
            ft::webservEmergError("param not found: '" + param[0] + "'");
    }
    ft::webservEmergError("unexpected end of file, '}' missing");
}

void ft::webservEmergError(std::string cause) {
    std::cerr << "webserv: [emerg] " << cause << std::endl;
    exit(EXIT_FAILURE);
}
