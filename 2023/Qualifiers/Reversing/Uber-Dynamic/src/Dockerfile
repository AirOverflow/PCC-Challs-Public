FROM python:3.8-slim-buster

WORKDIR /app
# Install GCC
RUN apt-get update && apt-get install -y \
    gcc build-essential \
    && rm -rf /var/lib/apt/lists/*

# Copy src to /app
COPY . /app
RUN mkdir /app/dist
ENTRYPOINT [ "/app/docker-entrypoint.sh" ]
