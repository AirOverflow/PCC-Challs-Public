FROM theflash2k/pwn-chal:latest

RUN apt update && apt install gcc python3 -y
RUN apt install -y build-essential

ENV CHAL_NAME="caas-revenge.py"
COPY ${CHAL_NAME} /app/${CHAL_NAME}

# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]