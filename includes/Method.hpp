/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef METHOD_HPP_
#define METHOD_HPP_

#include <dirent.h>
#include <sys/stat.h>

#include "./Response.hpp"
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
    virtual ~Method() {}

 protected:
     ft::Config::Server::Location *getLocation(ft::Config::Server *server,
    std::string uri);
    void setBodyErrorPage(ft::Config::Server *server, ft::Response& res, int code);
    void setBody(ft::Config::Server *server,
             ft::Response &res,
             std::ifstream &file,
             const ft::Server::headerType &header,
             const ft::Server::bodyType &body);
    std::string makeHtml(std::string statusLine);
    std::string buildHeader(const std::string &buffer, std::string path);
    std::string createFilePath(std::string root, std::string uri);
    std::string getErrorPage(ft::Config::Server *server, int code);
    void BuildErrorResponse(ft::Response res, std::string reasonPhrase, ft::Config::Server *server, int statusCode);
};

class Get : public Method {
 public:
    Get() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~Get() {}

 protected:
    std::string getAutoIndex(std::string root, std::string uri);
    bool isDirectory(std::string uri);
    bool isCurrentDirectory(char *name);
    void openFile(std::ifstream &file, std::string uri,
        ft::Config::Server *server,
        ft::Response &res);
};

class Post : public Method {
 public:
    Post() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~Post() {}

 protected:
    std::string postErrorResponse(ft::Config::Server *server, ft::Response *res,
    std::string statusLine, int statusCode);
    void createFile(std::string &path,
                const ft::Server::bodyType &body);
};

class Delete : public Method {
 public:
    Delete() {}
    std::string buildResponse(
        const ft::Server::headerType &header,
        const ft::Server::bodyType &body,
        ft::Config::Server *server);
    ~Delete() {}

 protected:
    std::string deleteErrorResponse(ft::Config::Server *server, ft::Response *res,
            std::string statusLine, int statusCode);
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
