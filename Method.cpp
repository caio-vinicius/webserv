/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <dirent.h>

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "./Method.hpp"
#include "./Response.hpp"
#include "./Cgi.hpp"

ft::Method *ft::Method::getRequest(std::string method) {
    if (!method.compare("GET"))
        return (new Get());
    else if (!method.compare("POST"))
        return (new Post());
    else if (!method.compare("DELETE"))
        return new Delete();
    return (new MethodNotAllowed());
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

    ss << buffer.length();
    header += "Server: 42webserv/1.0";
    header += CRLF;
    header += "Content-Length: " + ss.str();
    header += CRLF;
    if (mime_types.count(extension) != 0) {
        header += "Content-Type: " + mime_types.at(extension);
        header += CRLF;
    }
    return (header);
}

std::string buildHeaderPost(const std::string &buffer, std::string path) {
    std::string header;

    (void)buffer;
    header += "Server: 42webserv/1.0";
    header += CRLF;
    header += "Location: " + path;
    header += CRLF;
    return (header);
}

ft::Config::Server::Location *getLocation(ft::Config::Server *server,
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

std::string createFilePath(std::string root, std::string uri) {
    std::string path;

    path = "." + root + uri;
    return (path);
}

void openFile(std::ifstream &file,
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

std::string getErrorPage(ft::Config::Server *server, int code) {
    std::string errorPage;
    for (size_t i = 0; i < server->error_page.size(); i++) {
        for (std::set<u_int16_t>::iterator it = server->error_page[i].
            code.begin(); it != server->error_page[i].code.end(); it++) {
            if (*it == code) {
                errorPage = server->error_page[i].path;
                break;
            }
        }
    }
    return (errorPage);
}

std::string makeHtml(std::string statusLine) {
    std::string html;
    std::stringstream ss;

    ss << "<html><head><title>" << \
    statusLine << "</title></head><body><center><h1>" << \
    statusLine << "</h1></center><hr><center>42webserv/1.0</center></body></html>";
    html = ss.str();
    return (html);
}

void setBodyErrorPage(ft::Config::Server *server, ft::Response& res, int code) {
    std::ifstream file;
    std::stringstream buffer;
    std::string errorPage = getErrorPage(server, code);
    res.setPath(createFilePath(server->root, errorPage));

    if (!errorPage.empty()) {
        file.open(res.getPath().c_str());
    }
    if (file.is_open() == true) {
        buffer << file.rdbuf();
        res.setBody(buffer.str());
        file.close();
    } else {
        res.setBody(makeHtml(res.getStatusLine()));
    }
}

void setBody(ft::Config::Server *server,
             ft::Response &res,
             std::ifstream &file,
             const ft::Server::headerType &header,
             const ft::Server::bodyType &body) {
    std::stringstream buffer;

    if (file.is_open() == false) {
        res.setStatusLine(HTTP_STATUS_NOT_FOUND);
        setBodyErrorPage(server, res, 404);
    } else if (res.getPath().find(".py") != std::string::npos) {
        ft::Cgi cgi(res.getPath(), header, body);
        cgi.run();
        res.setBody(cgi.getResponse());
    } else {
        buffer << file.rdbuf();
        res.setBody(buffer.str());
        file.close();
    }
}

bool isCurrentDirectory(char *name) {
    if (name[0] == '.' && name[1] == '\0')
        return true;
    return false;
}

std::string getAutoIndex(std::string root, std::string uri) {
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

bool isDirectory(std::string uri) {
    if (uri.find(".") != std::string::npos)
        return false;
    return true;
}

std::string ft::Get::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;
    std::ifstream file;

    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        setBodyErrorPage(server, res, 400);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
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

void createFile(std::string &path,
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

std::string postErrorRespose(ft::Config::Server *server, ft::Response res,
    std::string statusLine, int statusCode) {
    res.setStatusLine(statusLine);
    setBodyErrorPage(server, res, statusCode);
    res.setHeader(buildHeader(res.getBody(), res.getPath()));
    return (res.makeResponse());
}

std::string ft::Post::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;
    std::string filePath;
    std::ifstream file;

    if (server == NULL) {
        return postErrorRespose(server, res, HTTP_STATUS_BAD_REQUEST, 400);
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

std::string ft::Delete::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;
    std::string filePath;
    std::ifstream file;

    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        setBodyErrorPage(server, res, 400);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
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
            res.setHeader(buildHeaderPost(res.getBody(), res.getPath()));
        } else {
            res.setStatusLine(HTTP_STATUS_NO_CONTENT);
            setBodyErrorPage(server, res, 204);
            res.setHeader(buildHeaderPost(res.getBody(), res.getPath()));
        }
    } else {
        res.setStatusLine(HTTP_STATUS_NOT_FOUND);
        setBodyErrorPage(server, res, 404);
        res.setHeader(buildHeaderPost(res.getBody(), res.getPath()));
    }
    file.close();
    return (res.makeResponse());
}

std::string ft::MethodNotAllowed::buildResponse(
    const ft::Server::headerType &header,
    const ft::Server::bodyType &body,
    ft::Config::Server *server) {
    ft::Response res;

    (void)header;
    (void)body;
    if (server == NULL) {
        res.setStatusLine(HTTP_STATUS_BAD_REQUEST);
        setBodyErrorPage(server, res, 400);
        res.setHeader(buildHeader(res.getBody(), res.getPath()));
        return (res.makeResponse());
    }
    res.setStatusLine(HTTP_STATUS_METHOD_NOT_ALLOWED);
    setBodyErrorPage(server, res, 405);
    res.setHeader(buildHeader(res.getBody(), res.getPath()));
    return (res.makeResponse());
}
