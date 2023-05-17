/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../../includes/Method.hpp"
#include "../../includes/Response.hpp"
#include "../../includes/Cgi.hpp"

ft::Method *ft::Method::getRequest(std::string method) {
    if (!method.compare("GET"))
        return (new Get());
    else if (!method.compare("POST"))
        return (new Post());
    else if (!method.compare("DELETE"))
        return new Delete();
    return (new MethodNotAllowed());
}

std::string ft::Method::makeHtml(std::string statusLine) {
    std::string html;
    std::stringstream ss;

    ss << "<html><head><title>" << \
    statusLine << "</title></head><body><center><h1>" << \
    statusLine << "</h1></center><hr><center>42webserv/1.0</center></body></html>";
    html = ss.str();
    return (html);
}

std::string ft::Method::buildHeader(const std::string &buffer, std::string path) {
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
    header += "Emoji: 🐴";
    header += CRLF;
    header += "Content-Length: " + ss.str();
    header += CRLF;
    if (mime_types.count(extension) != 0) {
        header += "Content-Type: " + mime_types.at(extension);
        header += CRLF;
    }
    return (header);
}

void ft::Method::setBody(ft::Config::Server *server,
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

void ft::Method::setBodyErrorPage(ft::Config::Server *server, ft::Response& res, int code) {
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

std::string ft::Method::createFilePath(std::string root, std::string uri) {
    std::string path;

    path = root + uri;
    return (path);
}

std::string ft::Method::getErrorPage(ft::Config::Server *server, int code) {
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
