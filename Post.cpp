/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <fstream>

#include "./Method.hpp"
#include "./Response.hpp"
#include "./Cgi.hpp"

std::string ft::Post::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;
    std::string filePath;
    std::ifstream file;

    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        res.setBody(makeHtml(res.getStatusLine()));
        res.setHeader(buildHeader(res.getBody(), ".html"));
        return (res.makeResponse());
    }
    if (header.count("Content-Length") == 0) {
        return postErrorRespose(server, res,HTTP_STATUS_LENGTH_REQUIRED, 411);
    }
    if (body.size() > server->client_max_body_size) {
        return postErrorRespose(server, res,
            HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE, 413);
    }

    filePath = createFilePath(server->root, header.at("Uri"));
    res.setPath(filePath);
    file.open(filePath.c_str());
    if (file.is_open()) {
        if (res.getPath().find(".py") != std::string::npos) {
            res.setStatusLine(HTTP_STATUS_CREATED);
        } else {
            res.setStatusLine(HTTP_STATUS_SEE_OTHER);
        }
        setBody(server, res, file, header, body);
        res.setBody("");
        res.setHeader(buildHeaderPost(res.getBody(),  header.at("Uri")));
    } else {
        try {
            createFile(filePath, body);
            res.setStatusLine(HTTP_STATUS_CREATED);
            setBodyErrorPage(server, res, 201);
            res.setHeader(buildHeaderPost(res.getBody(),  header.at("Uri")));
        } catch (std::exception &e) {
            res.setStatusLine(HTTP_STATUS_INTERNAL_SERVER_ERROR);
            setBodyErrorPage(server, res, 500);
            res.setHeader(buildHeader(res.getBody(),  header.at("Uri")));
        }
    }
    file.close();
    return (res.makeResponse());
}

std::string ft::Post::buildHeaderPost(const std::string &buffer, std::string path) {
    std::string header;

    (void)buffer;
    header += "Server: 42webserv/1.0";
    header += CRLF;
    header += "Location: " + path;
    header += CRLF;
    return (header);
}

std::string ft::Post::postErrorRespose(ft::Config::Server *server, ft::Response res,
    std::string statusLine, int statusCode) {
    res.setStatusLine(statusLine);
    setBodyErrorPage(server, res, statusCode);
    res.setHeader(buildHeader(res.getBody(), res.getPath()));
    return (res.makeResponse());
}

void ft::Post::createFile(std::string &path,
                const ft::Server::bodyType &body) {
    std::ofstream newFile;

    newFile.open(path.c_str());
    if (newFile.is_open()) {
        newFile << body;
        newFile.close();
    } else {
        throw std::exception();
    }
}

