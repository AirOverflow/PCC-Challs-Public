# Author: @TheFlash2k

CHAL_NAME := misc-caasv3
CHAL_FILE := chal.py
TAR_FILE := $(CHAL_NAME).tar.gz
CONTAINER_NAME := pcc-$(CHAL_NAME)
DEFAULT_FLAG := "PCC{77_byt35_pl4c3hold3r_fl4g_th4t_15_us3le5s_bu7_n0t_really_pl4c3h0ld3r_th0}"

all:
	dist
	build
	writeup

dist:
	mkdir -p ../dist
	echo -n $(DEFAULT_FLAG) > flag.txt
	tar -zcvf $(TAR_FILE) \
		$(CHAL_FILE) \
		docker-compose.yml \
		Dockerfile.dist \
		flag.txt

	rm -f ../dist/$(TAR_FILE)
	mv $(TAR_FILE) ../dist/

build:
	docker build -t $(CONTAINER_NAME) -f Dockerfile.dist .

build-ctfd:
	docker build -t $(CONTAINER_NAME) .

run:
	docker run -it --rm -p1337:8000 --hostname $(CHAL_NAME) --name $(CONTAINER_NAME) $(CONTAINER_NAME)

stop:
	docker stop $(CONTAINER_NAME)

writeup:
	mkdir -p ../writeup
	
clean:
	rm -f $(CHAL_NAME)
	docker rmi $(CONTAINER_NAME)
