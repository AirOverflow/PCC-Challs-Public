FROM theflash2k/pwn-chal:latest

ENV CHAL_NAME=directors-cut
COPY ${CHAL_NAME} .

# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]