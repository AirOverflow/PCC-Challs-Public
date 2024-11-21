FROM node:21.5-slim@sha256:c88704924204ee6d4e9da02275a8fd27355502f7177e4175cd0d3a4375a9c9c8

ARG DEBIAN_FRONTEND=noninteractive
ENV NODE_ENV production

RUN apt update && apt install -y tzdata openssl
RUN dpkg-reconfigure tzdata

ENV USER hash
RUN adduser --disabled-password --gecos "" --uid 1001 $USER
RUN mkdir /app
WORKDIR /app

RUN echo "PCC{DUMMY}" > /flag.txt
RUN chown root:root /flag.txt

COPY app/server.js /app/
COPY app/package.json /app/
COPY app/package-lock.json /app/
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh

RUN npm install


RUN openssl genrsa -out /priv.pem 2048
RUN openssl rsa -in /priv.pem -pubout -out /pub.crt

RUN chmod 664 /flag.txt
RUN chown root:$USER /flag.txt
RUN chmod 644 /priv.pem
RUN chmod 644 /pub.crt

USER $USER
EXPOSE 1337

ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]


