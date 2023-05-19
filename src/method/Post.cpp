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

    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        res.setBody(makeHtml(res.getStatusLine()));
        res.setHeader(buildHeader(res.getBody(), ".html"));
        return (res.makeResponse());
    }
    if (header.count("Content-Length") == 0) {
        return postErrorResponse(server, &res, HTTP_STATUS_LENGTH_REQUIRED, 411);
    }
    if (body.size() > server->client_max_body_size) {
        return postErrorResponse(server, &res,
            HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE, 413);
    }

    filePath = createFilePath(server->root, header.at("Uri"));
    res.setPath(filePath);
    file.open(filePath.c_str());
    if (file.is_open()) {
        if (res.getPath().find(".py") != std::string::npos ||
            res.getPath().find(".pl") != std::string::npos) {
            postErrorResponse(server, &res, HTTP_STATUS_CREATED, 201);
        } else {
            postErrorResponse(server, &res, HTTP_STATUS_SEE_OTHER, 303);
        }
    } else {
        try {
            createFile(filePath, body);
            postErrorResponse(server, &res, HTTP_STATUS_CREATED, 201);
        } catch (std::exception &e) {
            postErrorResponse(server, &res,
                HTTP_STATUS_INTERNAL_SERVER_ERROR, 500);
        }
    }
    file.close();
    return (res.makeResponse());
}

std::string ft::Post::postErrorResponse(ft::Config::Server *server, ft::Response *res,
    std::string statusLine, int statusCode) {
    res->setStatusLine(statusLine);
    setBodyErrorPage(server, *res, statusCode);
    if (server->error_page.empty())
        res->setHeader(buildHeader(res->getBody(), ".html"));
    else
        res->setHeader(buildHeader(res->getBody(), server->error_page.at(0).path));
    return (res->makeResponse());
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
