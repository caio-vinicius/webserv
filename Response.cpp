/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Response.hpp"

ft::Response::Response(
    std::string status_code,
    std::string reason_phrase,
    std::string version = "HTTP/1.1",
    std::string header = "",
    std::string body = ""
) : _status_code(status_code),
    _reason_phrase(reason_phrase),
    _version(version),
    _header(header),
    _body(body) {
    this->_status_line = this->_version + " " + \
                        this->_status_code + " " + \
                        this->_reason_phrase;
    this->_message = this->_status_line + "\n" + \
                        this->_header + "\n" + \
                        this->_body;
}

ft::Response::~Response() {}

std::string ft::Response::message() {
    return (this->_message);
}


