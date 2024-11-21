#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CHUNK_SIZE 0x40000

__attribute__((constructor))
void __setup__(){
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
}

bool hasName = false;
typedef struct {
	long *isInActive;
	char *userName;
} USER, *PUSER;

void read_flag() {
	FILE *file;
    char line[100];
    file = fopen("/flag", "r");
    if (file == NULL) {
        puts("[FAIL] Contact an admin.\n");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL) { puts(line); }
    fclose(file);
	exit(0);
}

void init_user(PUSER user) {
	user->isInActive = (long*)malloc(CHUNK_SIZE);
	*(user->isInActive) = 1; // Creating an inactive user.
	user->userName = NULL;
}

void add_username(PUSER user) {
	if(user->userName != NULL || hasName) {
		printf("You already have a username, you cannot change it..");
		return;
	}
	size_t len;
	printf("Welcome. Please identify yourself first\nHowever, for limited space, can you please tell me how long your name is? ");
	scanf("%lu", &len);
	user->userName = (char*)malloc(len);
	printf("Okay, that should be good. What's your name? ");
	read(0, user->userName, len);
	user->userName[len - 1 ] = '\0';
	printf("Perfect. Welcome %s\n");
	hasName = true;
}

void get_flag(PUSER user) {
	if(*(user->isInActive) == 0) {
		printf("Welcome back %s\n, here's the flag for you:\n", user->userName);
		read_flag();
	}
	else printf("You're an inactive user. You aren't allowed access to this resource.\n");
}

void print_user(PUSER user) {
	if(user->userName) printf("Name: %s\n", user->userName);
	printf("Is User Inactive? %p\n", user->isInActive);
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

int main() {

	PUSER user = NULL;

	printf("Welcome to TheFlash2k's User Management Program\nThe program is still in BETA and I don't save the creds, but you can have fun.\nYou can report the bugs found in this app too (However I highly doubt it.)\n[The only thing I remember, which I forgot to fix was using an invalid format specifier but I don't think it's that big a deal.]\n\n");

	int choice;

	while(1) {

		if(user && user->userName != NULL) {
			printf("==> Logged in as: %s\n", user->userName);
		}

		printf("\n=== MENU ===\n");
		printf("1. Create a user.\n");
		printf("2. Add Username for the user.\n");
		printf("3. Print current user's details.\n");
		printf("4. Get the flag\n");
		printf("0. Exit\n\nCHOICE [0-4]: ");
		get_input(&choice);
		printf("Choice: %d\n", choice);

		if(choice < 0 || choice > 4) {
			printf("Error: Invalid choice..\n");
			continue;
		}

		printf("\n\n");

		switch (choice) {
		case 1:
			if(user) {
				printf("User is already created. Cannot create a new user.\n");
				break;
			}
			user = (PUSER)malloc(sizeof(USER));
			init_user(user);
			printf("User allocated successfully.\n");
			break;
		case 2:
			if(!user) {
				printf("Please create a user first, and then assign them a username\n");
				break;
			}
			if(user && user->userName != NULL) {
				printf("Username already assigned. Cannot assign a new username..\n");
				break;
			}
			add_username(user);
			break;
		case 3:
			if(!user) {
				printf("Please create a user first, and then assign them a username\n");
				break;
			}
			print_user(user);
			break;
		case 4:
			if(!user) {
				printf("Please create a user first, and then assign them a username\n");
				break;
			}
			get_flag(user);
			break;
		default: printf("Thank you for using my program. Hope you had fun. Bye... ;)");
		}
		choice = -1;
	}

	return 0;
}