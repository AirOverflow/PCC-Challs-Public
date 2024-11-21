FROM theflash2k/pwn-chal

ENV CHAL_NAME=runner
COPY . /app

# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]