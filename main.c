#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "window/window.h"
#include "server/server.h"

int cHealth;
int cArmor;
int cKills;

int main()
{
	pthread_t thread_window;
	pthread_t thread_server;
	printf("Before Thread\n");
	pthread_create(&thread_window, NULL, raylib, NULL);
	pthread_create(&thread_server, NULL, servermain, NULL);
	pthread_join(thread_window, NULL);
	printf("After Thread\n");
	exit(0);
}
