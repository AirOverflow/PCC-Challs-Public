FROM node:latest

WORKDIR /app

COPY . .
RUN echo "PCC{DUMMY}" > /flag.txt

RUN openssl genrsa -out private.pem 2048

RUN npm install
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh

EXPOSE 1337

ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]