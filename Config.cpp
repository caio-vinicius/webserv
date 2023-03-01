/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>

#include "Config.hpp"

#define CONTEXTLESS 0
#define HTTP 1
#define SERVER 2
#define LOCATION 3

static bool ifhttp(std::string line, int &context)
{
    if (line.find("http") != std::string::npos)
    {
        context = HTTP;
        return (true);
    }
    return (false);
}

static bool ifserver(std::string line, int &context)
{
    if (line.find("server") != std::string::npos)
    {
        context = SERVER;
        return (true);
    }
    return (false);
}

static bool iflocation(std::string line, int &context)
{
    if (line.find("location") != std::string::npos)
    {
        context = LOCATION;
        return (true);
    }
    return (false);
}

static void load(std::ifstream &configFile, std::string context)
{
    std::string line;

    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string paramName;
        std::string paramValue;

        if (line.find(context) != std::string::npos)
        {
            while (std::getline(configFile, line))
            {
                if (line.find("}") != std::string::npos)
                    break;
                if (line.find("server") != std::string::npos) {
                    while (std::getline(configFile, line))
                }
                paramName = line.substr(0, line.find(" "));
                paramValue = line.substr(line.find(" ") + 1, line.length());
                std::cout << paramName << " " << paramValue << std::endl;
            }
        }
    }
}

ft::Config::Config(std::string const filename): _filename(filename) {
    std::map<std::string, std::string> conf;

    std::ifstream configFile(this->_filename.c_str());

    if (!configFile.is_open()) {
        exit(EXIT_FAILURE);
    }
    load(configFile, "http");
    std::cout << "Config file loaded" << std::endl;
}

ft::Config::~Config() {}
