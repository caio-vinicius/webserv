/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <string>
#include <map>



namespace ft {
    class Method {
        Method *getRequest(std::string method) {
        }
        virtual std::string buildResponse(std::map<std::string, std::string> header) = 0;
    };

    class Get : public Method {
        std::string buildResponse(map<std::string, std::string> header) {
            std::string response;
            response += "HTTP/1.1 200 OK\r";
        }
    };

    class Post : public Method {
        std::string buildResponse(map<std::string, std::string> header) {
            std::string response;
            response += "HTTP/1.1 200 OK\r";
        }
    };

    class Delete : public Method {
        std::string buildResponse(map<std::string, std::string> header) {
            std::string response;
            response += "HTTP/1.1 200 OK\r";
        }
    };
}

#endif
