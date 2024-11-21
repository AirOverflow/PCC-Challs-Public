#include <stdio.h>
#include <unistd.h>
#include "utils.h"

/*

    NOTE: This is not part of the challenge, however, if you somehow find a vulnerability here,
    and exploit this, please let me know. Ty.

    Due to the nature of my *container* and *CTFd-Flag-Validator* and all the dynamic-ness,
    I couldn't directly set Environment variables in my docker-image.

    To bypass that issue, I wrote this simple wrapper, that will run the binary with the
    ADMIN_PASSWORD environment variable. ;)
*/

int main() {

    char admin_pwd[MAX_READ_SIZE];
    if(getenv("ADMIN_PASSWORD") == NULL) {
        read_admin_password(admin_pwd);
        admin_pwd[strcspn(admin_pwd, "\n")] = 0;
        setenv("ADMIN_PASSWORD", admin_pwd, 1);
	}

    char *admin_pwd_env = getenv("ADMIN_PASSWORD");
    char *prefix = "ADMIN_PASSWORD=";
    char env[100];

    strcpy(env, prefix);
    strcat(env, admin_pwd_env);

    char *argv[] = {"/bin/sh", NULL};
    char *envp[] = {env, NULL};
    execve("/app/portal", argv, envp);
}