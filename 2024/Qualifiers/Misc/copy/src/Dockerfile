FROM theflash2k/pwn-chal:py38

ENV CHAL_NAME="app.py"
COPY ${CHAL_NAME} .

COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]