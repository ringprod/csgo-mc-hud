#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"

#define DELIMITERS " \t\n\":{,}"

extern int cHealth;
extern int cArmor;
extern int cKills;

void* servermain(void *vargp)
{
    int addr_len;
    struct sockaddr_in local, client_addr;

    SOCKET sock, msg_sock;
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
        error_die("WSAStartup()");

    // Fill in the address structure
    local.sin_family        = AF_INET;
    local.sin_addr.s_addr   = INADDR_ANY;
    local.sin_port          = htons(DEFAULT_PORT);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
        error_die("socket()");

    if (bind(sock, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
        error_die("bind()");

    if (listen(sock, 10) == SOCKET_ERROR)
        error_die("listen()");

    printf("Waiting for connection...\n");

    int count = 0;

    while (1)
    {
        addr_len = sizeof(client_addr);
        msg_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);

        if (msg_sock == INVALID_SOCKET || msg_sock == -1)
            error_die("accept()");

        printf("\n\n#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$ %d\n\n", ++count);
        //printf("Connected to %s:%d\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));

        REQUEST *request = GetRequest(msg_sock);
        //printf("Client requested %d %s\n", request->type, request->value);

        if (request->type == POST)
        {
            char* token = strtok(request->value, DELIMITERS);
            while (token != NULL) {
                if (strcmp(token, "player") == 0) {
                    // Found the "player" key
                    token = strtok(NULL, DELIMITERS);  // Move to the next token
                    while (token != NULL && strcmp(token, "health") != 0) {
                        token = strtok(NULL, DELIMITERS);  // Move to the next token
                    }
                    if (token != NULL) {
                        // Found the "health" key
                        token = strtok(NULL, DELIMITERS);  // Move to the next token (value)
                        if (token != NULL) {
                            // Extract the player's health value
                            int health = atoi(token);
                            printf("Player's health: %d\n", health);
                            cHealth = health;
                        }
                        else {
                            printf("Failed to extract player's health.\n");
                        }
                    }
                    else {
                        printf("Player's health not found.\n");
                    }

                    while (token != NULL && strcmp(token, "armor") != 0) {
                        token = strtok(NULL, DELIMITERS);  // Move to the next token
                    }
                    if (token != NULL) {
                        // Found the "armor" key
                        token = strtok(NULL, DELIMITERS);  // Move to the next token (value)
                        if (token != NULL) {
                            // Extract the player's armor value
                            int armor = atoi(token);
                            printf("Player's armor: %d\n", armor);
                            cArmor = armor;
                        }
                        else {
                            printf("Failed to extract player's armor.\n");
                        }
                    }
                    else {
                        printf("Player's armor not found.\n");
                    }

                    while (token != NULL && strcmp(token, "kills") != 0) {
                        token = strtok(NULL, DELIMITERS);  // Move to the next token
                    }
                    if (token != NULL) {
                        // Found the "kills" key
                        token = strtok(NULL, DELIMITERS);  // Move to the next token (value)
                        if (token != NULL) {
                            // Extract the player's kills value
                            int kills = atoi(token);
                            printf("Player's kills: %d\n", kills);
                            cKills = kills;
                        }
                        else {
                            printf("Failed to extract player's kills.\n");
                        }
                    }
                    else {
                        printf("Player's kills not found.\n");
                    }
                    break;
                }
                token = strtok(NULL, DELIMITERS);  // Move to the next token
            }

            printf("postd\n");
        }
        FreeRequest(request);

        closesocket(msg_sock);
        printf("closed\n");
    }

    WSACleanup();
	return NULL;
}