#ifndef WINDOW_H
#define WINDOW_H
#include <pthread.h>

void* raylib(void *vargp);
extern int cHealth;
extern int cArmor;
extern int cKills;
short GetAsyncKeyState(int vKey);

#endif