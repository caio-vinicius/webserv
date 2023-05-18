/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "../includes/Cgi.hpp"

ft::Cgi::Cgi(std::string path,
             std::map<std::string, std::string> header,
             std::string body) {
    this->_path = path;
    this->_header = header;
    this->_body = body;
    this->createEnv();

    std::map<std::string, std::string>::iterator it = header.begin();
    std::map<std::string, std::string>::iterator ite = header.end();

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
    this->_env["PATH_INFO"] = this->_header["Uri"];
    this->_env["PATH_TRANSLATED"] = "";
    this->_env["QUERY_STRING"] = this->_body;
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
        throw std::exception();
    }
}

void ft::Cgi::createProcess(void) {
    this->_pid = fork();
    if (this->_pid == -1) {
        throw std::exception();
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

    if (this->_path.find(".py") != std::string::npos) {
        argv.push_back("python3");
    } else if (this->_path.find(".pl") != std::string::npos) {
        argv.push_back("perl");
    }
    argv.push_back(this->_path);
    return (vectorToChar(argv));
}

char **ft::Cgi::createEnvp(void) {
    std::map<std::string, std::string>::iterator it = this->_env.begin();
    std::map<std::string, std::string>::iterator ite = this->_env.end();
    std::vector<std::string> envp;

    while (it != ite) {
        envp.push_back(reinterpret_cast<const char *>((it->first + "="
            + it->second).c_str()));
        it++;
    }

    return (vectorToChar(envp));
}

void ft::Cgi::runChild(void) {
    dup2(this->_pipe_fd[1], STDOUT_FILENO);
    char **argv = this->createArgv();
    char **envp = this->createEnvp();
    if (this->_path.find(".py") != std::string::npos) {
        if (execve("/usr/bin/python3", argv, envp) == -1) {
            throw std::exception();
        }
    } else if (this->_path.find(".pl") != std::string::npos) {
        if (execve("/usr/bin/perl", argv, envp) == -1) {
            throw std::exception();
        }
    }
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

std::string ft::Cgi::toUpperUnderscore(std::string str) {
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
