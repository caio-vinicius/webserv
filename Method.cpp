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
    else if (!method.compare("POST"))
        return new Post();
    else if (!method.compare("DELETE"))
        return new Delete();
    else
        return new MethodNotAllowed();
}

ft::Response    ft::Method::prepareResponse(
    const std::map <std::string, std::string> &header) {
    if (!header.count("Host") || !header.count("Version")) {
        return (ft::Response(HTTP_STATUS_BAD_REQUEST, "", ""));
    }
    if (header.at("Version").compare("HTTP/1.1")) {
        std::cout << header.at("Version");
        return (ft::Response(HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED, "", ""));
    }
    return (ft::Response(HTTP_STATUS_OK, "", ""));
}

std::string buildHeader(const std::string &buffer, std::string &path) {
    std::string extension;
    std::string header;
    std::stringstream   ss;

    extension = path.substr(path.find_last_of(".") + 1);
    std::cout << path << std::endl;
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
    // Do other stuff
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

std::string getOkBody(ft::Config::Server *server,const std::string &uri, std::string &path) {
    std::string body;
    for (size_t i = 0; i < server->index.size(); i++) {
        body = buildBody(uri, server->root, server->index[i], path);
        if (!body.empty())
            return (body);
    }
    return (body);
}

std::string ImNotOk(ft::Config::Server *server, std::string &path) {
    std::string body;
    body = buildBody("/", server->root, server->error_page.path, path);
    return (body);
}

ft::Response ft::Get::buildResponse(
    const std::map<std::string, std::string> &header,
    ft::Config::Server *server) {
    ft::Response response = prepareResponse(header);
    std::string responseHeader, body, path;
    body = "";
    if (response._status_code_reason_phrase != HTTP_STATUS_OK) {
        return (response);
    }
    ft::Config::Server::Location *location;

    location = getLocation(server, header.at("Uri"));
    if (!location) {
        std::cout << 1 << std::endl;
        body = ImNotOk(server, path);
        responseHeader = buildHeader(body, path);
        return Response(HTTP_STATUS_NOT_FOUND, responseHeader, body);
    }
    body = getOkBody(server, header.at("Uri"), path);
    responseHeader = buildHeader(body, path);
    if (body.empty()) {
        std::cout << 2 << std::endl;
        body = ImNotOk(server, path);
        return Response(HTTP_STATUS_NOT_FOUND, responseHeader, body);
    }

    if (responseHeader.empty()) {
        std::cout << 3 << std::endl;
        body = ImNotOk(server, path);
        responseHeader = buildHeader(body, path);
        return Response(HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, "", body);
    }
    return Response(HTTP_STATUS_OK, responseHeader, body);
}

ft::Response ft::Post::buildResponse(
    const std::map<std::string, std::string> &header,
    ft::Config::Server *server) {
    if (!header.count("Content-Length")) {
        return Response(HTTP_STATUS_LENGTH_REQUIRED, "", "");
    }
    (void)server;
    return Response(HTTP_STATUS_CREATED, "", "");
}

ft::Response ft::Delete::buildResponse(
    const std::map<std::string, std::string> &header,
    ft::Config::Server *server) {
    (void)server;
    (void)header;
    return Response(HTTP_STATUS_ACCEPTED, "", "");
}

ft::Response ft::MethodNotAllowed::buildResponse(
    const std::map<std::string, std::string> &header,
    ft::Config::Server *server) {
    (void)header;
    (void)server;
    return Response(HTTP_STATUS_METHOD_NOT_ALLOWED, "", "");
}
