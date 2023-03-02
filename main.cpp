/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <stdlib.h>
#include <iostream>

#include "Config.hpp"

int main(int argc, char **argv) {
    const char* configFilePath = "./webserv.conf";
    if (argc > 2) {
        exit(EXIT_FAILURE);
    } else if (argc != 1) {
        configFilePath = argv[1];
    }
    ft::Config config(configFilePath);

    // for (std::map<string, string>::iterator it = config.params.begin(); it != config.params.end(); it++)
    //     std::cout << it->first << " " << it->second << std::endl;

    std::cout << config.params["autoindex"] << std::endl;
    std::cout << config.server["localhost:8080;"].params["server_name"] << std::endl;
    std::cout << config.server["localhost:8081;"].params["server_name"] << std::endl;
    std::cout << config.server["localhost:8080;"].location["/"].params["autoindex"] << std::endl;
    std::cout << config.server["localhost:8080;"].location["/"].params["return"] << std::endl;
    std::cout << config.server["localhost:8080;"].location["/test"].params["fastcgi_pass"] << std::endl;
}
