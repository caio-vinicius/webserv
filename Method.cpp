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
