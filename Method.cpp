/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>
#include <sstream>

#include "./Method.hpp"
#include <string>

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

ft::Response ft::Get::buildResponse(
    std::map<std::string, std::string> header,
    ft::Config *config) {
    if (header.find("Host") == header.end()) {
        return Response("400", "Bad Request", "HTTP/1.1", "", "");
    }
    std::string html = openAndReadFile("./lenzo_gotico.html");
    return Response("200", "OK", "HTTP/1.1", "", html);
}

ft::Response ft::Post::buildResponse(
    std::map<std::string, std::string> header,
    ft::Config *config) {
    if (!header.count("Host")) {
        return Response("400", "Bad Request", "HTTP/1.1", "", "");
    } else if (!header.count("Content-Length")) {
        return Response("411", "Length Required", "HTTP/1.1", "", "");
    }
    return Response("201", "Created", "HTTP/1.1", "", "");
}

ft::Response ft::Delete::buildResponse(
    std::map<std::string, std::string> header,
    ft::Config *config) {
    if (!header.count("Host")) {
        return Response("400", "Bad Request", "HTTP/1.1", "", "");
    }
    return Response("202", "Accepted", "HTTP/1.1", "", "");
}
