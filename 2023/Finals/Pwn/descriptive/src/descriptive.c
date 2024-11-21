#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define BUF_SIZE 0x1F
#define DEFAULT_FD_NAME "flag"
#define DEBUG 0

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

typedef enum {
    BAN_CHAR,
    BAN_WORD,
    ALLOWED
} banlist;

typedef struct {
    int fd;
    char buffer[BUF_SIZE];
} FDHandler, *PFDHandler;

void cls() {
    printf("\033[2J\033[1;1H");
}

banlist is_banned(const char* str) {
    char blacklist[][10] = { "include", "exec", "ps", "system", "bash", "zsh", "dash", "sh", "echo", "sudo", "pwd", "usr", "dev", "sys", "proc", "pcc", "ctf", "less", "whoami", "grep", "cp", "mv"};
    char ops[] = {'|', '&', ';', '<', '>', '$', '`', '\\', '\'', '\"', '(', ')', '[', ']', '{', '}', '?', '-'};

    for (int i = 0; i < 22; i++) { 
        if (strstr(str, blacklist[i]) != NULL) {
            printf("Banned word: %s\n", blacklist[i]);
            return BAN_WORD;
        }
    }
    for (int i = 0; i < strlen(str); i++) {
        for (int j = 0; j < 19; j++) {
            if (str[i] == ops[j]) return BAN_CHAR;
        }
    }
    return ALLOWED;
}

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

int str_input(char *in, int len) {
    if(fgets(in, len, stdin) == NULL) {
        error("Could not read input.");
        return 1;
    }
    in[strlen(in)-1] = '\0';

    banlist ban = is_banned(in);
    if(ban == BAN_CHAR) {
        error("You have used a banned character.");
        return 1;
    }
    else if(ban == BAN_WORD) {
        error("You have used a banned word.");
        return 1;
    }

    return 0;
}

void init(PFDHandler fdh) {
    fdh->fd = -1;
    memset(fdh->buffer, 0, BUF_SIZE);
}

// ***************** FILE DESCRIPTOR HANDLER *****************
int create_fd(PFDHandler fdh) {

    char buffer[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);

    if(fdh->fd != -1) {
        error("You already have a file descriptor with fd=\033[33m%d\033[0m. Close it first.", fdh->fd);
        return 1;
    }

    print("==== Create a new file descriptor ====");
    print("NOTE: We only allow a limited set of file descriptors");
    print("1. /dev/urandom");
    print("2. /dev/zero");
    print("3. /dev/null");
    print("4. /dev/brrr");
    printf("\n>> ");
    int choice;
    range_input(&choice, 1, 4);

    switch(choice) {
        case 1:
            strncpy(buffer, "/dev/urandom", BUF_SIZE);
            break;
        case 2:
            strncpy(buffer, "/dev/zero", BUF_SIZE);
            break;
        case 3:
            strncpy(buffer, "/dev/null", BUF_SIZE);
            break;
        case 4:
            strncpy(buffer, DEFAULT_FD_NAME, BUF_SIZE);
            break;
    }

    info("Creating file descriptor with name: \033[31m%s\033[0m", buffer);
    if(choice != 4) {
        fdh->fd = fopen(buffer, "r");
    } else {
        fdh->fd = memfd_create(DEFAULT_FD_NAME, 0);
    }

    if(fdh->fd < 0) {
        error("Could not create file descriptor for \033[31m%s\033[0m", buffer);
        return 1;
    }

    success("Created file descriptor with FD: \033[33m%d\033[0m", fdh->fd);
    return 0;
}

int write_fd(PFDHandler fdh) {
    if(fdh->fd == -1) {
        error("You do not have a file descriptor open. Create one first.");
        return 1;
    }
    print("==== Write to a file descriptor ====");
    print("Enter your contents: ");
    if(str_input(fdh->buffer, BUF_SIZE) != 0) {
        return 1;
    }
    int ret = write(fdh->fd, fdh->buffer, BUF_SIZE);
    if(ret < 0) {
        error("Could not write to file descriptor.");
        return 1;
    }
    success("Wrote \033[33m%d\033[0m bytes to file descriptor.", ret & 0xffffffff);
    return 0;
}

int read_fd(PFDHandler fdh) {
    if(fdh->fd == -1) {
        error("You do not have a file descriptor open. Create one first.");
        return 1;
    }
    print("==== Read from a file descriptor ====");
    print("Enter the number of bytes you want to read: ");
    int bytes;
    range_input(&bytes, 0, BUF_SIZE);
    int ret = read(fdh->fd, fdh->buffer, bytes);
    if(ret < 0) {
        error("Could not read from file descriptor.");
        return 1;
    }
    success("Read \033[33m%d\033[0m bytes from file descriptor.", ret & 0xffffffff);
    return 0;
}

int exec_fd(PFDHandler fdh) {
    if(fdh->fd == -1) {
        error("You do not have a file descriptor open. Create one first.");
        return 1;
    }
    // Create empty argv and envp
    char* argv[] = {NULL};
    char* envp[] = {NULL};

    fdh->fd = fexecve(fdh->fd, argv, envp);
    if(fdh->fd < 0) {
        error("Could not execute file descriptor.");
        return 1;
    }

    success("Executed file descriptor.");
    return 0;
}

int close_fd(PFDHandler fdh) {
    if(fdh->fd == -1) {
        error("You do not have a file descriptor open. Create one first.");
        return 1;
    }
    int ret = fclose(fdh->fd);
    if(ret < 0) {
        error("Could not close file descriptor.");
        return 1;
    }
    success("Closed file descriptor.");
    fdh->fd = -1;
    return 0;
}
// ***********************************************************

void menu(PFDHandler fdh) {
    int choice;
    while(1) {
        cls();
        print("Welcome to \033[31mThe\033[33mFlash\033[0m2k's Descriptor Handler\nHandle deez descriptors with care.\n");
        if(fdh->fd != -1) {
            print("Current file descriptor: \033[33m%d\033[0m\n", fdh->fd);
        }
        print("==== File Descriptor Manager Menu ====");
        print("1. Create a new file descriptor");
        print("2. Write to a file descriptor");
        print("3. Read from a file descriptor");
        print("4. Execute a file descriptor");
        print("5. Close a file descriptor");
        print("0. Exit");
        printf("\n>> ");
        range_input(&choice, 0, 5);
        cls();
        switch(choice) {
            case 1:
                create_fd(fdh);
                break;
            case 2:
                write_fd(fdh);
                break;
            case 3:
                read_fd(fdh);
                break;
            case 4:
                exec_fd(fdh);
                break;
            case 5:
                close_fd(fdh);
                break;
            case 0:
                info("Thank you for using my File Descriptor Handler. Much Love. <3");
                exit(0);
                break;
        }
        print("Press enter to continue...");
        getchar();
    }
}

int main(int argc, char** argv, char** envp) {

    FDHandler fdh;

    init(&fdh);
    menu(&fdh);
}