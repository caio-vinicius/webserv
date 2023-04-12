/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#ifndef CGI_HPP_
#define CGI_HPP_

#include <map>
#include <string>

namespace ft {
class Cgi {
 public:
    Cgi();
    virtual ~Cgi();
    void run(void);
 private:
    int _pipe_fd[2];
    int _pid;
    char **_argv;
    std::map<std::string, std::string> _env;
    std::string _path;
    std::string _response;

    void createEnv(void);
    void createPipe(void);
    void createProcess(void);
    void closePipe(void);
    void closeProcess(void);
};

}  // namespace ft

#endif  // CGI_HPP_
