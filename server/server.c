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
    printf("Phase: %s\n", gameData.map.phase);
    printf("Round: %d\n", gameData.map.round);

    printf("\nRound:\n");
    printf("Phase: %s\n", gameData.round.phase);
    printf("Bomb: %s\n", gameData.round.bomb);

    printf("\nPlayer:\n");
    printf("Name: %s\n", gameData.player.name);
    printf("Activity: %s\n", gameData.player.activity);

    printf("State:\n");
    printf("Health: %d\n", gameData.player.state.health);
    printf("Armor: %d\n", gameData.player.state.armor);
    printf("Burning: %d\n", gameData.player.state.burning);
    printf("Round Kills: %d\n", gameData.player.state.round_kills);
    printf("Round Kill Headshots: %d\n", gameData.player.state.round_killhs);

    printf("Match Stats:\n");
    printf("Kills: %d\n", gameData.player.match_stats.kills);
    printf("Deaths: %d\n", gameData.player.match_stats.deaths);
    printf("MVPS: %d\n", gameData.player.match_stats.mvps);

    printf("Weapons:\n");
    printf("Count: %zu\n", gameData.player.weapons.count);
    for (size_t i = 0; i < gameData.player.weapons.count; i++) {
        Weapon weapon = gameData.player.weapons.weaponArray[i];
        printf("Weapon %d:\n", i + 1);
        printf("Name: %s\n", weapon.name);
        printf("Has Skin: %d\n", weapon.hasSkin);
        printf("Type: %s\n", weapon.type);
        printf("Is Active: %d\n", weapon.isActive);
        printf("Ammo Clip: %d\n", weapon.ammo_clip);
        printf("Ammo Clip Max: %d\n", weapon.ammo_clip_max);
        printf("Ammo Reserve: %d\n", weapon.ammo_reserve);
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

        cJSON* name = cJSON_GetObjectItem(weapon, "name");
        if (name != NULL && cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            free(gameData.player.weapons.weaponArray[i].name);
            gameData.player.weapons.weaponArray[i].name = strdup(name->valuestring);
        }

        cJSON* paintkit = cJSON_GetObjectItem(weapon, "paintkit");
        if (paintkit != NULL && cJSON_IsString(paintkit)) {
            printf("paintkit: %s\n", paintkit->valuestring);
            gameData.player.weapons.weaponArray[i].hasSkin = strcmp(paintkit->valuestring, "default");
        }

        cJSON* type = cJSON_GetObjectItem(weapon, "type");
        if (type != NULL && cJSON_IsString(type)) {
            printf("type: %s\n", type->valuestring);
            free(gameData.player.weapons.weaponArray[i].type);
            gameData.player.weapons.weaponArray[i].type = strdup(type->valuestring);
        }

        cJSON* state = cJSON_GetObjectItem(weapon, "state");
        if (state != NULL && cJSON_IsString(state)) {
            printf("state: %s\n", state->valuestring);
            gameData.player.weapons.weaponArray[i].isActive = !strcmp(state->valuestring, "active");
        }

        cJSON* ammo_clip = cJSON_GetObjectItem(weapon, "ammo_clip");
        if (ammo_clip != NULL && cJSON_IsString(ammo_clip)) {
            printf("ammo_clip: %d\n", ammo_clip->valueint);
            gameData.player.weapons.weaponArray[i].ammo_clip = ammo_clip->valueint;
        }

        cJSON* ammo_clip_max = cJSON_GetObjectItem(weapon, "ammo_clip_max");
        if (ammo_clip_max != NULL && cJSON_IsString(ammo_clip_max)) {
            printf("ammo_clip_max: %d\n", ammo_clip_max->valueint);
            gameData.player.weapons.weaponArray[i].ammo_clip_max = ammo_clip_max->valueint;
        }
        
        cJSON* ammo_reserve = cJSON_GetObjectItem(weapon, "ammo_reserve");
        if (ammo_reserve != NULL && cJSON_IsString(ammo_reserve)) {
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
        if (phase != NULL && cJSON_IsString(phase)) {
            printf("phase: %s\n", phase->valuestring);
            free(gameData.map.phase);
            gameData.map.phase = strdup(phase->valuestring);
        }
        // Access the 'round' object within 'map'
        cJSON* round = cJSON_GetObjectItem(map, "round");
        if (round != NULL && cJSON_IsNumber(round)) {
            printf("round: %d\n", round->valueint);
            gameData.map.round = round->valueint;
        }
    }

    // Acess the 'round' object
    cJSON* round = cJSON_GetObjectItem(root, "round");
    if (round == NULL && !cJSON_IsObject(round)) {
        printf("Failed to retrieve 'round' object from JSON.\n");
    }
    else
    {
        // Access the 'phase' object within 'round'
        cJSON* phase = cJSON_GetObjectItem(round, "phase");
        if (phase != NULL && cJSON_IsString(phase)) {
            printf("phase: %s\n", phase->valuestring);
            free(gameData.round.phase);
            gameData.round.phase = strdup(phase->valuestring);
        }
        // Access the 'bomb' object within 'round'
        cJSON* bomb = cJSON_GetObjectItem(round, "bomb");
        if (bomb != NULL && cJSON_IsString(bomb)) {
            printf("bomb: %s\n", bomb->valuestring);
            free(gameData.round.bomb);
            gameData.round.bomb = strdup(bomb->valuestring);
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
        if (name != NULL && cJSON_IsString(name)) {
            printf("name: %s\n", name->valuestring);
            free(gameData.player.name);
            gameData.player.name = strdup(name->valuestring);
        }

        // Access the 'activity' object within 'player'
        cJSON* activity = cJSON_GetObjectItem(player, "activity");
        if (activity != NULL && cJSON_IsString(activity)) {
            printf("activity: %s\n", activity->valuestring);
            free(gameData.player.activity);
            gameData.player.activity = strdup(activity->valuestring);
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