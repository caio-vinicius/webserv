/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>
#include <sstream>
#include <string>

#include "./Method.hpp"

#define DEF_PATH "./www"
#define DEF_INDEX "index.html"

ft::Method *ft::Method::getRequest(std::string method) {
    if (!method.compare("GET"))
        return new Get();
    else if (!method.compare("POST"))
        return new Post();
    else if (!method.compare("DELETE"))
        return new Delete();
    return NULL;
}

std::string getPath(ft::Config::Server *server, ft::Config::Location *location,
    std::string param, std::string def) {
    std::string str;
    if (location->params.count(param))
        return location->params[param];
    else if (server->params.count(param))
        return server->params[param];
    return def;
}

std::string openAndReadFile(std::string filename) {
    std::ifstream file(filename.c_str());
    std::stringstream buffer;

    buffer << file.rdbuf();
    file.close();
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
    ft::Config::Server      *server;
    ft::Config::Location    *location;
    std::string root, file, path;

    server = getHost(header, config);
    if (!server)
        return Response("400", "Bad Request", "HTTP/1.1", "", "");

    location = getLocation(header, server);
    if (!location)
        return Response("404", "Not Found", "HTTP/1.1", "", "");

    root = getPath(server, location, "root", DEF_PATH);
    file = getPath(server, location, "index", DEF_INDEX);

    if (!header["Uri"].compare("/")) {
        path = root + header["Uri"] + file;
    } else {
        path = root + header["Uri"] + "/" + file;
    }

    return Response("200", "OK", "HTTP/1.1", "",
        openAndReadFile(path).c_str());
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
