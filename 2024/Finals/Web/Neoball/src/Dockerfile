FROM node:20

ARG DEBIAN_FRONTEND=noninteractive
ENV NODE_ENV production

RUN mkdir /app
WORKDIR /app

RUN echo "PCC{DUMMY}" > /flag.txt
RUN chown root:root /flag.txt

COPY --chown=root:root app/server.js /app/
COPY --chown=root:root app/package.json /app/
COPY --chown=root:root app/package-lock.json /app/

RUN npm install

RUN chmod 644 /flag.txt

EXPOSE 1337

COPY docker-entrypoint.sh /tmp
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]


