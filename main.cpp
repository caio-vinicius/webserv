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

    if (argc > 2) {
        exit(EXIT_FAILURE);
    } else if (argc != 1) {
        configFilePath = argv[1];
    }
    ft::Config config(configFilePath);
    ft::Server server(config);
    server.run();
}
