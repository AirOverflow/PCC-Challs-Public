FROM theflash2k/pwn-chal:latest

RUN apt update && apt install gcc python3 -y
RUN apt install -y build-essential
RUN apt install -y python3-pip
RUN pip3 install unicorn

ENV CHAL_NAME="unibrr.py"
COPY ${CHAL_NAME} /app/${CHAL_NAME} 
