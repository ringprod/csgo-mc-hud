#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 3000 // HTTP Port
#define SERVER "127.0.0.1" // CSGO game server IP address

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char request[1024], response[4096];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket.\n");
        WSACleanup();
        return 1;
    }

    // Connect to server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER);
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Could not connect to server.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Prepare HTTP request
    sprintf(request, "GET /gamestate HTTP/1.1\r\nHost: %s:%d\r\n\r\n", SERVER, PORT);

    // Send HTTP request
    if (send(clientSocket, request, strlen(request), 0) == SOCKET_ERROR) {
        printf("Send failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Receive HTTP response
    if (recv(clientSocket, response, 4096, 0) == SOCKET_ERROR) {
        printf("Receive failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Process HTTP response
    printf("HTTP Response:\n%s\n", response);

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
