#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_SIZE 0x100

__attribute__((constructor))
void __setup__(){
	// IGNORE ME
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
}

void clean() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void get_flag(int __v1) {
	printf("This is the restricted section, what're you doing here??\n");
	if(__v1 == 0xdeadbeef) {
		printf("Damn. You know all the code, how?\n\n");
		FILE *file;
	    char line[100];
	    file = fopen("/flag", "r");
	    if (file == NULL) {
	        puts("[FAIL] Contact an admin.\n");
	        return 1;
	    }
	    while (fgets(line, sizeof(line), file) != NULL) {
	        puts(line);
	    }
	    fclose(file);
	}
}

void identify(int pass) {
    clean();
    char name[BUFFER_SIZE/2];
    printf("Well, you did know the passcode was %d, but who are you?\n");
    fgets(name, BUFFER_SIZE * 2, stdin);
    printf("%s? Are you, really? Never heard of ya' tbh.\n", name);
    printf("Any how, you can pass now...");
    return;
}

int main() {
    
    srand(time(NULL));

    int to_guess = rand();
    int guess_me = -1;

    printf("Sorry boss, I can't identify you, can you tell me the passcode?\n>> ");
    scanf("%d", &guess_me);

    if(to_guess == guess_me) {
        printf("Well, I guess you do know the passcode. But, please identify yourself first\n");
        identify(guess_me);
    }
    else {
        printf("Nope, Let me call the bouncer..\n");
        return 1;
    }

    return 0;
}