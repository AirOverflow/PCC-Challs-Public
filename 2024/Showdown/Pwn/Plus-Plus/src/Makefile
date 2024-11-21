# Author: @TheFlash2k

CHAL_NAME := plusplus
SRC := $(CHAL_NAME).cxx
TAR_FILE := showdown-$(CHAL_NAME).tar.gz
CONTAINER_NAME := showdown-pwn-$(CHAL_NAME)
DEFAULT_FLAG := "PCC{F4k3_fl4g_f0r_t3st1ng}"

# FLAGS
CC := g++
FLAGS := -w -std=c++23 -lfmt -fno-stack-protector -fPIE

all:
	$(CC) -o $(CHAL_NAME) $(SRC) $(FLAGS)

dist:
	mkdir -p ../dist
	echo $(DEFAULT_FLAG) > flag.txt
	# you can change these files to your likings
	tar -zcvf $(TAR_FILE) $(CHAL_NAME) $(SRC) docker-compose.yml Dockerfile.dist flag.txt
	rm -f ../dist/$(TAR_FILE)
	mv $(TAR_FILE) ../dist/

generate:
	# This will generate a dockerfile for your challenge accordingly:
	rm -f Dockerfile
	echo "FROM theflash2k/pwn-chal:latest" > Dockerfile
	echo "\nENV CHAL_NAME=$(CHAL_NAME)" >> Dockerfile
	echo '\nCOPY $${CHAL_NAME} .' >> Dockerfile
	echo "COPY flag.txt ." >> Dockerfile
	echo "\nEXPOSE 8000" >> Dockerfile

build:
	docker build -t $(CONTAINER_NAME) .

run:
	docker run -it --rm -p1337:8000 --hostname $(CHAL_NAME) --name $(CONTAINER_NAME) $(CONTAINER_NAME)

stop:
	docker stop $(CONTAINER_NAME)

writeup:
	mkdir -p ../writeup
	mv -f .gdb_history $(CHAL_NAME)_patched ld-* libc* exploit.py ../writeup/
	cp $(CHAL_NAME) ../writeup/
	echo "# $(CHAL_NAME)" > ../writeup/WRITEUP.md

clean:
	rm -f $(CHAL_NAME)
	docker rmi $(CONTAINER_NAME)
