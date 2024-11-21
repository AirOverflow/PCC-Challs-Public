#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 0x100

__attribute__((constructor))
void __setup__(){
	// IGNORE ME
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
}

void get_flag() { // Sneak 100
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

char inputChar() {
	char choice;
	scanf("%c", &choice);
	getchar();
	return choice;
}

void menu();
void secret();
void ask();
void init();

int main() {
    srand(time(NULL));
    init();
    return 0;
}

void secret() {
    printf("Well, well, well. You're in luck. The oracle wants to give you a secret. I hope you know what you need to do with it. %p.", main);
    printf("Even if you don't, that still looks l33t ngl.\n");
}

void ask() {
    char question[25];
    printf("The oracle is unavailable at the moment. However, please tell me your question, I'll convey it to her.\n");
    fgets(question, BUFFER_SIZE, stdin);
    printf("Okay, very well. I'll ask her and let you know soon...");
    return;
}

void menu() {
    printf("=== Welcome to the Oracle. I'm here to help you. ===\n");
    printf("1. Ask for a secret that might change your life.\n");
    printf("2. Ask the Oracle a question.\n");
}

void init() {
    char choice;
_init_:
    menu();
    printf("\n>> ");
    choice = inputChar();
    if(choice == '1') {
        secret();
    }
    else if(choice == '2') {
        ask();
    }
    else {
        printf("Invalid option selected.");
    }
    printf("Do you want to continue? (Y/N): ");
    choice = inputChar();
    if(tolower(choice) == 'y') {
        printf("\n\n");
        goto _init_;
    }
}