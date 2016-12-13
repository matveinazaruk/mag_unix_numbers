#include "io.h"

#include <errno.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define SOCK_PATH "echo_socket"
#define MIN 0
#define MAX 1000000000

void PrintUsage(const char* prog) {
    fprintf(stderr, "=== number guessing client ===\n");
    fprintf(stderr, "Usage: %s UNIX_SOCKET_PATH \n\n", prog);
}

bool Equals(const char result) {
    return result == '=';
}

bool Greater(const char result) {
    return result == '>';
}

bool Less(const char result) {
    return result == '<';
}

uint32_t BinarySearch(const int sockfd) {
    uint32_t left = MIN;
    uint32_t right = MAX;
    uint32_t toSend, guess;
    char result = ' ';

    while (!Equals(result) && left != right) {
        guess = (left + right) / 2;
        fprintf(stderr, "Client try to guess: %d\n", guess);
        toSend = htonl(guess);

        if (!SendAll(sockfd, (char*)&toSend, sizeof(toSend))) {
            fprintf(stderr, "send");
            break;
        }
            
        if (!RecvAll(sockfd, &result, sizeof(result))) {
            fprintf(stderr, "recv");
            break;
        }
        fprintf(stderr, "Result is: x %c %d\n", result, guess);

        if (Less(result)) {
            right = guess;
        } else if (Greater(result)) {
            left = guess;
        }

    }
    return guess;
}

int main(int argc, char* argv[])
{
    int sockfd;

    if (argc != 2) {
        PrintUsage(argv[0]);
        return 2;
    }

    const char* socketPath = argv[1];
    

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    fprintf(stderr, "Trying to connect...\n");

    struct sockaddr_un remote;
    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, socketPath);
    socklen_t remoteLen = sizeof(remote);
    
    if (connect(sockfd, (struct sockaddr *)&remote, remoteLen) == -1) {
        perror("connect");
        exit(1);
    }

    fprintf(stderr, "Connected.\n");

    uint32_t guess = BinarySearch(sockfd);
    fprintf(stdout, "%d\n", guess);

    if (close(sockfd) == -1) {
        perror("close socket");
        return 2;
    }
    return 0;
}
