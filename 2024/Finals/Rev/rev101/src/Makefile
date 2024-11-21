CHAL_NAME := rev101
SRC := $(CHAL_NAME).cpp
TAR_FILE := rev-$(CHAL_NAME).tar.gz

CC := g++
FLAGS := -w -O0

all:
	$(CC) -o $(CHAL_NAME) $(SRC) $(FLAGS)
	strip $(CHAL_NAME)

dist:
	mkdir -p ../dist
	tar -zcvf $(TAR_FILE) $(CHAL_NAME)
	rm -f ../dist/$(TAR_FILE)
	mv $(TAR_FILE) ../dist/

writeup:
	mkdir -p ../writeup
	cp $(CHAL_NAME) ../writeup/
	echo "# $(CHAL_NAME)" > ../writeup/WRITEUP.md
	
clean:
	rm -f $(CHAL_NAME)
