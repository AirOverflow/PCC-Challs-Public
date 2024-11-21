#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 0x100

__attribute__((constructor))
void __setup__(){
	// IGNORE ME
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
}

typedef struct {
    char buffer[BUFFER_SIZE/2];
    int age;
}User, *PUser;

void init(PUser user) {
    puts("Enter your username:: ");
    fgets(user->buffer, BUFFER_SIZE, stdin);
    if(strncmp(user->buffer, "\x4c\x45\x54\x4d\x45\x57\x49\x4e\x50\x4c\x53\x53\x53", 14) == 0) {
        puts("Welcome. I didn't think you'd make it.");
    } else {
        puts("Nope.. not allowed fam..");
    }
    return;
}

int main() {
    User user;
    init(&user);
    return 0;
}