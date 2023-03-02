/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>
#include <map>

typedef std::string string;
namespace ft {

class Location {
 public:
    Location(void) {};

    Location(const Location& obj) {
		*this = obj;
	}

    Location&    operator=(const Location& obj) {
        this->params = obj.params;

        return *this;
    }

    std::map<string, string> params;
};

class Server {
 public:
    Server(void) {};

    Server(const Server& obj) {
		*this = obj;
	}

    Server&    operator=(const Server& obj) {
        this->params = obj.params;
        this->location = obj.location;

        return *this;
    }

    std::map<string, string> params;
    std::map<string, Location> location;
};

class Config {
 public:
    std::map<string, string> params;
    std::map<string, Server> server;
    std::string _filename;

    Config(void){};
    explicit Config(std::string const filename);
    ~Config(void);

    void load(std::ifstream &configFile);
    void httpContext(std::ifstream &configFile);
    ft::Server serverContext(std::ifstream &configFile);
    ft::Location locationContext(std::ifstream &configFile);
};  // class Config

}  // namespace ft

#endif // CONFIG_HPP_
