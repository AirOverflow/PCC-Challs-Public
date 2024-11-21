#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

int main(int argc, char* argv[], char* envp[]) {

    srand(time(NULL));
    long rnd;
    long user;


    for(int i = 0; i < 10; ++i) {
        rnd = (rand() >> 8) | (rand() | 0xFFFF);
        printf("? ");
        scanf("%ld", &user);
        if(rnd != user) {
            printf("Nope!\n");
            return 1;
        }
    }
    execve("/bin/sh", NULL, envp);
}