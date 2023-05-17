/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Method.hpp"
#include "../../includes/Response.hpp"
#include "../../includes/Cgi.hpp"

std::string ft::Delete::buildResponse(
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
    if (body.size() > server->client_max_body_size) {
        res.setStatusLine(HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE);
        setBodyErrorPage(server, res, 413);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
        return (res.makeResponse());
    }

    filePath = createFilePath(server->root, header.at("Uri"));
    res.setPath(filePath);
    file.open(filePath.c_str());
    if (file.is_open()) {
        if (std::remove(filePath.c_str())) {
            res.setStatusLine(HTTP_STATUS_INTERNAL_SERVER_ERROR);
            setBodyErrorPage(server, res, 500);
            res.setHeader(buildHeaderDelete(res.getBody(), res.getPath()));
        } else {
            res.setStatusLine(HTTP_STATUS_NO_CONTENT);
            setBodyErrorPage(server, res, 204);
            res.setHeader(buildHeaderDelete(res.getBody(), res.getPath()));
        }
    } else {
        res.setStatusLine(HTTP_STATUS_NOT_FOUND);
        setBodyErrorPage(server, res, 404);
        res.setHeader(buildHeaderDelete(res.getBody(), res.getPath()));
    }
    file.close();
    return (res.makeResponse());
}

std::string ft::Delete::buildHeaderDelete(const std::string &buffer, std::string path) {
    std::string header;

    (void)buffer;
    header += "Server: 42webserv/1.0";
    header += CRLF;
    header += "Emoji: 🐴";
    header += CRLF;
    header += "Location: " + path;
    header += CRLF;
    return (header);
}
