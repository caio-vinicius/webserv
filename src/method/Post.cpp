/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Method.hpp"
#include "../../includes/Response.hpp"
#include "../../includes/Cgi.hpp"

std::string ft::Post::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;
    std::string filePath;
    std::ifstream file;
    ft::Config::Server::Location *location;

    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        res.setBody(makeHtml(res.getStatusLine()));
        res.setHeader(buildHeader(res.getBody(), ".html"));
        return (res.makeResponse());
    }
    if (header.count("Content-Length") == 0) {
        return errorResponse(server, &res, HTTP_STATUS_LENGTH_REQUIRED, 411);
    }
    if (body.size() > server->client_max_body_size) {
        return errorResponse(server, &res,
            HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE, 413);
    }
    location = getLocation(server, header.at("Uri"));
    if (location->allowedMethods.count(header.at("Method")) == 0) {
        return (errorResponse(server, &res,
            HTTP_STATUS_METHOD_NOT_ALLOWED, 405));
    }
    if (!location->redirection.empty()) {
        res.setStatusLine(HTTP_STATUS_MOVED_PERMANENTLY);
        res.setHeader(addRedirectionLocation(location->redirection));
        return (res.makeResponse());
    }

    filePath = createFilePath(server->root, header.at("Uri"));
    res.setPath(filePath);
    file.open(filePath.c_str());
    if (file.is_open()) {
        if (res.getPath().find(".py") != std::string::npos ||
            res.getPath().find(".pl") != std::string::npos) {
            errorResponse(server, &res, HTTP_STATUS_CREATED, 201);
        } else {
            errorResponse(server, &res, HTTP_STATUS_SEE_OTHER, 303);
        }
    } else {
        try {
            createFile(filePath, body);
            errorResponse(server, &res, HTTP_STATUS_CREATED, 201);
        } catch (std::exception &e) {
            errorResponse(server, &res,
                HTTP_STATUS_INTERNAL_SERVER_ERROR, 500);
        }
    }
    file.close();
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
