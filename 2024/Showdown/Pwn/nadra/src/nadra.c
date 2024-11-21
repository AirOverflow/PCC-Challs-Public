#include <stdio.h>
#include <stdlib.h>

#define MAX_USER_SZ 0x100

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

struct info {
    int id;
    int age;
    char *name;
}  __attribute__((packed));
typedef struct info UserProfile;

void get_input(long *_in) {
    scanf("%lld%*c", _in);
}

void win() {
    system("/bin/sh\x00");
}

int initiate_user(UserProfile *user) {
    user->id = 1;
    user->name = malloc(MAX_USER_SZ);
}

int setup_user(UserProfile *user) {
    printf("Enter age: ");
    get_input(&user->age);
    printf("Enter your name: ");
    scanf("%255s", user->name);
}

int main(int argc, char* argv[]) {
    UserProfile up;
    initiate_user(&up);
    setup_user(&up);
    exit(0);
}