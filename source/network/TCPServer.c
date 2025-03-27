#include "headers/network/TCPServer.h"

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) // ################################ WINDOWS #################################################
/* Create a seperate thread to run the Server. */
HANDLE thread;

void enableNetworkInterface(void) {
    thread = CreateThread(NULL, 0, startTCPServer, NULL, 0, NULL);
    if (thread == NULL) {
        debug_log_critical(stderr, "HANDLE thread = CreateThread(NULL, 0, startTCPServer, NULL, 0, NULL)");
        exit(-1);
    }
}
void disableNetworkInterface(void) {
    WaitForSingleObject(thread, 0);
    CloseHandle(thread);
}
// Function designed to handle Client requests.
static void connectionHandler(unsigned int connfd) {
    char buff[MAX];

    recv(connfd, buff, sizeof(buff), 0);
    printf("Received: %s\n", buff);

    snprintf(buff, 121, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccept: */*;\r\nContent:Length: 1024\r\n\r\n<html><body>Hello World!</body></html>\r\n");
    send(connfd, buff, 121, 0);

    closesocket(connfd);
}
// Starting a tcp Server Listening at port: PORT.
DWORD WINAPI startTCPServer(void *args) {
    unsigned int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        debug_log_critical(stderr, "int err = WSAStartup(MAKEWORD(2, 2), &wsaData)");
        WSACleanup();
        exit(-1);
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        debug_log_critical(stderr, "sockfd == INVALID_SOCKET");
        WSACleanup();
        exit(-1);
    }

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");// htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        debug_log_critical(stderr, "bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR");
        WSACleanup();
        exit(-1);
    }

    // Now server is ready to listen and verification
    if (listen(sockfd, 5) == SOCKET_ERROR) {
        debug_log_critical(stderr, "listen(sockfd, 5) == SOCKET_ERROR");
        WSACleanup();
        exit(-1);
    }

    printf("IP address: %s:%d\n", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    printf("Server running...\n");

    len = sizeof(cli);
    // Accept the data packet from client and verification
    while ((connfd = accept(sockfd, (SA*)&cli, &len)) != INVALID_SOCKET) {
        connectionHandler(connfd);
    }

    // Close the socket.
    closesocket(sockfd);
    WSACleanup();

    //int *input = (int*)arg;

    return 0;
}
#else // ########################################################### LINUX ######################################################################
/* Create a seperate thread to run the Server. */
HANDLE thread;

void enableNetworkInterface(void) {
    thread = CreateThread(NULL, 0, startTCPServer, NULL, 0, NULL);
    if (thread == NULL) {
        debug_log_critical(stderr, "HANDLE thread = CreateThread(NULL, 0, startTCPServer, NULL, 0, NULL)");
        exit(-1);
    }
}
void disableNetworkInterface(void) {
    WaitForSingleObject(thread, 0);
    CloseHandle(thread);
}
// Function designed to handle Client requests.
static void connectionHandler(int connfd) {
    char buff[MAX];

    read(connfd, buff, sizeof(buff), 0);
    printf("Received: %s\n", buff);

    snprintf(buff, 121, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccept: */*;\r\nContent:Length: 1024\r\n\r\n<html><body>Hello World!</body></html>\r\n");
    write(connfd, buff, 121, 0);

    close(connfd);
}
// Starting a tcp Server Listening at port: PORT.
void *startTCPServer(void *args) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        debug_log_critical(stderr, "sockfd == -1\n");
        exit(-1);
    }

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) == -1) {
        debug_log_critical(stderr, "bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) == -1");
        exit(-1);
    }

    // Now server is ready to listen and verification
    if (listen(sockfd, 5) == -1) {
        debug_log_critical(stderr, "listen(sockfd, 5) == -1");
        exit(-1);
    }

    len = sizeof(cli);
    // Accept the data packet from client and verification
    while ((connfd = accept(sockfd, (SA*)&cli, &len) != -1 )
        // Function for chatting between client and server
        connectionHandler(connfd);

    close(sockfd);

    return 1;
}
#endif //!WINDOWS


