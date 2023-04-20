/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>
#include <sstream>
#include <string>

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
        return (ft::Response(HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED, "", ""));
    }
    return (ft::Response(HTTP_STATUS_OK, "", ""));
}

std::string openAndReadFile(ft::Config::Server const *server, const std::map<std::string, std::string> &header) {
    std::string filename, index;
    std::string root = server->root;
    std::ifstream file;
    std::stringstream buffer;

    for (size_t i = 0; i < server->index.size(); i++) {
        index = server->index.at(i);
        if (!header.at("Uri").compare("/")) {
            filename = root + header.at("Uri") + index;
        } else {
            filename = root + header.at("Uri") + "/" + index;
        }
        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            buffer << file.rdbuf();
            file.close();
            return buffer.str();
        }
    }
    return "";
}

std::string buildHeader(const std::string &buffer, std::string path) {
    std::string extension;
    std::string header;
    std::stringstream   ss;

    extension = path.substr(path.find_last_of(".") + 1);
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
    header += "Content-Lenght: " + ss.str() + "\r\n";
    header += "Content-Type: " + mime_types.at(extension) + "\r\n";
    // Do other stuff
    std::cout << header << "_____________________" << std::endl;
    return header;
}

ft::Response ft::Get::buildResponse(
    const std::map<std::string, std::string> &header,
    const ft::Config &config) {
    ft::Response response = prepareResponse(header);
    if (response._status_code_reason_phrase != HTTP_STATUS_OK)
        return (response);

    ft::Config::Server const *server;
    ft::Config::Server::Location const *location;
    std::string root, file, path, responseHeader, buffer;

    server = &config.server.at(header.at("Host"));
    if (!server)
        return Response(HTTP_STATUS_BAD_REQUEST, "", "");

    location = &server->location.at(header.at("Uri"));
    if (!location)
        return Response(HTTP_STATUS_NOT_FOUND, "", "");

    buffer = openAndReadFile(server, header);

    responseHeader = buildHeader(buffer, path);
    if (buffer.empty())
        return Response(HTTP_STATUS_NOT_FOUND, responseHeader, "");

    if (responseHeader.empty())
        return Response(HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, "", "");
    return Response(HTTP_STATUS_OK, responseHeader, buffer);
}

ft::Response ft::Post::buildResponse(
    const std::map<std::string, std::string> &header,
    const ft::Config &config) {
    if (!header.count("Host")) {
        return Response(HTTP_STATUS_BAD_REQUEST, "", "");
    } else if (!header.count("Content-Length")) {
        return Response(HTTP_STATUS_LENGTH_REQUIRED, "", "");
    }
    (void)config;
    return Response(HTTP_STATUS_CREATED, "", "");
}

ft::Response ft::Delete::buildResponse(
    const std::map<std::string, std::string> &header,
    const ft::Config &config) {
    if (!header.count("Host")) {
        return Response(HTTP_STATUS_BAD_REQUEST, "", "");
    }
    (void)config;
    return Response(HTTP_STATUS_ACCEPTED, "", "");
}

ft::Response ft::MethodNotAllowed::buildResponse(
    const std::map<std::string, std::string> &header,
    const ft::Config &config) {
    (void)header;
    (void)config;
    return Response(HTTP_STATUS_METHOD_NOT_ALLOWED, "", "");
}
