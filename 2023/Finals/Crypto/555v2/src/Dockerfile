FROM theflash2k/pwn-chal-python:latest


RUN pip3 install pycryptodome

ENV CHAL_NAME="rsa.py"
COPY ${CHAL_NAME} /app/${CHAL_NAME} 

# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]

