/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef METHOD_HPP_
#define METHOD_HPP_

#include <string>
#include <map>
#include <iostream>

namespace ft {
class Method {
 public:
    Method() {}
    static Method *getRequest(std::string method);
    virtual std::string buildResponse(std::map<std::string,
        std::string> header) = 0;
    ~Method() {}
};

class Get : public Method {
 public:
    Get() {}
    std::string buildResponse(std::map<std::string, std::string> header) {
        std::string response;
        std::cout << "GET" << std::endl;
        return  ("HTTP/1.1 200 OK\n\n");
    }
    ~Get() {}
};
class Post : public Method {
 public:
    Post() {}
    std::string buildResponse(std::map<std::string, std::string> header) {
        std::string response;
        std::cout << "POST" << std::endl;
        return ("HTTP/1.1 200 OK\n\n");
    }
    ~Post() {}
};
class Delete : public Method {
 public:
    Delete() {}
    std::string buildResponse(std::map<std::string, std::string> header) {
        std::string response;
        std::cout << "DELETE" << std::endl;
        return  ("HTTP/1.1 200 OK\n\n");
    }
    ~Delete() {}
};
}  // namespace ft

#endif  // METHOD_HPP_
