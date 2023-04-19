/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./NewConfig.hpp"
#include <fstream>
#include <sstream>

ft::NewConfig::Server::Server(){
    this->serverParams["listen"] = &Server::processListen;
    this->serverParams["server_name"] = &Server::processServerName;
    this->serverParams["error_page"] = &Server::processErrorPage;
    this->serverParams["client_max_body_size"] = &Server::processClientMaxBodySize;
    this->serverParams["root"] = &Server::processRoot;
    this->serverParams["index"] = &Server::processIndex;
}

ft::NewConfig::Server::Location::Location(){
    this->locationParams["uri"] = &Location::processUri;
    this->locationParams["autoindex"] = &Location::processAutoindex;
}

void ft::NewConfig::Server::processListen(std::vector<std::string> &param) {
    std::vector<std::string>::iterator paramIterator = param.begin();
    std::vector<std::string>    address;
    address_port    structAddressPort;

    paramIterator++;

    while (paramIterator != param.end()) {
        this->raw_address.push_back(*paramIterator);
        address = ft::split(*paramIterator, ':');
        //if (address.size != 2) ERRO;
        structAddressPort.address = address[0];
        structAddressPort.port = std::atoi(address[1].c_str());
        listen.push_back(structAddressPort);
    }
    std::cout << "Listen found" << std::endl;
}

void ft::NewConfig::Server::processServerName(std::vector<std::string> &param) {
    std::cout << "ServerName found" << std::endl;
}

void ft::NewConfig::Server::processErrorPage(std::vector<std::string> &param) {
    std::cout << "ErrorPage found" << std::endl;
}

void ft::NewConfig::Server::processClientMaxBodySize(std::vector<std::string> &param) {
    std::cout << "ClientMaxBodySize found" << std::endl;
}

void ft::NewConfig::Server::processRoot(std::vector<std::string> &param) {
    std::cout << "Root found" << std::endl;
}

void ft::NewConfig::Server::processIndex(std::vector<std::string> &param) {
    std::cout << "Index found" << std::endl;
}

void ft::NewConfig::Server::Location::processUri(std::vector<std::string> &param) {
    std::cout << "Uri found" << std::endl;
    //this->uri = param[1];
}

void ft::NewConfig::Server::Location::processAutoindex(std::vector<std::string> &param) {
    std::cout << "Autoindex found" << std::endl;
    // TODO: implement verification if param[1] exists
    //this->autoindex = param[1];
}

void ft::NewConfig::parseLocation(std::ifstream &file, std::string &location_line, ft::NewConfig::Server *server) {
    std::string token;
    std::vector<std::string> param;
    std::map<std::string, void(ft::NewConfig::Server::Location::*)(std::vector<std::string> &)>::iterator it;
    ft::NewConfig::Server::Location   locationClass = ft::NewConfig::Server::Location();

    ft::trim(location_line);
    param = ft::split(location_line, ' ');
    it = locationClass.locationParams.find("uri");
    it->second(param);
    while (std::getline(file, token, '\n'))
    {
        if (token.find("}") != std::string::npos) {
            break;
        }
        ft::trim(token);
        param = ft::split(token, ' ');
        it = this->location_params.find(param[0]);
        if (it != this->location_params.end()) {
            it->second(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << param[0] << "'" << std::endl;
        }
    }
}

void ft::NewConfig::createServerMap(ft::NewConfig::Server &server) {
    std::vector<std::string>::iterator  addressIterator;

    addressIterator = server.getRawAddress().begin();
    while (addressIterator != server.getRawAddress().end()) {
        this->server[*addressIterator] = server;
    }
}

void ft::NewConfig::parseServer(std::ifstream &file) {
    std::string token;
    std::vector<std::string> param;
    std::map<std::string, void(ft::NewConfig::Server::Location::*)(std::vector<std::string> &)>::iterator it;
    ft::NewConfig::Server   serverClass = ft::NewConfig::Server();

    while (getline(file, token, '\n')) {
        if (token.find("location") != std::string::npos && \
            token.find("{") != std::string::npos) {
            this->parseLocation(file, token, &serverClass);
            continue ;
        }
        if (token.find("}") != std::string::npos) {
            this->server[serverClass.getListen().at(0)] = serverClass;
            createServerMap(&serverClass);
            break ;
        }
        ft::trim(token);
        param = ft::split(token, ' ');
        it = serverClass.serverParams.find(param[0]);
        if (it != serverClass.serverParams.end()) {
            it->second(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << param[0] << "'" << std::endl;
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
//    config->server["localhost:8080"]->listen[0].address; // address of the server
//    config->server["localhost:8084"]->listen[0].address; // address of the server
//    config->server["localhost:8080"]->listen[0].port; // port of the server
//    config->server["localhost:8084"]->listen[0].port; // port of the server
//    config->server["localhost:8080"]->server_name; // server_name of the server
//    config->server["localhost:8080"]->error_page.code; // vector of codes
//    config->server["localhost:8080"]->error_page.path; // error page path
//    config->server["localhost:8080"]->client_max_body_size; // max body size server
//    config->server["localhost:8080"]->root; // root path of the server
//    config->server["localhost:8080"]->index; // index of the server
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
