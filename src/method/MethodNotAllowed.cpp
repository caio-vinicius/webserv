/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Method.hpp"
#include "../../includes/Response.hpp"
#include "../../includes/Cgi.hpp"

std::string ft::MethodNotAllowed::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;

    (void)header;
    (void)body;
    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        res.setBody(makeHtml(res.getStatusLine()));
        res.setHeader(buildHeader(res.getBody(), ".html"));
        return (res.makeResponse());
    }
    res.setStatusLine(HTTP_STATUS_METHOD_NOT_ALLOWED);
    setBodyErrorPage(server, res, 405);
    res.setHeader(buildHeader(res.getBody(), res.getPath()));
    return (res.makeResponse());
}
