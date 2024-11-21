# Author: @TheFlash2k

CHAL_NAME := fhod
SRC := $(CHAL_NAME).c
TAR_FILE := pwn-$(CHAL_NAME).tar.gz
CONTAINER_NAME := pcc-pwn-$(CHAL_NAME)
DEFAULT_FLAG := "PCC{F4k3_fl4g_f0r_t3st1ng}"

# FLAGS
CC := gcc
FLAGS := -w -Wl,-z,relro,-z,now -no-pie

all:
	$(CC) -o $(CHAL_NAME) $(SRC) $(FLAGS)

dist:
	mkdir -p ../dist/
	echo -n $(DEFAULT_FLAG) > flag.txt
	tar -czvf $(TAR_FILE) $(SRC) $(CHAL_NAME) docker-compose.yml Dockerfile.dist flag.txt
	rm -f ../dist/$(TAR_FILE)
	mv $(TAR_FILE) ../dist/

generate:
	# This will generate a dockerfile for your challenge accordingly:
	rm -f Dockerfile
	echo "FROM theflash2k/pwn-chal:latest" > Dockerfile
	echo "\nENV CHAL_NAME=$(CHAL_NAME)" >> Dockerfile
	echo '\nCOPY $${CHAL_NAME} .' >> Dockerfile
	echo "COPY flag.txt ." >> Dockerfile

build:
	docker build --no-cache -t $(CONTAINER_NAME) .

run:
	docker run -it --rm -p1337:8000 --hostname $(CHAL_NAME) --name $(CONTAINER_NAME) $(CONTAINER_NAME)

stop:
	docker stop $(CONTAINER_NAME)

writeup:
	mkdir -p ../writeup/
	mv -f libc.so.6 exploit.py ../writeup/
	cp $(CHAL_NAME) ../writeup/
	echo "# $(CHAL_NAME)" > ../writeup/WRITEUP.md
	
clean:
	rm -f $(CHAL_NAME)
	docker rmi $(CONTAINER_NAME)
