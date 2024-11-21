FROM theflash2k/pwn-chal:latest

ENV NO_FLAG=1

COPY --chmod=550 --chown=root:ctf-player --from=theflash2k/showdown-submitter:latest /app/submitter .
COPY --chmod=550 --chown=root:ctf-player ld.so .
COPY --chmod=550 --chown=root:ctf-player libc.so.6 .

ENV CHAL_NAME=entity-encoder
COPY ${CHAL_NAME} .