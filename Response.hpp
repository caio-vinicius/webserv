/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <cstddef>
#include <string>

namespace ft {
class Response {
 public:
        Response(std::string status_code,
                    std::string reason_phrase,
                    std::string version,
                    std::string header,
                    std::string body);
        ~Response();
        std::string message(void);
        std::string statusCode(void);
        std::string header(void);
        std::string body(void);
 private:
        std::string _status_code;
        std::string _reason_phrase;
        std::string _version;
        std::string _header;
        std::string _body;
        std::string _message;
        std::string _status_line;
};  // Response

}  // namespace ft

#endif  // RESPONSE_HPP_
