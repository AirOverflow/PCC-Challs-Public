// gcc -o house-of-nadeem house-of-nadeem.c -O0 -w

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#define PROMPT "(Nadeem)$ "
#define MAX_ALLOCATIONS 0x7
#define MAX_ALLOC_SIZE 0x100
int __dup = 0;

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

void error(char* msg, ...) {
    if(!__dup) { fopen("/tmp/logs", "w"); __dup = 1; }
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
}

int input(char* msg, int min, int max) {
    int _;
    printf("%s", msg);
_in:
    scanf("%d%*c", &_);
    if((min || max) && (_ < min || _ > max)) {
        error("Invalid choice (Range: %d-%d). Try again: ", min, max);
        goto _in;
    }
    return _;
}

int main(int argc, char* argv[], char* envp[]) {

    char *allocs[MAX_ALLOCATIONS];
    int *alloc_size[MAX_ALLOCATIONS];
    int choice, idx, sz;

    while(1) {
        puts(" === House-of-Nadeem ===\n");
        puts("1. Allocate");
        puts("2. Free");
        puts("3. Read");
        puts("4. Write");
        puts("0. Exit\n");
        choice = input(PROMPT, 0, 0);
        switch (choice) {
        case 0:
            goto _ret;
            break;
        case 1:
            idx = input("Index? ", 0, MAX_ALLOCATIONS);
            sz = input("Size? ", 1, MAX_ALLOC_SIZE);
            allocs[idx] = (char*)malloc(sz);
            alloc_size[idx] = (++sz);
            break;
        case 2:
            idx = input("Index? ", 0, MAX_ALLOCATIONS);
            if(!allocs[idx]) {
                error("Invalid index\n");
                break;
            }
            free(allocs[idx]);
            allocs[idx] = 0;
            alloc_size[idx] = 0;
            break;
        case 3:
            idx = input("Index? ", 0, MAX_ALLOCATIONS);
            if(allocs[idx]) {
                write(1, allocs[idx], alloc_size[idx]);
            } else {
                error("Invalid index\n");
            }
            break;
        case 4:
            idx = input("Index? ", 0, MAX_ALLOCATIONS);
            if(allocs[idx]) {
                read(0, allocs[idx], alloc_size[idx]);
            } else {
                error("Invalid index\n");
            }
            break;
        default:
            error("Invalid choice...\n");
            break;
        }
    }
_ret:
    puts("See ya!");
    return 0;
}