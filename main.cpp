/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <stdlib.h>

#include <iostream>
#include <csignal>
#include <cstring>

#include "./Config.hpp"
#include "./Server.hpp"

int main(int argc, char **argv) {
    const char* configFilePath = "./webserv.conf";

    //  std::cout << "SIGNAL: " << signal.getSignal(); << std::endl;
    //  signal.setSignal(false);

    if (argc > 2) {
        exit(EXIT_FAILURE);
    } else if (argc != 1) {
        configFilePath = argv[1];
    }
    ft::Config config(configFilePath);

    std::cout << config.params["autoindex"] << std::endl;
    std::cout << config.server["localhost:8080"].params["server_name"]\
        << std::endl;
    std::cout << config.server["localhost:8081"].params["server_name"]\
        << std::endl;
    std::cout << config.server["localhost:8080"].location["/"].params\
        ["autoindex"] << std::endl;
    std::cout << config.server["localhost:8080"].location["/"].params\
        ["return"] << std::endl;
    std::cout << config.server["localhost:8080"].location["/test"].params\
        ["fastcgi_pass"] << std::endl;
    std::cout << config.params["root"] << std::endl;
    std::cout << config.server["localhost:8080"].params["root"] << std::endl;

    ft::Server server(config);
    server.run();
}
