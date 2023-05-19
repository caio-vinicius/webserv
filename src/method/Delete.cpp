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
    ft::Config::Server::Location *location;

    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        res.setBody(makeHtml(res.getStatusLine()));
        res.setHeader(buildHeader(res.getBody(), ".html"));
        return (res.makeResponse());
    }
    if (body.size() > server->client_max_body_size) {
        return (errorResponse(server, &res,
            HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE, 413));
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
        if (std::remove(filePath.c_str())) {
            errorResponse(server, &res, HTTP_STATUS_INTERNAL_SERVER_ERROR, 500);
        } else {
            errorResponse(server, &res, HTTP_STATUS_NO_CONTENT, 204);
        }
    } else {
        errorResponse(server, &res, HTTP_STATUS_NOT_FOUND, 404);
    }
    file.close();
    return (res.makeResponse());
}
