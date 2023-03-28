/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>
#include <sstream>
#include <string>

#include "./Method.hpp"

ft::Method *ft::Method::getRequest(std::string method) {
    if (!method.compare("GET"))
        return new Get();
    else if (!method.compare("POST"))
        return new Post();
    else if (!method.compare("DELETE"))
        return new Delete();
    return NULL;
}

std::string openAndReadFile(std::string filename) {
    std::ifstream file(filename.c_str());
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}

ft::Config::Server  *getHost(std::map<std::string, std::string> header,
    ft::Config *config) {
    std::map<std::string, ft::Config::Server>::iterator it_server;
    std::string host;

    if (header.find("Host") == header.end()) {
        return NULL;
    }
    host = header["Host"];
    it_server = config->server.begin();
    for (; it_server != config->server.end(); it_server++) {
        if (!it_server->first.compare(host)) {
            return &it_server->second;
        }
    }
    return NULL;
}

ft::Config::Location  *getLocation(std::map<std::string, std::string> header,
    ft::Config::Server *server) {
    std::map<std::string, ft::Config::Location>::iterator it_location;
    std::string location;

    location = header["Uri"];
    it_location = server->location.begin();
    for (; it_location != server->location.end(); it_location++) {
        if (!it_location->first.compare(location)) {
            return &it_location->second;
        }
    }
    return NULL;
}

ft::Response ft::Get::buildResponse(
    std::map<std::string, std::string> header,
    ft::Config *config) {
    ft::Config::Server *server;
    ft::Config::Location    *location;

    server = getHost(header, config);
    if (!server)
        return Response("400", "Bad Request", "HTTP/1.1", "", "");

    location = getLocation(header, server);
    if (!location)
        return Response("404", "Not Found", "HTTP/1.1", "", "");

    return Response("200", "OK", "HTTP/1.1", "", "");
}

ft::Response ft::Post::buildResponse(
    std::map<std::string, std::string> header,
    ft::Config *config) {
    if (!header.count("Host")) {
        return Response("400", "Bad Request", "HTTP/1.1", "", "");
    } else if (!header.count("Content-Length")) {
        return Response("411", "Length Required", "HTTP/1.1", "", "");
    }
    (void)config;
    return Response("201", "Created", "HTTP/1.1", "", "");
}

ft::Response ft::Delete::buildResponse(
    std::map<std::string, std::string> header,
    ft::Config *config) {
    if (!header.count("Host")) {
        return Response("400", "Bad Request", "HTTP/1.1", "", "");
    }
    (void)config;
    return Response("202", "Accepted", "HTTP/1.1", "", "");
}
