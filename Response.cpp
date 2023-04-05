/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Response.hpp"

#define CRLF "\r\n"

ft::Response::Response(
    const char *status_code_reason_phrase,
    std::string header = "",
    std::string body = ""
) : _status_code_reason_phrase(status_code_reason_phrase),
    _header(header),
    _body(body) {
    this->_status_line = std::string(HTTP_VERSION) + " " + \
                        this->_status_code_reason_phrase;
    this->_message = this->_status_line + CRLF + \
                        this->_header + CRLF + \
                        this->_body;
}

ft::Response::~Response() {}

std::string ft::Response::message() {
    return (this->_message);
}
