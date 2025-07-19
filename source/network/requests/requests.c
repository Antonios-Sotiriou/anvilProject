#include "headers/network/requests/requests.h"

#define SA struct sockaddr

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) // ################################ WINDOWS #################################################
/* Send a TCP request to ip_address, on port. */
int sendRequest(const char ip_address[], const uint16_t port, const char req[]) {
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
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
    servaddr.sin_port = htons(port);//(uint16_t)atoi(port);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) == 0) {
        // anvil_snprinf(req, sizeof(req), "Server quit");
        send(sockfd, req, 18, 0);

        char responce[80];
        recv(sockfd, responce, sizeof(responce), 0);
        printf("Client Received responce: {\n%s\n", responce);
        printf("}\n\n");
    } else {
        perror("Error");
    }

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
#else // ########################################################### LINUX ######################################################################
/* Send a TCP request to ip_address, on port. */
int sendRequest(const char ip_address[], const uint16_t port, const char req[]) {
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
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
    servaddr.sin_port = htons(port);//(uint16_t)atoi(port);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) == 0) {
        // anvil_snprinf(req, sizeof(req), "Server quit");
        write(sockfd, req, 18);

        char responce[80];
        read(sockfd, responce, sizeof(responce));
        printf("Client Received responce: {\n%s\n", responce);
        printf("}\n\n");
    } else {
        perror("Error");
    }
    
    close(sockfd);
    return 0;
}
#endif //!WINDOWS


