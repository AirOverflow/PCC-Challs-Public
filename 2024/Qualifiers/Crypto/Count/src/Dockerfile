FROM theflash2k/pwn-chal:crypto

ENV CHAL_NAME="challenge.py"
COPY ${CHAL_NAME} /app/${CHAL_NAME} 
COPY a_quote_from_a_leader.txt .

# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]

