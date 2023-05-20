#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "gamedata.h"
#include "../include/cJSON.h"

extern GameData gameData;

#include <stdio.h>

void printGameData() {
    printf("Map:\n");
    printf("\tPhase: %s\n", gameData.map.phase);
    printf("\tRound: %d\n", gameData.map.round);

    printf("\nRound:\n");
    printf("\tPhase: %s\n", gameData.round.phase);
    printf("\tBomb: %s\n", gameData.round.bomb);

    printf("\nPlayer:\n");
    printf("\tName: %s\n", gameData.player.name);
    printf("\tActivity: %s\n", gameData.player.activity);

    printf("State:\n");
    printf("\tHealth: %d\n", gameData.player.state.health);
    printf("\tArmor: %d\n", gameData.player.state.armor);
    printf("\tBurning: %d\n", gameData.player.state.burning);
    printf("\tRound Kills: %d\n", gameData.player.state.round_kills);
    printf("\tRound Kill Headshots: %d\n", gameData.player.state.round_killhs);

    printf("Match Stats:\n");
    printf("\tKills: %d\n", gameData.player.match_stats.kills);
    printf("\tDeaths: %d\n", gameData.player.match_stats.deaths);
    printf("\tMVPS: %d\n", gameData.player.match_stats.mvps);

    printf("Weapons:\n");
    printf("Count: %I64d\n", gameData.player.weapons.count);
    for (size_t i = 0; i < gameData.player.weapons.count; i++) {
        Weapon weapon = gameData.player.weapons.weaponArray[i];
        printf("\tWeapon %I64d:\n", i + 1);
        printf("\t\tName: %s\n", weapon.name);
        printf("\t\tHas Skin: %d\n", weapon.hasSkin);
        printf("\t\tType: %s\n", weapon.type);
        printf("\t\tIs Active: %d\n", weapon.isActive);
        printf("\t\tAmmo Clip: %d\n", weapon.ammo_clip);
        printf("\t\tAmmo Clip Max: %d\n", weapon.ammo_clip_max);
        printf("\t\tAmmo Reserve: %d\n", weapon.ammo_reserve);
        printf("\n");
    }
}

