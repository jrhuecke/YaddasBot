#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>

typedef struct Player Player;

struct Player {
    char *name;
    float dealt;
    float taken;
};

Player *player_create(char *name);

void player_delete(Player **p);

#endif
