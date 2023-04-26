/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>

namespace utils {

std::string remove_chr(std::string str, char ch);
void trim(std::string &str);
std::vector<std::string> split(std::string &str, char delimiter);
std::string getAddress(std::string listen_t);
int getPort(std::string listen_t);

}  // namespace Utils

#endif  // UTILS_HPP_
