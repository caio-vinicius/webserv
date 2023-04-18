/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./NewConfig.hpp"
#include <fstream>
#include <sstream>

void ft::NewConfig::Server::processListen(std::vector<std::string> &param) {
    std::cout << "Listen found" << std::endl;
}

void processServerName(std::vector<std::string> &param) {
    std::cout << "ServerName found" << std::endl;
}

void processErrorPage(std::vector<std::string> &param) {
    std::cout << "ErrorPage found" << std::endl;
}

void processClientMaxBodySize(std::vector<std::string> &param) {
    std::cout << "ClientMaxBodySize found" << std::endl;
}

void processRoot(std::vector<std::string> &param) {
    std::cout << "Root found" << std::endl;
}

void processIndex(std::vector<std::string> &param) {
    std::cout << "Index found" << std::endl;
}

void processUri(std::vector<std::string> &param) {
    std::cout << "Uri found" << std::endl;
    //this->uri = param[1];
}

void processAutoindex(std::vector<std::string> &param) {
    std::cout << "Autoindex found" << std::endl;
    // TODO: implement verification if param[1] exists
    //this->autoindex = param[1];
}

ft::NewConfig::NewConfig() {
    this->server_params["listen"] = &processListen;
    this->server_params["server_name"] = &processServerName;
    this->server_params["error_page"] = &processErrorPage;
    this->server_params["client_max_body_size"] = &processClientMaxBodySize;
    this->server_params["root"] = &processRoot;
    this->server_params["index"] = &processIndex;
    this->location_params["uri"] = &processLocationUri;
    this->location_params["autoindex"] = &processLocationAutoindex;
    std::map<std::string, void(*)(std::vector<std::string> &)> location_params;
}


void ft::NewConfig::parseLocation(std::ifstream &file, std::string &location_line) {
    std::string token;
    std::vector<std::string> param;

    ft::trim(location_line);
    param = ft::split(location_line, ' ');
    this->location_params.at("uri")(param);
    while (std::getline(file, token, '\n'))
    {
        if (token.find("}") != std::string::npos) {
            break;
        }
        ft::trim(token);
        param = ft::split(token, ' ');
        std::map<std::string, void(*)(std::vector<std::string> &)>::iterator it = this->location_params.find(param[0]);
        if (it != this->location_params.end()) {
            it->second(param);
        } else {
            std::cerr << "webserv: [emerg] param not found: '" << param[0] << "'" << std::endl;
        }
    }
}

void ft::NewConfig::parseServer(std::ifstream &file) {
    std::string token;
    std::vector<std::string> param;

    while (getline(file, token, '\n')) {
        if (token.find("location") != std::string::npos && \
            token.find("{") != std::string::npos) {
            this->parseLocation(file, token);
            continue ;
        }
        if (token.find("}") != std::string::npos) {
            break ;
        }
        ft::trim(token);
        param = ft::split(token, ' ');
        std::map<std::string, void(*)(std::vector<std::string> &)>::iterator it = this->server_params.find(param[0]);
        if (it != this->server_params.end()) {
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
