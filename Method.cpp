/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>
#include <sstream>
#include <string>

#include "./Method.hpp"

#define DEF_PATH "./www"
#define DEF_INDEX "index.html"

ft::Method *ft::Method::getRequest(std::string method) {
    if (!method.compare("GET"))
        return new Get();
    else if (!method.compare("POST"))
        return new Post();
    else if (!method.compare("DELETE"))
        return new Delete();
    return NULL;
}

std::string getPath(const ft::Config::Server &server,
    const ft::Config::Location &location, std::string param, std::string def) {
    std::string str;
    if (location.params.count(param))
        return location.params.at(param);
    else if (server.params.count(param))
        return server.params.at(param);
    return def;
}

std::string openAndReadFile(std::string filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        return ("");
    std::stringstream buffer;

    buffer << file.rdbuf();
    file.close();
    return buffer.str();
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
    ft::Config::Server const *server;
    ft::Config::Location const *location;
    std::string root, file, path, responseHeader, buffer;

    server = config.getServer(header.at("Host"));
    if (!server)
        return Response(HTTP_STATUS_BAD_REQUEST, "", "");

    location = config.getLocation(header.at("Uri"), *server);
    if (!location)
        return Response(HTTP_STATUS_NOT_FOUND, "", "");

    root = getPath(*server, *location, "root", DEF_PATH);
    file = getPath(*server, *location, "index", DEF_INDEX);
    file = getPath(*server, *location, "index", "x");

    if (!header.at("Uri").compare("/")) {
        path = root + header.at("Uri") + file;
    } else {
        path = root + header.at("Uri") + "/" + file;
    }
    std::cout << "PATH: " << path << std::endl;
    buffer = openAndReadFile(path);
    responseHeader = buildHeader(buffer, path);

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
