#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "gamedata.h"
#include "../include/cJSON.h"

extern GameData gameData;

void parseWeapons(const cJSON* weapons) {
    free(gameData.player.weapons.weaponArray);
    gameData.player.weapons.count = 0;

    int count = cJSON_GetArraySize(weapons);
    gameData.player.weapons.count = count;
    gameData.player.weapons.weaponArray = malloc(sizeof(Weapon) * count);

    for (int i = 0; i < count; i++) {
        cJSON* weapon = cJSON_GetArrayItem(weapons, i);

        cJSON* name = cJSON_GetObjectItem(weapon, "name");
        if (name != NULL || cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            gameData.player.weapons.weaponArray[i].name = name->valuestring;
        }

        cJSON* paintkit = cJSON_GetObjectItem(weapon, "paintkit");
        if (paintkit != NULL || cJSON_IsString(paintkit)) {
            printf("paintkit: %s\n", paintkit->valuestring);
            gameData.player.weapons.weaponArray[i].hasSkin = strcmp(paintkit->valuestring, "default");
        }

        cJSON* type = cJSON_GetObjectItem(weapon, "type");
        if (type != NULL || cJSON_IsString(type)) {
            printf("type: %s\n", type->valuestring);
            gameData.player.weapons.weaponArray[i].type = type->valuestring;
        }

        cJSON* state = cJSON_GetObjectItem(weapon, "state");
        if (state != NULL || cJSON_IsString(state)) {
            printf("state: %s\n", state->valuestring);
            gameData.player.weapons.weaponArray[i].isActive = !strcmp(state->valuestring, "active");
        }

        cJSON* ammo_clip = cJSON_GetObjectItem(weapon, "ammo_clip");
        if (ammo_clip != NULL || cJSON_IsString(ammo_clip)) {
            printf("ammo_clip: %d\n", ammo_clip->valueint);
            gameData.player.weapons.weaponArray[i].ammo_clip = ammo_clip->valueint;
        }

        cJSON* ammo_clip_max = cJSON_GetObjectItem(weapon, "ammo_clip_max");
        if (ammo_clip_max != NULL || cJSON_IsString(ammo_clip_max)) {
            printf("ammo_clip_max: %d\n", ammo_clip_max->valueint);
            gameData.player.weapons.weaponArray[i].ammo_clip_max = ammo_clip_max->valueint;
        }
        
        cJSON* ammo_reserve = cJSON_GetObjectItem(weapon, "ammo_reserve");
        if (ammo_reserve != NULL || cJSON_IsString(ammo_reserve)) {
            printf("ammo_reserve: %d\n", ammo_reserve->valueint);
            gameData.player.weapons.weaponArray[i].ammo_reserve = ammo_reserve->valueint;
        }

        printf("%d weapon = %s\n", i, name->valuestring);
        printf("-name: %s,\nhasSkin: % d,\ntype : %s,\nisActive : %d\nammo_clip : %d,\nammo_clip_max : %d,\nammo_reserve : %d\n",
            gameData.player.weapons.weaponArray[i].name,
            gameData.player.weapons.weaponArray[i].hasSkin,
            gameData.player.weapons.weaponArray[i].type,
            gameData.player.weapons.weaponArray[i].isActive,
            gameData.player.weapons.weaponArray[i].ammo_clip,
            gameData.player.weapons.weaponArray[i].ammo_clip_max,
            gameData.player.weapons.weaponArray[i].ammo_reserve
        );
    }
}

