FROM theflash2k/pwn-chal:latest

COPY --chmod=550 --chown=root:ctf-player --from=theflash2k/showdown-submitter:latest /app/submitter .

ENV NO_FLAG=1

ENV CHAL_NAME=seed
COPY ${CHAL_NAME} .