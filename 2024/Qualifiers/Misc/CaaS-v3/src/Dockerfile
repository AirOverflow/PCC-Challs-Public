FROM theflash2k/pwn-chal:py38

RUN apt update && \
    apt install -y gcc && \
    apt-get clean -y && \
    apt autoremove -y && \
    rm -rf /var/lib/apt/lists/*

ENV CHAL_NAME="chal.py"
COPY ${CHAL_NAME} .

COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]