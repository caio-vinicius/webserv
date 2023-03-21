/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <string>
#include <iterator>

#include "./Utils.hpp"

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
