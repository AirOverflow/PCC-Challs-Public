// Compile,g++ -o entity-encoder entity-encoder.cxx
#include <iostream>
#include <string>
#include <unordered_map>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define MAX_SZ 0x100
#define TO_COPY 3

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    signal(SIGALRM, exit);
    alarm(0x20);
}

std::unordered_map<char, std::string> decoder = {
    { '{',"%7D" }, { '}',"%7B" }, { '?',"%3F" }, { '!',"%21" }, { '%',"%25" }, { '^',"%5E" }, { '&',"%26" }, { '=',"%3D" }, { '+',"%2B" }, { '|',"%7C" }, { '\\',"%5C" }, { '/',"%2F" }, { ' ', "%20" }
};

int main(int argc, char* argv[], char* envp[]) {

    char curr;
    char element[TO_COPY+1] = { "\x00" };
    __int64_t i = 0, j, len;
    char buf[MAX_SZ] = { "\x00" };

    while(true) {
        memset(buf, 0x0, MAX_SZ);
        std::cout << "(Type \"exit\" to exit)\nEnter the string to url-encode: ";
        len = read(STDIN_FILENO, buf, MAX_SZ-1);

        buf[strcspn(buf, "\n")] = ' ';
        if(len) buf[len] = ' ';

        if(strncmp(buf, "exit", 4) == 0)
            break;

        std::cout << "Non-Encoded string: " << buf << std::endl;

        i = 0;
        while(true) {
        
            curr = buf[i];
            if(i > len)
                break;

            if(decoder.find(curr) == decoder.end() || !curr) {
                ++i;
                continue;
            }

            strncpy(element, decoder[curr].c_str(), TO_COPY);

            for(j = len; j > i; --j)
                buf[j + (TO_COPY-1)] = buf[j];
            
            for(j = 0; j < TO_COPY; ++j)
                buf[i + j] = element[j];

            len += (TO_COPY-1);
            i += TO_COPY;
        }

        std::cout << "Encoded string: " << buf << std::endl;
    }

    return 0;
}