void parseWeapons(const cJSON* weapons) {
    free(gameData.player.weapons.weaponArray);
    gameData.player.weapons.count = 0;

    int count = cJSON_GetArraySize(weapons);
    gameData.player.weapons.count = count;
    gameData.player.weapons.weaponArray = malloc(sizeof(Weapon) * count);

    for (int i = 0; i < count; i++) {
        cJSON* weapon = cJSON_GetArrayItem(weapons, i);

        /*cJSON* name = cJSON_GetObjectItem(weapon, "name");
        if (name != NULL || cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            const char* src = name->valuestring;
            size_t length = strlen(src) + 1;
            gameData.player.weapons.weaponArray[i].name = malloc(length);
            strcpy_s(gameData.player.weapons.weaponArray[i].name, length, name->valuestring);
        }*/
        cJSON* name = cJSON_GetObjectItem(weapon, "name");
        if (name != NULL && cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            const char* src = name->valuestring;
            size_t length = strlen(src) + 1;
            gameData.player.weapons.weaponArray[i].name = malloc(length);
            if (gameData.player.weapons.weaponArray[i].name != NULL) {
                if (strcpy_s(gameData.player.weapons.weaponArray[i].name, length, src) != 0) {
                    // Error occurred during string copy
                    printf("Failed to copy name string.\n");
                    free(gameData.player.weapons.weaponArray[i].name);
                    gameData.player.weapons.weaponArray[i].name = NULL;
                }
            }
            else {
                // Error allocating memory
                printf("Failed to allocate memory for name string.\n");
            }
        }

        cJSON* paintkit = cJSON_GetObjectItem(weapon, "paintkit");
        if (paintkit != NULL || cJSON_IsString(paintkit)) {
            printf("paintkit: %s\n", paintkit->valuestring);
            gameData.player.weapons.weaponArray[i].hasSkin = strcmp(paintkit->valuestring, "default");
        }

        cJSON* type = cJSON_GetObjectItem(weapon, "type");
        if (type != NULL || cJSON_IsString(type)) {
            printf("type: %s\n", type->valuestring);
            const char* src = type->valuestring;
            size_t length = strlen(src) + 1;
            gameData.player.weapons.weaponArray[i].type = malloc(length);
            strcpy_s(gameData.player.weapons.weaponArray[i].type, length, type->valuestring);
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
        else
        {
            gameData.player.weapons.weaponArray[i].ammo_clip = 0;
        }

        cJSON* ammo_clip_max = cJSON_GetObjectItem(weapon, "ammo_clip_max");
        if (ammo_clip_max != NULL || cJSON_IsString(ammo_clip_max)) {
            printf("ammo_clip_max: %d\n", ammo_clip_max->valueint);
            gameData.player.weapons.weaponArray[i].ammo_clip_max = ammo_clip_max->valueint;
        }
        else
        {
            gameData.player.weapons.weaponArray[i].ammo_clip_max = 0;
        }

        cJSON* ammo_reserve = cJSON_GetObjectItem(weapon, "ammo_reserve");
        if (ammo_reserve != NULL || cJSON_IsString(ammo_reserve)) {
            printf("ammo_reserve: %d\n", ammo_reserve->valueint);
            gameData.player.weapons.weaponArray[i].ammo_reserve = ammo_reserve->valueint;
        }
        else
        {
            gameData.player.weapons.weaponArray[i].ammo_reserve = 0;
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
            const char* src = phase->valuestring;
            size_t length = strlen(src) + 1;
            gameData.map.phase = malloc(length);
            strcpy_s(gameData.map.phase, length, phase->valuestring);
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
            const char* src = phase->valuestring;
            size_t length = strlen(src) + 1;
            gameData.round.phase = malloc(length);
            strcpy_s(gameData.round.phase, length, phase->valuestring);
        }
        // Access the 'bomb' object within 'round'
        cJSON* bomb = cJSON_GetObjectItem(round, "bomb");
        if (bomb != NULL || cJSON_IsString(bomb)) {
            printf("bomb: %s\n", bomb->valuestring);
            const char* src = bomb->valuestring;
            size_t length = strlen(src) + 1;
            gameData.round.bomb = malloc(length);
            strcpy_s(gameData.round.bomb, length, bomb->valuestring);
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
        /*cJSON* name = cJSON_GetObjectItem(player, "name");
        if (name != NULL || cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            strcpy_s(gameData.player.name, strlen(name->valuestring) + 1, name->valuestring);
        }*/

        cJSON* name = cJSON_GetObjectItem(player, "name");
        if (name != NULL && cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            const char* src = name->valuestring;
            size_t length = strlen(src) + 1;
            gameData.player.name = malloc(length);
            if (gameData.player.name != NULL) {
                if (strcpy_s(gameData.player.name, length, src) != 0) {
                    // Error occurred during string copy
                    printf("Failed to copy name string.\n");
                    free(gameData.player.name);
                    gameData.player.name = NULL;
                }
            }
            else {
                // Error allocating memory
                printf("Failed to allocate memory for name string.\n");
            }
        }

        // Access the 'activity' object within 'player'
        /*cJSON* activity = cJSON_GetObjectItem(player, "activity");
        if (activity != NULL || cJSON_IsString(activity)) {
            printf("activity: %s\n", activity->valuestring);
            strcpy_s(gameData.player.activity, strlen(activity->valuestring) + 1, activity->valuestring);
        }*/

        cJSON* activity = cJSON_GetObjectItem(player, "activity");
        if (activity != NULL && cJSON_IsString(activity)) {
            printf("activity: %s\n", activity->valuestring);
            const char* src = activity->valuestring;
            size_t length = strlen(src) + 1;
            gameData.player.activity = malloc(length);
            if (gameData.player.activity != NULL) {
                if (strcpy_s(gameData.player.activity, length, src) != 0) {
                    // Error occurred during string copy
                    printf("Failed to copy activity string.\n");
                    free(gameData.player.activity);
                    gameData.player.activity = NULL;
                }
            }
            else {
                // Error allocating memory
                printf("Failed to allocate memory for activity string.\n");
            }
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


void FreeRequest(REQUEST* request)
{
    free(request->value);
    free(request);
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
                printf("\njson::\n%s\n^^end of json^^\n", cJSON_Print(root));
            if (root == NULL || root == 0) {
                printf("Failed to parse JSON: %s\n", cJSON_GetErrorPtr());
            }
            else
            {
                parseJSON(root);
            }
            printf("\n\ngame data dump :\n ");
            printGameData();
            printf("\n\nend of data dump.\n");

            cJSON_Delete(root);

            printf("postd\n");
        }
        FreeRequest(request);

        closesocket(msg_sock);
        printf("closed\n");
    }

    WSACleanup();
	return NULL;
}