/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Cgi.hpp"

std::string toUpperUnderscore(std::string str) {
    std::string result;

    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        if (*it == '-') {
            result += '_';
        } else {
            result += toupper(*it);
        }
    }
    return (result);
}

ft::Cgi::Cgi(std::string const path,
             std::map<std::string, std::string> env) {
    this->createEnv();

    std::map<std::string, std::string>::iterator it = env.begin();
    std::map<std::string, std::string>::iterator ite = env.end();

    while (it != ite) {
        this->_env[toUpperUnderscore(it->first)] = it->second;
        it++;
    }
}

ft::Cgi::~Cgi(void) {}

void ft::Cgi::createEnv(void) {
    this->_env["AUTH_TYPE"] = "";
    this->_env["CONTENT_LENGTH"] = "";
    this->_env["CONTENT_TYPE"] = "";
    this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->_env["PATH_INFO"] = "";
    this->_env["PATH_TRANSLATED"] = "";
    this->_env["QUERY_STRING"] = "";
    this->_env["REMOTE_ADDR"] = "";
    this->_env["REMOTE_IDENT"] = "";
    this->_env["REMOTE_USER"] = "";
    this->_env["REQUEST_METHOD"] = "";
    this->_env["REQUEST_URI"] = "";
    this->_env["SCRIPT_NAME"] = "";
    this->_env["SERVER_NAME"] = "";
    this->_env["SERVER_PORT"] = "";
    this->_env["SERVER_PROTOCOL"] = "";
    this->_env["SERVER_SOFTWARE"] = "42webserv/1.0";
    this->_env["REDIRECT_STATUS"] = "200";
}

void ft::Cgi::createPipe(void) {
    if (pipe(this->_pipe_fd) == -1) {
        // throw ft::Cgi::PipeError();
        std::cout << "Pipe error" << std::endl;
    }
}

void ft::Cgi::createProcess(void) {
    this->_pid = fork();
    if (this->_pid == -1) {
        // throw ft::Cgi::ForkError();
        std::cout << "Fork error" << std::endl;
    }
}

void ft::Cgi::closePipe(void) {
    close(this->_pipe_fd[0]);
}

char **vectorToChar(std::vector<std::string> &vec) {
    char **ret = new char *[vec.size() + 1];
    for (size_t i = 0; i < vec.size(); i++) {
        ret[i] = strdup(vec[i].c_str());
    }
    ret[vec.size()] = NULL;
    return (ret);
}

char **ft::Cgi::createArgv(void) {
    std::vector<std::string> argv;

    argv.push_back("python3");
    argv.push_back("cgi.py");
    return (vectorToChar(argv));
}

char **ft::Cgi::createEnvp(void) {
    std::map<std::string, std::string>::iterator it = this->_env.begin();
    std::map<std::string, std::string>::iterator ite = this->_env.end();
    std::vector<std::string> envp;

    while (it != ite) {
        envp.push_back(reinterpret_cast<char *>((it->first + "=" + it->second).c_str()));
        it++;
    }
    return (vectorToChar(envp));
}

void ft::Cgi::runChild(void) {
    dup2(this->_pipe_fd[1], STDOUT_FILENO);
    char **argv = this->createArgv();
    char **envp = this->createEnvp();
    execve("/usr/bin/python3", argv, envp);
    delete argv;
    delete envp;
    exit(0);
}

void ft::Cgi::runParent(void) {
    waitpid(this->_pid, NULL, 0);
    close(this->_pipe_fd[1]);
    char buffer[1024];
    int ret = 0;
    while ((ret = read(this->_pipe_fd[0], buffer, 1024)) > 0) {
        this->_response.append(buffer, ret);
    }
}

void ft::Cgi::run(void) {
    this->createPipe();
    this->createProcess();
    if (this->_pid == 0) {
        this->runChild();
    } else {
        this->runParent();
    }
    this->closePipe();
}

std::string ft::Cgi::getResponse(void) const {
    return (this->_response);
}
