#include "headers/network/c_interface/TCPClient.h"

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) // ################################ WINDOWS #################################################
static void connectionHandler(int sockfd) {
    char buff[MAX] = { 0 };

    snprintf(buff, 18, "Server stop");
    send(sockfd, buff, 18, 0);

    char responce[MAX] = { 0 };
    recv(sockfd, responce, sizeof(responce), 0);
    printf("Client Received responce: {\n%s\n", responce);
    printf("}\n");
}
DWORD WINAPI startTCPClient(void *args) {
    unsigned int sockfd;
    struct sockaddr_in servaddr;

    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        debug_log_critical(stderr, "int err = WSAStartup(MAKEWORD(2, 2), &wsaData)");
        WSACleanup();
        exit(-1);
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        debug_log_critical(stderr, "sockfd == -1\n");
        closesocket(sockfd);
        WSACleanup();
        exit(-1);
    }

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) == 0) {
        connectionHandler(sockfd);
    } else {
        perror("Error");
    }

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
#else // ########################################################### LINUX ######################################################################
static void connectionHandler(int sockfd) {
    char buff[MAX] = { 0 };

    snprintf(buff, 18, "Server stop");
    write(sockfd, buff, 18);

    char responce[MAX] = { 0 };
    read(sockfd, responce, sizeof(responce));
    printf("Client Received responce: {\n%s\n", responce);
    printf("}\n");
}
void *startTCPClient(void *args) {
    socklen_t sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        debug_log_critical(stderr, "sockfd == -1\n");
        close(sockfd);
        exit(-1);
    }

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) == 0) {
        connectionHandler(sockfd);
    } else {
        perror("Error");
    }

    close(sockfd);

    int *input = (int*)args;
    return input;
}
#endif //!WINDOWS


