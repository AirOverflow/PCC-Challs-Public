FROM theflash2k/pwn-chal:latest

RUN apt update && apt install gcc python3 -y
RUN apt install -y build-essential
RUN apt install -y g++ g++-multilib gcc-multilib

ENV CHAL_NAME="caas.py"
ENV PORT=8000
EXPOSE $PORT
COPY ${CHAL_NAME} /app/${CHAL_NAME}

# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]