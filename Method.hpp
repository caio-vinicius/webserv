/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef METHOD_HPP_
#define METHOD_HPP_

#include <string>
#include <map>
#include <iostream>

#include "./Response.hpp"
#include "./Config.hpp"

namespace ft {
class Method {
 public:
    Method() {}
    static Method *getRequest(std::string method);
    ft::Response prepareResponse(const std::map <std::string, \
        std::string> &header);
    virtual ft::Response buildResponse(
        const std::map<std::string, std::string> &header,
        ft::Config  &config) = 0;
    ~Method() {}
};

class Get : public Method {
 public:
    Get() {}
    ft::Response buildResponse(
        const std::map<std::string, std::string> &header,
        ft::Config &config);
    ~Get() {}
};

class Post : public Method {
 public:
    Post() {}
    ft::Response buildResponse(
        const std::map<std::string, std::string> &header,
        ft::Config &config);
    ~Post() {}
};

class Delete : public Method {
 public:
    Delete() {}
    ft::Response buildResponse(
        const std::map<std::string, std::string> &header,
        ft::Config &config);
    ~Delete() {}
};

class MethodNotAllowed : public Method {
 public:
    MethodNotAllowed() {}
    ft::Response buildResponse(
        const std::map<std::string, std::string> &header,
        ft::Config &config);
    ~MethodNotAllowed() {}
};

}  // namespace ft

#endif  // METHOD_HPP_
