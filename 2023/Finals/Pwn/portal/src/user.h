#include "utils.h"

void check_region(user_t *user) {
    if(strcmp(user->region, ADMIN_REGION) == 0) {
        error("%s is not a valid region for you. Defaulting to %s.", ADMIN_REGION, DEFAULT_REGION);
        strcpy(user->region, DEFAULT_REGION);
    }
}

user_t *user_create(char *name, char *password, char *region) {
    user_t *user = (user_t*)malloc(sizeof(user_t));
    user->name = (char*)malloc(sizeof(char) * MAX_READ_SIZE);
    user->password = (char*)malloc(sizeof(char) * MAX_READ_SIZE);
    user->region = (char*)malloc(sizeof(char) * MAX_READ_SIZE);
    strcpy(user->name, name);
    strcpy(user->password, password);
    strcpy(user->region, region);
    check_region(user);
    user->is_admin = !(strcmp(user->name, "admin"));
    return user;
}

void user_destroy(user_t *user) {
    debug("=> User    : [%1$p] %s", user->name);
    free(user->name);
    debug("=> Password: [%1$p] %s", user->password);
    free(user->password);
    debug("=> Region  : [%1$p] %s", user->region);
    free(user->region);
}

void user_print(user_t *user, bool is_admin) {
    printf("Name: %s\n", user->name);
    printf("Region: %s\n", user->region);
    ((is_admin) ? printf("Password: %s\n", user->password) : printf("Password: ********\n"));
    printf("Is admin: %s\n", user->is_admin ? "true" : "false");
}

void user_print_admin(user_t req, user_t *user) {
    if(req.is_admin) {
        user_print(user, true);
    } else {
        printf("You are not an admin!\n");
    }
}

void change_password(user_t *user) {
    printf("Enter new password: ");
    user->region = (char*)malloc(sizeof(char) * MAX_READ_SIZE);
    get_input_str(user->password, MAX_READ_SIZE);
    printf("Password changed!\n");
}

void change_region(user_t *user) {
    printf("Enter new region: ");
    user->region = (char*)malloc(sizeof(char) * MAX_READ_SIZE);
    scanf("%s", user->region);
    printf("New Region: %s", user->region);
    check_region(user);
}

void change_name(user_t *user) {
    printf("Enter new name: ");
    user->name = (char*)malloc(sizeof(char) * MAX_READ_SIZE);
    get_input_str(user->name, MAX_READ_SIZE);
    printf("New name: %s\n", user->name);
}