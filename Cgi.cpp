/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

#include "./Cgi.hpp"

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
    this->_env["SERVER_SOFTWARE"] = "Webserv/1.0";
    this->_env["REDIRECT_STATUS"] = "200";
}

void ft::Cgi::createPipe(void) {
    if (pipe(this->_pipe_fd) == -1) {
        throw ft::Cgi::PipeError();
    }
}

void ft::Cgi::createProcess(void) {
    this->_pid = fork();
    if (this->_pid == -1) {
        throw ft::Cgi::ForkError();
    }
}

void ft::Cgi::closePipe(void) {
    close(this->_pipe_fd[0]);
}

char **ft::Cgi::createArgv(void) {
    std::vector<std::string> argv;

    argv.push_back(this->_path);
    argv.push_back(NULL);
    return (&argv[0]);
}

char **ft::Cgi::createEnvp(void) {
    std::map<std::string, std::string>::iterator it = this->_env.begin();
    std::map<std::string, std::string>::iterator ite = this->_env.end();
    std::vector<std::string> envp;

    while (it != ite) {
        envp.push_back((it->first + "=" + it->second).c_str());
        it++;
    }
    return (&envp[0]);
}

void ft::Cgi::runChild(void) {
    dup2(this->_pipe_fd[0], STDIN_FILENO);
    char **argv = this->createArgv();
    char **_envp = this->createEnvp();
    execve(this->path, argv, _envp);
    exit(0);
}

void ft::Cgi::runParent(void) {
    waitpid(this->_pid, NULL, 0);
    this->closePipe();
    char buffer[1024];
    int ret = 0;
    while ((ret = read(this->_pipe_fd[0], buffer, 1024)) > 0) {
        this->_response.append(buffer, ret);
    }
}

void ft::Cgi::run(void) {
    this->createEnv();
    this->createPipe();
    this->createProcess();
    if (this->_pid == 0) {
        this->runChild();
    } else {
        this->runParent();
    }
    this->closePipe();
}
