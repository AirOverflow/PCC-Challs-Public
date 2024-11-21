#include <stdio.h>
#include "flag.h"

void print_flag() {
	for(size_t i = 0; i < strlen(flag); i++) {
		if(flag[i] != '\n') {
			printf("%c", flag[i]);
		}
	}
	printf("\n");
}

int main(int argc, char* argv[]) {	
	build_flag();
	printf("Good one. Didn't know you had it in you tho.\n");
	print_flag();
}
