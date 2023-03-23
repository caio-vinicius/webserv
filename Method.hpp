/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef METHOD_HPP_
#define METHOD_HPP_

#include <string>
#include <map>
#include <iostream>

#include "./Response.hpp"

namespace ft {
class Method {
 public:
    Method() {}
    static Method *getRequest(std::string method);
    virtual ft::Response buildResponse(std::map<std::string,
        std::string> header) = 0;
    ~Method() {}
};

class Get : public Method {
 public:
    Get() {}
    ft::Response buildResponse(std::map<std::string, std::string> header);
    ~Get() {}
};

class Post : public Method {
 public:
    Post() {}
    ft::Response buildResponse(std::map<std::string, std::string> header);
    ~Post() {}
};

class Delete : public Method {
 public:
    Delete() {}
    ft::Response buildResponse(std::map<std::string, std::string> header);
    ~Delete() {}
};

}  // namespace ft

#endif  // METHOD_HPP_
