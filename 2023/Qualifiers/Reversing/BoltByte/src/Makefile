all:
	python3 build-flag.py
	gcc -w -o boltbyte -g -O2 byte.c
	strip boltbyte
	python3 setup.py
	chmod +x boltbyte_patched
	cp boltbyte_patched ../dist/boltbyte
	cp boltbyte_patched ../writeup/boltbyte
clean:
	rm -f boltbyte*
