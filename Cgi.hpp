/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef CGI_HPP_
#define CGI_HPP_

#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstring>


namespace ft {
class Cgi {
 public:
    Cgi(std::string const path, std::map<std::string, std::string> const env);
    ~Cgi(void);
    void run(void);
    std::string getResponse(void) const;
 private:
    int _pipe_fd[2];
    int _pid;
    char **_argv;
    std::map<std::string, std::string> _env;
    std::string _path;
    std::string _response;

    void createEnv(void);
    void createPipe(void);
    char **createArgv(void);
    char **createEnvp(void);
    void createProcess(void);
    void closePipe(void);
    void closeProcess(void);
    void runChild(void);
    void runParent(void);
};

}  // namespace ft

#endif  // CGI_HPP_