/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Method.hpp"
#include "../../includes/Response.hpp"
#include "../../includes/Cgi.hpp"

std::string ft::Get::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;
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

    ft::Config::Server::Location *location;
    location = getLocation(server, header.at("Uri"));
    if (location->autoindex == true && isDirectory(header.at("Uri"))) {
        res.setStatusLine(HTTP_STATUS_OK);
        res.setBody(getAutoIndex(server->root, header.at("Uri")));
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
        return (res.makeResponse());
    }
    openFile(file, header.at("Uri"), server, res);
    setBody(server, res, file, header, body);
    res.setHeader(buildHeader(res.getBody(), res.getPath()));
    return (res.makeResponse());
}

ft::Config::Server::Location *ft::Get::getLocation(ft::Config::Server *server,
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

std::string ft::Get::getAutoIndex(std::string root, std::string uri) {
    std::string path = createFilePath(root, uri);

    DIR *dir;
    struct dirent *ent;
    std::stringstream ss;

    if ((dir = opendir(path.c_str())) != NULL) {
        ss << "<html><head><title>Index of " << uri <<
            "</title></head><body><h1>Index of " << uri << "</h1><hr><pre>";
        while ((ent = readdir(dir)) != NULL) {
            if (isCurrentDirectory(ent->d_name)) {
               ss << "<a href=\"" << uri << "\">" << ent->d_name << "</a><br>";
            } else {
                ss << "<a href=\"" << uri << "/" << ent->d_name <<
                    "\">" << ent->d_name << "</a><br>";
            }
        }
        ss << "</pre><hr><center>42webserv/1.0</center></body></html>";
        closedir(dir);
    }
    return (ss.str());
}

void ft::Get::openFile(std::ifstream &file,
    std::string uri,
    ft::Config::Server *server,
    ft::Response &res) {

    if (*(uri.end() - 1) != '/') {
        res.setPath(createFilePath(server->root, uri));
        file.open(res.getPath().c_str());
        if (!file.is_open()) {
            uri += "/";
        }
    }
    if (file.is_open() == false) {
        for (size_t i = 0; i < server->index.size(); i++) {
            res.setPath(createFilePath(server->root, uri + server->index[i]));
            file.open(res.getPath().c_str());
            if (file.is_open()) {
                break;
            }
        }
    }
}

bool ft::Get::isDirectory(std::string uri) {
    if (uri.find(".") != std::string::npos)
        return false;
    return true;
}

bool ft::Get::isCurrentDirectory(char *name) {
    if (name[0] == '.' && name[1] == '\0')
        return true;
    return false;
}
