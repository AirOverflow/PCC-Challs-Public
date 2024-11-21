FROM theflash2k/pwn-chal:latest

RUN apt update
RUN apt install python3 -y

ENV CHAL_NAME="yape.py"
COPY ${CHAL_NAME} /app/${CHAL_NAME}

# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]