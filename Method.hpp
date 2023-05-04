/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef METHOD_HPP_
#define METHOD_HPP_

#include <string>
#include <map>
#include <iostream>
#include <cstring>

#include "./Response.hpp"
#include "./Config.hpp"
#include "./Server.hpp"

namespace ft {
class Method {
 public:
    Method() {}
    static Method *getRequest(std::string method);
    ft::Response prepareResponse(const std::map <std::string, \
        std::string> &header);
    virtual std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server) = 0;
    ~Method() {}
};

class Get : public Method {
 public:
    Get() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~Get() {}
};

class Post : public Method {
 public:
    Post() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~Post() {}
};

class Delete : public Method {
 public:
    Delete() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~Delete() {}
};

class MethodNotAllowed : public Method {
 public:
     MethodNotAllowed() {}
     std::string buildResponse(
         const ft::Server::headerType &header,
         const ft::Server::bodyType &body,
         ft::Config::Server *server);
     ~MethodNotAllowed() {}
};

}  // namespace ft

#endif  // METHOD_HPP_
