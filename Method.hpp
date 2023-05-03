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

class get : public Method {
 public:
    get() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~get() {}
};

class post : public Method {
 public:
    post() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~post() {}
};

// class Delete : public Method {
//  public:
//     Delete() {}
//     std::string buildResponse(
//         const ft::Server::headerType &header,
//         const ft::Server::bodyType &body,
//         ft::Config::Server *server);
//     ~Delete() {}
// };

class methodNotAllowed : public Method {
  public:
     methodNotAllowed() {}
     std::string buildResponse(
         const ft::Server::headerType &header,
         const ft::Server::bodyType &body,
         ft::Config::Server *server);
     ~methodNotAllowed() {}
 };

}  // namespace ft

#endif  // METHOD_HPP_
