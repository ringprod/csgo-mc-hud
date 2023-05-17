#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
//#include "../include/jsmn.h"
#include "../include/cJSON.h"

#define DELIMITERS " \t\n\":{,}"

extern int live;
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

    //jsmn_parser p;
    //jsmntok_t t[128];
    //jsmn_init(&p);

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
            cJSON* root = cJSON_ParseWithLength(request->value, request->length);
            // root
            if (root == NULL) {
                printf("Failed to parse JSON: %s\n", cJSON_GetErrorPtr());
            }

            // Access the 'map' object
            cJSON* map = cJSON_GetObjectItem(root, "map");
            if (map == NULL || !cJSON_IsObject(map)) {
                printf("Failed to retrieve 'map' object from JSON.\n");
            }

            // Access the 'phase' object
            cJSON* phase = cJSON_GetObjectItem(map, "phase");
            if (phase != NULL || cJSON_IsString(phase)) {
                printf("phase: %s\n", phase->valuestring);
                live = strcmp(phase->valuestring, "live") == 0;
            }

            // Access the 'player' object
            cJSON* player = cJSON_GetObjectItem(root, "player");
            if (player == NULL || !cJSON_IsObject(player)) {
                printf("Failed to retrieve 'player' object from JSON.\n");
            }

            // Access the 'state' object within 'player'
            cJSON* state = cJSON_GetObjectItem(player, "state");
            if (state == NULL || !cJSON_IsObject(state)) {
                printf("Failed to retrieve 'state' object from JSON.\n");
            }

            // Access the 'health' and 'armor' values within 'state'
            cJSON* health = cJSON_GetObjectItem(state, "health");
            if (health != NULL && cJSON_IsNumber(health)) {
                printf("Health: %d\n", health->valueint);
                cHealth = health->valueint;
            }

            cJSON* armor = cJSON_GetObjectItem(state, "armor");
            if (armor != NULL && cJSON_IsNumber(armor)) {
                printf("Armor: %d\n", armor->valueint);
                cArmor = armor->valueint;
            }
            cJSON_Delete(root);

            /*int curlyBracesCount = 0;
            int squareBracketsCount = 0;

            char* token = strtok(jsonStringCopy, "{[,]}");
            while (token != NULL) {
                if (strcmp(token, "{") == 0) {
                    curlyBracesCount++;
                }
                else if (strcmp(token, "}") == 0) {
                    curlyBracesCount--;
                }
                else if (strcmp(token, "[") == 0) {
                    squareBracketsCount++;
                }
                else if (strcmp(token, "]") == 0) {
                    squareBracketsCount--;
                }

                token = strtok(NULL, "{[,]}");
            }

            if (curlyBracesCount == 0 && squareBracketsCount == 0) {
                printf("Valid af!\n");
            }
            else {
                printf("NOOOOOOOOOO!\n");
            }

            int r = jsmn_parse(&p, jsonStringCopy, strlen(jsonStringCopy), t, 128);

            if (r < 0) {
                printf("Failed to parse JSON: %d\n", r);
            }*/
#if 0
            live = 1;
            char* token = strtok(request->value, DELIMITERS);
            while (token != NULL) {
                if (strcmp(token, "map") == 0) {
                    while (token != NULL && strcmp(token, "phase") != 0) {
                        token = strtok(NULL, DELIMITERS);  // Move to the next token
                    }
                    if (token != NULL) {
                        // Found the "phase" key
                        token = strtok(NULL, DELIMITERS);  // Move to the next token (value)
                        if (token != NULL) {
                            // Extract the Map's phase value
                            char phase[30];
                            strcpy(phase, token);
                            printf("Map's phase: %s\n", phase);
                            if (strcmp(phase, "live") == 0)
                            {
                                printf("phase is live!\n");
                                //islive = 1;
                            }
                            else
                            {
                                printf("phase is not live!\n");
                                //islive = 0;
                            }
                            //cHealth = health;
                        }
                        else {
                            printf("Failed to extract maps's phase.\n");
                        }
                    }
                    else {
                        printf("Map's phase not found.\n");
                    }
                }

                if (strcmp(token, "player") == 0) {
                    // Found the "player" key
                    //token = strtok(NULL, DELIMITERS);  // Move to the next token
                    while (token != NULL && strcmp(token, "activity") != 0) {
                        token = strtok(NULL, DELIMITERS);  // Move to the next token
                    }
                    if (token != NULL) {
                        // Found the "activiy" key
                        token = strtok(NULL, DELIMITERS);  // Move to the next token (value)
                        if (token != NULL) {
                            // Extract the player's activity value
                            char activity[30];
                            strcpy(activity, token);
                            printf("Player's activity: %s\n", activity);
                            //cHealth = health;
                        }
                        else {
                            printf("Failed to extract player's activity.\n");
                        }
                    }
                    else {
                        printf("Player's activity not found.\n");
                    }

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
#endif       
            printf("postd\n");
        }
        FreeRequest(request);

        closesocket(msg_sock);
        printf("closed\n");
    }

    WSACleanup();
	return NULL;
}