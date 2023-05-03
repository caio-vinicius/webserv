/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Response.hpp"

ft::Response::Response(void): _header(""), _body(""), _status_line(HTTP_STATUS_OK) {}
ft::Response::~Response(void) {}

std::string ft::Response::getHeader(void) {
    return this->_header;
}

std::string ft::Response::getBody(void) {
    return this->_body;
}

std::string ft::Response::getStatusLine(void) {
    return this->_status_line;
}

std::string ft::Response::getStatusCode(void) {
    return this->_status_code;
}

std::string ft::Response::getPath(void) {
    return this->_path;
}

void ft::Response::setHeader(std::string header) {
    this->_header = header;
}

void ft::Response::setBody(std::string body) {
    this->_body = body;
}

void ft::Response::setStatusLine(std::string status_line) {
    this->_status_line = status_line;
}

void ft::Response::setStatusCode(std::string status_code) {
    this->_status_code = status_code;
}

void ft::Response::setPath(std::string path) {
    this->_path = path;
}

std::string ft::Response::makeResponse(void) {
    std::string response = std::string(HTTP_VERSION) + SP + this->getStatusLine() + CRLF;
    response += this->getHeader() + CRLF;
    response += this->getBody();
    std::cout << "Response: " << std::endl << response << std::endl;
    return response;
}

