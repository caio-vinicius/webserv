/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>
#include <map>

typedef std::string string;
namespace ft {

class Config {
 private:
    class Location {
     private:
        std::map<string, string> params;
     public:
        Location(void) {}
    };

    class Server {
     private:
        std::map<string, string> params;
        std::map<string, Location> location;
     public:
        Server(void) {}
    };

    std::map<string, string> params;
    std::map<string, Server> server;
    std::string _filename;

 public:
    Config(void) {}
    explicit Config(std::string const filename): _filename(filename) {}
    void addServer(void) {}
    ~Config(void) {}
};  // class Config

}  // namespace ft

#endif  // CONFIG_HPP_
