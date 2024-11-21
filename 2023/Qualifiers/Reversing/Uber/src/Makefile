obj-m:=Uber.o

all:
	python3 flag.py
	#make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	make -C /lib/modules/5.16.0-kali7-amd64/build M=$(shell pwd) modules

clean:
	#make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	make -C /lib/modules/5.16.0-kali7-amd64/build M=$(shell pwd) clean
