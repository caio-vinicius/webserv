#include <Config.hpp>
#include <fstream>
#include <

ft::Config::Config(std::string const filename) : _filename(filename) {
    std::map<std::string, std::string> conf;

    std::ifstream configFile(this->_filename.c_str());

    if (!configFile.is_open()) {
        exit(-1);
    }
    this->load(configFile);
    std::cout << "Config file loaded" << std::endl;
}
