# Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari.
# All rights reserved. 42

# Webserv tests with curl. All tests are based on the configuration file below:

#    server {
#        listen localhost:8080 localhost:8084
#        server_name server1 banana pera
#        error_page 400 /errora.html
#        error_page 300 404 /error.html
#        error_page 400 /errora.html
#
#        client_max_body_size 1k
#        root /www
#        index form.html
#        location / {
#            autoindex off
#        }
#        location /pratodecomida {
#            autoindex on
#        }
#        location /cgi {
#        }
#    }
#
#    server {
#        listen localhost:8081
#        server_name server2
#        error_page 500 error.html
#        root  /spool/www
#        location /upload {
#        }
#    }
#
#    server {
#        listen localhost:8082
#        server_name gui
#        error_page 500 /accepted.html
#        root /www
#        index gui.html
#
#        location /upload {
#        }
#    }
#
#    server {
#        listen localhost:8082
#        server_name lenzo
#        error_page 204 /accepted.html
#        root /www
#        index lenzo.html
#
#        location /upload {
#        }
#    }

# Tests

# Port 8080

# 1. GET request with a valid path
curl -i localhost:8080/index.html
# 2. GET request with a valid path and a valid query string
curl -i localhost:8080/index.html?query=string
# 3. POST request with a valid path
curl -i -X POST localhost:8080/gui.html -d "name=gui&age=21"
# 4. POST request with a valid path and a valid query string
curl -i -X POST localhost:8080/gui.html?query=string
# 5. GET request with CGI path
curl -i localhost:8080/cgi.py
# 6. POST request with CGI path
curl -i -X POST localhost:8080/cgi.py
# 7. GET request with CGI path and a valid query string
curl -i localhost:8080/cgi.py?query=string
# 8. POST request with CGI path and a valid query string
curl -i -X POST localhost:8080/cgi.py?query=string
# 9. POST request with CGI path and a valid body
curl -i -X POST localhost:8080/cgi.py -d "body=valid"
# 10. POST request with CGI path, a valid body and a valid query string
curl -i -X POST localhost:8080/cgi.py?query=string -d "body=valid"
# 11. GET request with not supported method
curl -i -X PUT localhost:8080/index.html
# 12. GET request to test autoindex
curl -i localhost:8080/pratodecomida
# 13. DELETE request to delete file
curl -i -X DELETE localhost:8080/gui.html
# 14. DELETE request to delete directory
curl -i -X DELETE localhost:8080/pratodecomida
# 15. DELETE request to delete file that does not exist
curl -i -X DELETE localhost:8080/doesnotexist.html
# 16. POST request with a body bigger than client_max_body_size 50 bytes
curl -i -X POST localhost:8080 -d "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
# 17. GET request with a body bigger than client_max_body_size 50 bytes
curl -i -X GET localhost:8080 -d "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

# Port 8081 tests

# 18. GET request with a valid path
curl -i localhost:8081/index.html
# 19. GET request with a valid path and a valid query string
curl -i localhost:8081/index.html?query=string

# Port 8082 tests

# 20. GET request with server name lenzo
curl -H "Host: lenzo" -i localhost:8082
# 21. GET request with server name gui
curl -H "Host: gui" -i localhost:8082
# 22. POST lenzo persosonalized error_page
curl -i -X POST localhost:8082/lenzotest.html -d "name=lenzo&age=21"
# 23. DELETE lenzo persosonalized error_page
curl -i -X DELETE localhost:8082/lenzotest.html
