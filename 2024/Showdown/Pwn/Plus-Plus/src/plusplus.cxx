// g++ -o plusplus plusplus.cxx -w -std=c++23 -lfmt -fno-stack-protector -fPIE

#define NDEBUG

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fmt/core.h>

#define MAX_BUF_SIZE 0x100
#define MAX_PRINT_SZ 0x50

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

int safe_str_input(char *buffer, int size) {
    int sz = -1;
    while(buffer[sz] != '\n') { buffer[++sz] = getchar(); }
    buffer[sz] = 0;
    return sz;
}

int main() {

    // Run me once, shame on you. Run me twice, shame on me.
    char buf[MAX_BUF_SIZE];
    char buf_2[MAX_BUF_SIZE];
    snprintf(buf_2, MAX_BUF_SIZE, "%p", &setvbuf); // help-help-help
    char output[MAX_BUF_SIZE];

    memset(output, 0, MAX_BUF_SIZE);
    for(int i = 0; i < 2; ++i) {

        for(int j=0;j<=i;++j) std::cout << "+";
        std::cout << " ";
        if(safe_str_input(buf, MAX_BUF_SIZE) < 0) break;
        if(i) break;
        size_t sz = strlen(buf);
        if(sz <= 1) continue;
        buf[(sz >= MAX_BUF_SIZE) ? MAX_BUF_SIZE-1 : strcspn(buf, "\n")] = 0;
        fmt::format_to(output, fmt::runtime(buf), MAX_PRINT_SZ);
        std::cout << std::string(output) << std::endl;
    }

    return 0;

}