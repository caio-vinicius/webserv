FROM debian:latest

RUN apt-get update

RUN apt-get install nginx -y

RUN rm -rf /etc/nginx/sites-available/default

COPY confs/default /etc/nginx/sites-available/default

COPY confs/sla.html /var/www/html

EXPOSE 3000

ENTRYPOINT ["nginx", "-g", "daemon off;"]
