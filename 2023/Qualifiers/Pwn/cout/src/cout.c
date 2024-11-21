#include <stdio.h>
#include <string.h>

__attribute__((constructor))
void setup(){
	// IGNORE ME
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
}

void init() {

    char name[0x100];
    char flag[0x100];
    FILE *file;

    // Read flag.txt into flag
    file = fopen("/app/flag.txt", "r");
    if (file == NULL) {
        printf("flag.txt not found. Contact an admin if this is happening on the remote server.\n");
        return;
    }
    fgets(flag, 0x100, file);
    fclose(file);

    printf("HOLD UP!!!\n");
    printf("I forgot to ask, what was your name again? ");
    scanf("%s", name);

    printf("Well, it's good to have you...");
    printf("Damn.. I keep forgetting..\n");
    printf(name);
    printf("\nThis is your name, right? ");
    scanf("%s", name);
    printf("Well, nice to meet you %s...\n", name);
}

int main() {
    init();
}