FILE=portal
all:
	gcc -o $(FILE) $(FILE).c -w
	gcc -o runner runner.c -w
	./docker-build.sh
run:
	docker run -it --rm --name portal -p8000:8000 pccfinals_pwn_portal:latest
clean:
	rm $(FILE)
