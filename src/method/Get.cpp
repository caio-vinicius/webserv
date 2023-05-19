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
    ft::Config::Server::Location *location;

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
    location = getLocation(server, header.at("Uri"));
    if (location->allowedMethods.count(header.at("Method")) == 0) {
        res.setStatusLine(HTTP_STATUS_METHOD_NOT_ALLOWED);
        setBodyErrorPage(server, res, 405);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
        return (res.makeResponse());
    }
    if (!location->redirection.empty()) {
        res.setStatusLine(HTTP_STATUS_MOVED_PERMANENTLY);
        res.setHeader(addRedirectionLocation(location->redirection));
        return (res.makeResponse());
    }
    if (location->autoindex == true && isDirectory(header.at("Uri"))) {
        res.setStatusLine(HTTP_STATUS_OK);
        res.setBody(getAutoIndex(server->root, header.at("Uri")));
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
        return (res.makeResponse());
    }
    try {
        openFile(file, header.at("Uri"), server, res);
        setBody(server, res, file, header, body);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
    } catch (std::exception &e) {
        res.setStatusLine(HTTP_STATUS_INTERNAL_SERVER_ERROR);
        setBodyErrorPage(server, res, 500);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
    }
    return (res.makeResponse());
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
            } else if (uri.compare("/") == 0) {
                ss << "<a href=\"" << uri << ent->d_name <<
                    "\">" << ent->d_name << "</a><br>";
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
    struct stat fileStat;

    res.setPath(createFilePath(server->root, uri));
    stat(res.getPath().c_str(), &fileStat);

    if( fileStat.st_mode & S_IFDIR ) {
        if (uri.at(uri.size() - 1) != '/')
            uri += "/";
        for (size_t i = 0; i < server->index.size(); i++) {
            res.setPath(createFilePath(server->root, uri + server->index[i]));
            file.open(res.getPath().c_str());
            if (file.is_open()) {
                break;
            }
        }
    } else if( fileStat.st_mode & S_IFREG )
        file.open(res.getPath().c_str());
    else
        throw std::exception();
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
