FROM php:7.4-apache

COPY run.sh /usr/sbin/
RUN chmod +x /usr/sbin/run.sh

COPY ./src /var/www/html
RUN chmod 777 /var/www/html/uploads

RUN echo 'PCC{DUMMY}' > /flag.txt

EXPOSE 80
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]