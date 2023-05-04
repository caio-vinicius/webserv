/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <cstddef>
#include <string>

#include "./Config.hpp"

#define CRLF "\r\n"
#define SP ' '

#define HTTP_VERSION "HTTP/1.1"
#define HTTP_STATUS_NULL "000"
#define HTTP_STATUS_OK "200 OK"
#define HTTP_STATUS_CREATED "201 Created"
#define HTTP_STATUS_ACCEPTED "202 Accepted"
#define HTTP_STATUS_NOT_FOUND "404 Not Found"
#define HTTP_STATUS_BAD_REQUEST "400 Bad Request"
#define HTTP_STATUS_NOT_IMPLEMENTED "501 Not Implemented"
#define HTTP_STATUS_INTERNAL_SERVER_ERROR "500 Internal Server Error"
#define HTTP_STATUS_NOT_MODIFIED "304 Not Modified"
#define HTTP_STATUS_MOVED_PERMANENTLY "301 Moved Permanently"
#define HTTP_STATUS_MOVED_TEMPORARILY "302 Moved Temporarily"
#define HTTP_STATUS_SEE_OTHER "303 See Other"
#define HTTP_STATUS_FORBIDDEN "403 Forbidden"
#define HTTP_STATUS_UNAUTHORIZED "401 Unauthorized"
#define HTTP_STATUS_METHOD_NOT_ALLOWED "405 Method Not Allowed"
#define HTTP_STATUS_NOT_ACCEPTABLE "406 Not Acceptable"
#define HTTP_STATUS_REQUEST_TIMEOUT "408 Request Timeout"
#define HTTP_STATUS_CONFLICT "409 Conflict"
#define HTTP_STATUS_GONE "410 Gone"
#define HTTP_STATUS_LENGTH_REQUIRED "411 Length Required"
#define HTTP_STATUS_PRECONDITION_FAILED "412 Precondition Failed"
#define HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE "413 Request Entity Too Large"
#define HTTP_STATUS_REQUEST_URI_TOO_LONG "414 Request-URI Too Long"
#define HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE "415 Unsupported Media Type"
#define HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE \
    "416 Requested Range Not Satisfiable"
#define HTTP_STATUS_EXPECTATION_FAILED "417 Expectation Failed"
#define HTTP_STATUS_UNPROCESSABLE_ENTITY "422 Unprocessable Entity"
#define HTTP_STATUS_LOCKED "423 Locked"
#define HTTP_STATUS_FAILED_DEPENDENCY "424 Failed Dependency"
#define HTTP_STATUS_UPGRADE_REQUIRED "426 Upgrade Required"
#define HTTP_STATUS_INTERNAL_SERVER_ERROR "500 Internal Server Error"
#define HTTP_STATUS_NOT_IMPLEMENTED "501 Not Implemented"
#define HTTP_STATUS_BAD_GATEWAY "502 Bad Gateway"
#define HTTP_STATUS_SERVICE_UNAVAILABLE "503 Service Unavailable"
#define HTTP_STATUS_GATEWAY_TIMEOUT "504 Gateway Timeout"
#define HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED "505 HTTP Version Not Supported"
#define HTTP_STATUS_INSUFFICIENT_STORAGE "507 Insufficient Storage"

namespace ft {
class Response {
 public:
        Response(void);
        void setHeader(std::string header);
        void setBody(std::string body);
        void setStatusLine(std::string status_line);
        void setStatusCode(std::string status_code);
        void setPath(std::string path);
        std::string getHeader(void);
        std::string getBody(void);
        std::string getStatusLine(void);
        std::string getStatusCode(void);
        std::string getPath(void);
        std::string getDefaultErrorPage(std::string status_code);
        std::string makeResponse(void);
        ~Response(void);
 private:
        std::string _path;
        std::string _header;
        std::string _body;
        std::string _status_line;
        std::string _status_code;
};  // Response

}  // namespace ft

#endif  // RESPONSE_HPP_