void parseJSON(const cJSON* root) {

    // Access the 'map' object
    cJSON* map = cJSON_GetObjectItem(root, "map");
    if (map == NULL || !cJSON_IsObject(map)) {
        printf("Failed to retrieve 'map' object from JSON.\n");
    }
    else
    {
        // Access the 'phase' object within 'map'
        cJSON* phase = cJSON_GetObjectItem(map, "phase");
        if (phase != NULL || cJSON_IsString(phase)) {
            printf("phase: %s\n", phase->valuestring);
            gameData.map.phase = phase->valuestring;
        }
        // Access the 'round' object within 'map'
        cJSON* round = cJSON_GetObjectItem(map, "round");
        if (round != NULL || cJSON_IsNumber(round)) {
            printf("round: %d\n", round->valueint);
            gameData.map.round = round->valueint;
        }
    }

    // Acess the 'round' object
    cJSON* round = cJSON_GetObjectItem(root, "round");
    if (round == NULL || !cJSON_IsObject(round)) {
        printf("Failed to retrieve 'round' object from JSON.\n");
    }
    else
    {
        // Access the 'phase' object within 'round'
        cJSON* phase = cJSON_GetObjectItem(round, "phase");
        if (phase != NULL || cJSON_IsString(phase)) {
            printf("phase: %s\n", phase->valuestring);
            gameData.round.phase = phase->valuestring;
        }
        // Access the 'bomb' object within 'round'
        cJSON* bomb = cJSON_GetObjectItem(round, "bomb");
        if (bomb != NULL || cJSON_IsString(bomb)) {
            printf("bomb: %s\n", bomb->valuestring);
            gameData.round.bomb = bomb->valuestring;
        }
    }

    // Access the 'player' object
    cJSON* player = cJSON_GetObjectItem(root, "player");
    if (player == NULL || !cJSON_IsObject(player)) {
        printf("Failed to retrieve 'player' object from JSON.\n");
    }
    else
    {
        // Access the 'name' object within 'player'
        cJSON* name = cJSON_GetObjectItem(player, "name");
        if (name != NULL || cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            gameData.player.name = name->valuestring;
        }

        // Access the 'activity' object within 'player'
        cJSON* activity = cJSON_GetObjectItem(player, "activity");
        if (activity != NULL || cJSON_IsString(activity)) {
            printf("activity: %s\n", activity->valuestring);
            gameData.player.activity = activity->valuestring;
        }

        // Access the 'state' object within 'player'
        cJSON* state = cJSON_GetObjectItem(player, "state");
        if (state == NULL || !cJSON_IsObject(state)) {
            printf("Failed to retrieve 'state' object from JSON.\n");
        }
        else
        {
            // Access the 'health' values within 'state'
            cJSON* health = cJSON_GetObjectItem(state, "health");
            if (health != NULL && cJSON_IsNumber(health)) {
                printf("health: %d\n", health->valueint);
                gameData.player.state.health = health->valueint;
                //cHealth = health->valueint;
            }
            else
            {
                gameData.player.state.health = 100;
            }

            // Access the 'armor' values within 'state'
            cJSON* armor = cJSON_GetObjectItem(state, "armor");
            if (armor != NULL && cJSON_IsNumber(armor)) {
                printf("armor: %d\n", armor->valueint);
                gameData.player.state.armor = armor->valueint;
                //cArmor = armor->valueint;
            }

            // Access the 'burning' values within 'state'
            cJSON* burning = cJSON_GetObjectItem(state, "burning");
            if (burning != NULL && cJSON_IsNumber(burning)) {
                printf("burning: %d\n", burning->valueint);
                gameData.player.state.burning = burning->valueint;
            }

            // Access the 'round_kills' values within 'state'
            cJSON* round_kills = cJSON_GetObjectItem(state, "round_kills");
            if (round_kills != NULL && cJSON_IsNumber(round_kills)) {
                printf("round_kills: %d\n", round_kills->valueint);
                gameData.player.state.round_kills = round_kills->valueint;
            }
            
            // Access the 'round_killhs' values within 'state'
            cJSON* round_killhs = cJSON_GetObjectItem(state, "round_killhs");
            if (round_killhs != NULL && cJSON_IsNumber(round_killhs)) {
                printf("round_killhs: %d\n", round_killhs->valueint);
                gameData.player.state.round_killhs = round_killhs->valueint;
            }

            // Access the 'weapons' object within 'player'
            cJSON* weapons = cJSON_GetObjectItem(player, "weapons");
            if (weapons == NULL || !cJSON_IsObject(weapons)) {
                printf("Failed to retrieve 'weapons' object from JSON.\n");
            }
            else
            {
                printf("WEAPONS COUNT: %d\n", cJSON_GetArraySize(weapons));
                parseWeapons(weapons);
            }
        }
    }
}


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
            cJSON* root = cJSON_ParseWithLength(request->value, request->length);

            // root
            if (root == NULL) {
                printf("Failed to parse JSON: %s\n", cJSON_GetErrorPtr());
            }
            else
            {
                parseJSON(root);
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