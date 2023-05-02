/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>
#include <sstream>
#include <string>
//CHECK
#include <algorithm>

#include "./Method.hpp"

ft::Method *ft::Method::getRequest(std::string method) {
    if (!method.compare("GET"))
        return new Get();
    return new Get();
    // else if (!method.compare("POST"))
    //     return new Post();
    // else if (!method.compare("DELETE"))
    //     return new Delete();
    // else
    //     return new MethodNotAllowed();
}


std::string buildHeader(const std::string &buffer, std::string path) {
    std::string extension;
    std::string header;
    std::stringstream   ss;

    extension = path.substr(path.find_last_of(".") + 1);
    std::cout << extension << std::endl;
    std::map <std::string, std::string> mime_types;
    mime_types["html"] = "text/html";
    mime_types["css"] = "text/css";
    mime_types["txt"] = "text/plain";
    mime_types["csv"] = "text/csv";
    mime_types["xml"] = "text/xml";
    mime_types["jpg"] = "image/jpeg";
    mime_types["jpeg"] = "image/jpeg";
    mime_types["png"] = "image/png";
    mime_types["gif"] = "image/gif";
    mime_types["mp3"] = "audio/mpeg";
    mime_types["mp4"] = "video/mp4";
    if (mime_types.count(extension) == 0)
        return "";

    ss << buffer.length();
    header += "Server: 42webserv/1.0\r\n";
    header += "Content-Length: " + ss.str() + "\r\n";
    header += "Content-Type: " + mime_types.at(extension) + "\r\n";
    std::cout << header << "_____________________" << std::endl;
    return (header);
}

ft::Config::Server::Location *getLocation(ft::Config::Server *server,
    std::string uri) {
    std::map<std::string, ft::Config::Server::Location>::iterator locationIt;
    ft::Config::Server::Location *currentLocation;

    locationIt = server->location.begin();
    while (locationIt != server->location.end()) {
        if (uri.find(locationIt->first) == 0) {
            currentLocation = &(locationIt->second);
        }
        locationIt++;
    }
    return (currentLocation);
}


std::string getPath(const std::string &uri,
                    const std::string &root,
                    const std::string &index) {
    std::string path;
    if (!uri.compare("/")) {
        path = "." + root + uri + index;
    } else {
        path = "." + root + uri + "/" + index;
    }
    // if (*(uri.end() - 1) != '/') {
        // uri += "/";
    // }
    //path = "." + root + uri + index;
    return (path);
}

std::string buildBody(const std::string &uri,
                        const std::string &root,
                        const std::string &page,
                        std::string &path) {
    std::stringstream buffer;
    path = getPath(uri, root, page);
    std::ifstream file(path.c_str());
    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }
    return "";
}

std::string createFilePath(std::string root, std::string uri) {
    std::string path;

    path = "." + root + uri;
    return (path);
}

void openFile(std::ifstream&file, std::string uri,
    ft::Config::Server *server,
    ft::Response &res) {

    if (*(uri.end() - 1) != '/') {
        res.setPath(createFilePath(server->root, uri));
        file.open(res.getPath().c_str());
        if (!file.is_open()) {
            uri += "/";
        }
    }
    if (file.is_open() == false) {
        for (size_t i = 0; i < server->index.size(); i++) {
            res.setPath(createFilePath(server->root, uri + server->index[i]));
            file.open(res.getPath().c_str());
            if (file.is_open()) {
                break ;
            }
        }
    }
}

std::string getErrorPage(ft::Config::Server *server, int code) {
    std::string errorPage;
    for (size_t i = 0; i < server->error_page.size(); i++) {
        for (std::set<u_int16_t>::iterator it = server->error_page[i].code.begin();
            it != server->error_page[i].code.end(); it++) {
            if (*it == code) {
                errorPage = server->error_page[i].path;
                break ;
            }
        }
    }
    return (errorPage);
}

void setBodyErrorPage(ft::Config::Server *server, ft::Response& res, int code) {
    std::ifstream file;
    std::stringstream buffer;
    std::string errorPage = getErrorPage(server, code);
    res.setPath(createFilePath(server->root, errorPage));
    if (!errorPage.empty()) {
        file.open(res.getPath().c_str());
    }
    if (file.is_open() == true) {
        buffer << file.rdbuf();
        res.setBody(buffer.str());
        file.close();
    } else {
        res.setBody("<html><body><h1>Pensonalizned body</h1></body></html>");
    }
}

void setBody(ft::Config::Server *server, ft::Response &res, std::ifstream &file) {
    std::stringstream buffer;
    if (file.is_open() == false) {
        res.setStatusLine(HTTP_STATUS_NOT_FOUND);
        setBodyErrorPage(server, res, 404);
    } else {
        buffer << file.rdbuf();
        res.setBody(buffer.str());
        file.close();
    }
}

std::string ft::Get::buildResponse(
    const std::map<std::string, std::string> &header,
    ft::Config::Server *server) {
        ft::Response res;
        std::ifstream file;
        if (server == NULL) {
            res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
            res.setPath(".html");
            res.setBody("<html><body><h1>Bad request</h1></body></html>");
            res.setHeader(buildHeader(res.getBody(), res.getPath()));
            return (res.makeResponse());
        }
        openFile(file, header.at("Uri"), server, res);
        setBody(server, res, file);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
    return res.makeResponse();
}
