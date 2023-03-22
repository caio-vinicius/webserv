/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

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

ft::Response ft::Get::buildResponse(std::map<std::string, std::string> header) {
    if (header.find("Host") == header.end()) {
        return Response("400", "Bad Request", "HTTP/1.1", "", "");
    }
    return Response("200", "OK", "HTTP/1.1", "", "");
}
