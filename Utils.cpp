/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Utils.hpp"

namespace utils {

std::string remove_chr(std::string str, char ch) {
    for (std::string::iterator it = str.begin(); it != str.end(); ) {
        if (*it == ch) {
            it = str.erase(it);
        } else {
            ++it;
        }
    }
    return (str);
}

void trim(std::string *str) {
    str->erase(0, str->find_first_not_of("\t\n\v\f\r "));
    str->erase(str->find_last_not_of("\t\n\v\f\r ") + 1);
}

std::vector<std::string> split(std::string *str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(*str);
    std::string token;

    while (getline(iss, token, delimiter))
        result.push_back(token);
    return (result);
}


std::string getAddress(std::string listen_t) {
    std::string ip;
    size_t pos = listen_t.find(":");
    if (pos != std::string::npos) {
        ip = listen_t.substr(0, pos);
    }
    return (ip);
}

int getPort(std::string listen_t) {
    int n;
    std::string port;
    size_t pos = listen_t.find(":");
    if (pos != std::string::npos) {
        port = listen_t.substr(pos + 1);
    }
    std::istringstream stream(port);
    stream >> n;
    return (n);
}


}  // namespace utils
