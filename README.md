![cpplint](https://github.com/caio-vinicius/webserv/actions/workflows/cpplint.yml/badge.svg)
![make](https://github.com/caio-vinicius/webserv/actions/workflows/make.yml/badge.svg)

# webserv
This is when you finally understand why a URL starts with HTTP

## Sockets

* A socket is one end of an interprocess communication channel.

* Processes can communicate with each other only if their sockets are of the same type and in the same domain

* There are two widely used address domains
    * unix domain, in which two processes which share a common file system communicate
    * Internet domain in which two processes running on any two hosts on the Internet communicate
* Each socket needs a port number on that host. Port numbers are 16 bit unsigned integers. The lower numbers are reserved in Unix for standard services
* There are two widely used socket types, stream sockets, and datagram sockets
    * Stream sockets use TCP (Transmission Control Protocol), which is a reliable, stream oriented protocol
    * Datagram sockets use UDP (Unix Datagram Protocol), which is unreliable and message oriented.

The steps involved in establishing a socket on the client side are as follows:

* Create a socket with the socket() system call
* Connect the socket to the address of the server using the connect() system call
* Send and receive data

# CONFIG FILE

## How works
```
Syntax:     server { ... }
Default:     —
Context:     http

Sets configuration for a virtual server.

Syntax:     listen address[:port] ...
Default:     listen *:80 | *:8000;
Context:     server

Sets the address and port for IP.

Syntax:     server_name name ...;
Default:     server_name _;
Context:     server

Sets names of a virtual server.

Syntax:     error_page code ... uri;
Default:     —
Context:     server

Defines the URI that will be shown for the specified errors.

Syntax:     client_max_body_size size;
Default:     client_max_body_size 1m;
Context:     server

Sets the maximum allowed size of the client request body.

Syntax:     root path;
Default:     root html;
Context:     server

Sets the root directory for requests.

Syntax:     location uri { ... }
Default:     /
Context:     server

Sets configuration depending on a request URI.

Syntax:     index file ...;
Default:     index index.html;
Context:     server

Defines files that will be used as an index.

Syntax:     auto on | off;
Default:     autoindex off;
Context:     location

Enables or disables the directory listing output.
```

