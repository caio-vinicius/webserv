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

std::string getPath(//const std::map <std::string, std::string> &header,
                    const std::string &uri,
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

std::string openAndReadFile(ft::Config::Server *server,
                    //const std::map<std::string, std::string> &header,
                    const std::string &uri,
                    std::string &path) {
    std::stringstream buffer;

    std::vector<std::string>::const_iterator it = server->index.begin();
    for (; it != server->index.end(); ++it) {
        path = getPath(uri, server->root, *it);
        std::ifstream file(path.c_str());
        if (file.is_open()) {
            buffer << file.rdbuf();
            file.close();
            return (buffer.str());
        }
    }
    return ("");
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

ft::Response makeResponse(const char *status_code_reason_phrase,
                    const std::string &uri,
                    ft::Config::Server *server)
{
    std::string body, path, header, code;
    int code_int;

    std::cout << "error page code: " << server->error_page.code[0] << std::endl;
    std::cout << "error page path: " << server->error_page.path << std::endl;

    std::string code_phrase = std::string(status_code_reason_phrase);
    code = code_phrase.substr(0, code_phrase.find(" "));
    //phrase = code_phrase.substr(code_phrase.find(" ") + 1);
    code_int = std::atoi(code.c_str());

    if (std::find(server->error_page.code.begin(), server->error_page.code.end(), code_int) != server->error_page.code.end()) {
        path = "." + server->root + "/" + server->error_page.path;
        std::cout << "path: " << path << std::endl;
        std::ifstream file(path.c_str());
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            body = buffer.str();
        }
    } else {
        body = openAndReadFile(server, uri, path);
    }

    header = buildHeader(body, path);

    return ft::Response(status_code_reason_phrase, header, body);
}

ft::Response ft::Get::buildResponse(
    const std::map<std::string, std::string> &header,
    ft::Config::Server *server) {
    ft::Response response = prepareResponse(header);

    return (makeResponse(HTTP_STATUS_OK, header.at("Uri"), server));

    if (response._status_code_reason_phrase != HTTP_STATUS_OK)
        return (response);

    ft::Config::Server::Location *location;
    std::string responseHeader, buffer, path;

    location = getLocation(server, header.at("Uri"));
    if (!location)
        return Response(HTTP_STATUS_NOT_FOUND, "", "");

    buffer = openAndReadFile(server, header.at("Uri"), path);
    if (buffer.empty())
        return Response(HTTP_STATUS_NOT_FOUND, responseHeader, "");

    responseHeader = buildHeader(buffer, path);
    if (responseHeader.empty())
        return Response(HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, "", "");

    return Response(HTTP_STATUS_OK, responseHeader, buffer);
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
