#include "player.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function to copy a string to new memory
char *name_copy(char *word) {
    if (word) {
        //allocates space based on size of input string
        uint32_t len = strlen(word);
        char *new = (char *) malloc(len + 1);
        //copies chars over byte at a time
        for (uint32_t i = 0; i < len; i++) {
            new[i] = word[i];
        }
        //adds a null terminator to end of string
        new[len] = '\0';
        return new;
    } else {
        return NULL;
    }
}

typedef struct Player Player;

//creates a node, player name is copied over to new memory before
//being put into the node
Player *player_create(char *name) {
    Player *p = (Player *) malloc(sizeof(Player));
    if (p) {
        if (name) {
            p->name = name_copy(name);
        } else {
            p->name = NULL;
        }
	p->dealt = 0;
	p->taken = 0;
        return p;
    } else {
        return NULL;
    }
}

//deletes the node along with the copied words
void player_delete(Player **p) {
    if (*p) {
        if ((*p)->name) {
            free((*p)->name);
        }
        free(*p);
        *p = NULL;
    }
    return;
}

