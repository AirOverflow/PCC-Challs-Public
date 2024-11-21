FROM theflash2k/pwn-chal:latest

ENV CHAL_NAME=plusplus
COPY ${CHAL_NAME} .

RUN apt-get update && \
    apt-get install -y libfmt-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

COPY --chmod=550 --chown=root:ctf-player --from=theflash2k/showdown-submitter:latest /app/submitter .
ENV NO_FLAG=1
