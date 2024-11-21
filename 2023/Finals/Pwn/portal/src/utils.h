#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_READ_SIZE 0x64

#define ADMIN_REGION "ADMINISTRATOR"
#define DEFAULT_REGION "EU"
#define DEBUG 0

typedef struct user {
    char *name;
    char *password;
    char *region;
    bool is_admin;
} user_t;

__attribute__((constructor))
void __setup__() {
    // IGNORE ME
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
}

// *************************************************************
// ************************** LOGGING **************************
// *************************************************************
// https://gist.github.com/TheFlash2k/a32b2b1ffac86afe9faca844a9a6b0d9

char* get_color(const char* mode) {
    if(strcmp(mode, "INFO") == 0) return "\033[36m";
    if(strcmp(mode, "ERROR") == 0) return "\033[31m";
    if(strcmp(mode, "SUCCESS") == 0) return "\033[32m";
    if(strcmp(mode, "DEBUG") == 0) return "\033[33m";
    return "\033[0m";
}

#define base(file, method, msg, ...) {\
	fprintf(file, "[%s%s\033[0m] ", get_color(method), method);\
	fprintf(file, msg, ##__VA_ARGS__);\
	fprintf(file, "\n");\
}

#define info(msg, ...) base(stdout, "INFO", msg, ##__VA_ARGS__)
#define error(msg, ...) base(stderr, "ERROR", msg, ##__VA_ARGS__)
#define success(msg, ...) base(stdout, "SUCCESS", msg, ##__VA_ARGS__)
#define debug(msg, ...) if(DEBUG == 1) { base(stdout, "DEBUG", msg, ##__VA_ARGS__) }
#define print(msg, ...) printf(msg, ##__VA_ARGS__); printf("\n")
// *************************************************************

void get_input(int *in) {
	// Secure integer input function.
	// https://stackoverflow.com/questions/41145908/how-to-stop-user-entering-char-as-int-input-in-c
	char next;
	if (scanf("%d", in) < 0 || *in < 0 || ((next = getchar()) != EOF && next != '\n')) {
        clearerr(stdin);
        do next = getchar(); while (next != EOF && next != '\n');
        clearerr(stdin);
	}
}

void range_input(int *in, int min, int max) {
    do {
        get_input(in);
        if(*in < min || *in > max) {
            error("Invalid input. Try again. [Range: %d-%d]", min, max);
            printf(">> ");
        }
    } while(*in < min || *in > max);
}

void get_input_str(char *in, int len) {
	if(fgets(in, len, stdin) == NULL) {
		puts("[ERROR] - Could not read input.");
		return;
	}
	in[strlen(in)-1] = '\0';
}

void read_admin_password(char *admin_pwd) {
	FILE *file;
	file = fopen("/admin-password.txt", "r");
	if (file == NULL) { puts("[ERROR] - Please contact @TheFlash2k. The admin-password wasn't generated on the remote instance.\n"); exit(1); }
	fgets(admin_pwd, MAX_READ_SIZE, file);
	fclose(file);
}

void read_flag(const user_t* user) {

	if(strcmp(user->region, ADMIN_REGION)) {
		printf("[ERROR] - Your region %s cannot read the flag. It was generated for %s only.\n", user->region, ADMIN_REGION);
		return;
	}

    if(!user->is_admin) {
        printf("[ERROR] - You are not an admin!\n");
        return;
    }

    if(strcmp(user->name, "admin")) {
        printf("[ERROR] - You are not an admin!\n");
        return;
    }

    if(strcmp(user->password, getenv("ADMIN_PASSWORD"))) {
        printf("[ERROR] - Invalid administrator password.\n");
        return;
    }

	FILE *file;
    char line[MAX_READ_SIZE];
    file = fopen("/flag", "r");
    if (file == NULL) {
        puts("[FAIL] /flag doesn't exist on remote instance. Please contact @TheFlash2k.\n");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL) { puts(line); }
    fclose(file);
	exit(0);
}