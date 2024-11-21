FROM python:3-slim-buster
COPY c2 /app
WORKDIR /app

EXPOSE 1337

COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]