FROM python:3.11-slim-buster

WORKDIR /app
COPY . /app
RUN mkdir /app/dist
EXPOSE 8000
RUN pip3 install -r requirements.txt

ENTRYPOINT [ "/app/docker-entrypoint.sh" ]