#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>

#pragma warning (disable : 4005)
#pragma warning (disable : 4172) // Line 22

#define TIME_BUFFER_SIZE 20
char timeBuffer[TIME_BUFFER_SIZE];

void gettime() {
	time_t currentTime;
	struct tm* timeInfo;
	time(&currentTime);
	timeInfo = localtime(&currentTime);
	strftime(timeBuffer, TIME_BUFFER_SIZE, "%d/%m/%Y-%H:%M:%S", timeInfo);
}

#define base(file, method, msg, ...) {\
	gettime();\
	fprintf(file, "[%s] ", method);\
	fprintf(file, msg, ##__VA_ARGS__);\
	fprintf(file, "\n");\
}

#define Log(x, ...) {\
	base(stdout, "\033[32mLOG\033[0m", x, ##__VA_ARGS__)\
}

#define Error(x, ...) {\
	base(stdout, "\033[31mERROR\033[0m", x, ##__VA_ARGS__)\
}

#define Info(x, ...) {\
	base(stdout, "\033[34mINFO\033[0m", x, ##__VA_ARGS__)\
}

#define Warn(x, ...) {\
	base(stdout, "\033[33mWARNING\033[0m", x, ##__VA_ARGS__)\
}

#define Debug(x, ...) {\
	base(stdout, "\033[35mDEBUG\033[0m", x, ##__VA_ARGS__)\
}
#define MSG(x, ...) {\
	printf(x, ##__VA_ARGS__);\
	printf("\n");\
}