FROM python:3.10-slim

WORKDIR /app
COPY . .
RUN pip install -r requirements.txt
RUN mv ./docker-entrypoint.sh /docker-entrypoint.sh
ENTRYPOINT [ "/docker-entrypoint.sh" ]