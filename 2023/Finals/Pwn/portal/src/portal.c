#include "user.h"

int LOGGED_IN = 0;
user_t *USER = NULL;
char admin_pwd[MAX_READ_SIZE];

void login() {
	char username[MAX_READ_SIZE];
	char password[MAX_READ_SIZE];
	char line[MAX_READ_SIZE];
	user_t *user = NULL;

	printf("Username: ");
	fgets(username, MAX_READ_SIZE, stdin);
	username[strlen(username)-1] = '\0';

	
	if(strcmp(username, "admin") != 0) {
		printf("Invalid username: ");
		printf(username);
		return;
	}

	printf("Password: ");
	fgets(password, MAX_READ_SIZE, stdin);
	password[strlen(password)-1] = '\0';

	if(strcmp(admin_pwd, password) != 0) {
		puts("[ERROR] - Invalid password.");
		return;
	}

	LOGGED_IN = 1;
	USER = user_create(username, password, DEFAULT_REGION);
	success("Logged in as %s.", USER->name);
}

void register_user() {
	printf("Due to the recent security breach, we have disabled user registration.\n");
}

void admin_menu() {
	int choice;
	char line[MAX_READ_SIZE];
	if(USER->name && USER->region) printf("\nLogged in as: %s [%s]\n\t\t = ADMIN MENU =\n", USER->name, USER->region);
	printf("1. Print user info\n");
	printf("2. Change username\n");
	printf("3. Change password\n");
	printf("4. Change region\n");
	printf("5. Read flag\n");
	printf("6. DELETE USER [Experimental]\n");
	printf("0. Exit\n");
	printf(">> ");
	range_input(&choice, 0, 6);

	switch(choice) {
		case 1:
			user_print_admin(*USER, USER);
			break;
		case 2:
			change_name(USER);
			break;
		case 3:
			change_password(USER);
			break;
		case 4:
			change_region(USER);
			break;
		case 5:
			read_flag(USER);
			break;
		case 6:
			user_destroy(USER);
			puts("User deleted.");
			break;
		case 0:
			exit(0);
			break;
		default:
			puts("[ERROR] - Invalid choice.");
			exit(1);
	}

}

void main_menu() {
    int choice;
    char line[MAX_READ_SIZE];

	if(USER) {
		admin_menu();
		return;
	}

	printf("\n\t\t = MENU =\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf(">> ");
    range_input(&choice, 1, 3);

    switch(choice) {
        case 1:
            login();
            break;
        case 2:
            register_user();
            break;
        case 3:
            exit(0);
            break;
        default:
            puts("[ERROR] - Invalid choice.");
            exit(1);
    }
}

int main() {

	// read_admin_password(admin_pwd); // It was buggy, so I got the password from environment, it is MUCH MUCH secure now.

	if(getenv("ADMIN_PASSWORD") == NULL) {
		puts("[ERROR] - Please contact @TheFlash2k. The admin-password wasn't generated on the remote instance.\n");
		exit(1);
	}

	strcpy(admin_pwd, getenv("ADMIN_PASSWORD"));

	printf("Welcome to the portal. Please identify yourself first.\n");
	while(true) main_menu();

	return 0;
}