// Compile: gcc -o loadme loadme.c -fno-stack-protector

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAX_BUF 0x100

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    signal(SIGALRM, exit);
    alarm(0x20);
}

int main(int argc, char* argv[], char* envp[]) {
    char filename[MAX_BUF];
    char name[MAX_BUF];
    memset(filename, 0x0, MAX_BUF);
    printf("Enter file you want to read: ");
    fgets(filename, MAX_BUF, stdin);
    filename[strcspn(filename, "\n")] = 0;
    printf("[*] Reading %s into memory...\n", filename);

    FILE *file = fopen(filename, "rb");
    if (file == NULL
        || strstr(filename, "flag") != NULL
        || strstr(filename, "loadme") != NULL
        || strstr(filename, "bin") != NULL) {
        printf("Error: Invalid file.\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *chunk = (char*)mmap(
        (void*)0x13370000,
        size,
        PROT_EXEC | PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1, 0LL);
    memset(chunk, 0x00, size);
    fread(chunk, 1, size, file);
    fclose(file);

    printf("I'm sorry, I never asked for your name: ");
    read(0, name, MAX_BUF*4);
    return 0;
}