#include "node.h"
#include "dict.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function to copy a string to new memory
char *word_copy(char *word) {
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

typedef struct Node Node;

//creates a node, player name is copied over to new memory before
//being put into the node
Node *node_create(char *player) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        if (player) {
            n->player = word_copy(player);
        } else {
            n->player = NULL;
        }
	n->potions = 0;
	n->pearls = 0;
	n->dealt = 0;
	n->taken = 0;
	n->food = 0;
	n->ingots = 0;
	n->kills = 0;
	n->deaths = 0;
	n->charges = 0;
        n->next = NULL;
        n->prev = NULL;
	n->hitters = dict_create(200);
	n->recievers = dict_create(200);
        return n;
    } else {
        return NULL;
    }
}

//deletes the node along with the copied words
void node_delete(Node **n) {
    if (*n) {
        if ((*n)->player) {
            free((*n)->player);
        }
	dict_delete(&((*n)->hitters));
	dict_delete(&((*n)->recievers));
        free(*n);
        *n = NULL;
    }
    return;
}

void node_swap(Node *a, Node *b) {
    //temp storing a's data
    char *tplayer = a->player;
    uint32_t tpotions = a->potions;
    uint32_t tpearls = a->pearls;
    float tdealt = a->dealt;
    float ttaken = a->taken;
    uint32_t tfood = a->food;
    uint32_t tingots = a->ingots;
    uint32_t tkills = a->kills;
    uint32_t tdeaths = a->deaths;
    uint32_t tcharges = a->charges;
    Dict *thitters = a->hitters;
    Dict *trecievers = a->recievers;
    //swapping b's data over to a
    a->player = b->player;
    a->potions = b->potions;
    a->pearls = b->pearls;
    a->dealt = b->dealt;
    a->taken = b->taken;
    a->food = b->food;
    a->ingots = b->ingots;
    a->kills = b->kills;
    a->deaths = b->deaths;
    a->charges = b->charges;
    a->hitters = b->hitters;
    a->recievers = b->recievers;
    //putting a's stored data into b
    b->player = tplayer;
    b->potions = tpotions;
    b->pearls = tpearls;
    b->dealt = tdealt;
    b->taken = ttaken;
    b->food = tfood;
    b->ingots = tingots;
    b->kills = tkills;
    b->deaths = tdeaths;
    b->charges = tcharges;
    b->hitters = thitters;
    b->recievers = trecievers;
    return;
}

//prints out the node
void node_print(Node *n) {
    if (n->player != NULL) {
        printf("%s:\n", n->player);
	printf("    Kills: %d  Deaths: %d\n", n->kills, n->deaths);
	printf("    Potions Thrown: %d\n", n->potions);
	printf("    Pearls Thrown: %d\n", n->pearls);
	printf("    Damage Dealt: %1.1f\n", n->dealt);
	printf("    Damage Taken: %1.1f\n", n->taken);
	printf("    Food Eaten: %d\n", n->food);
	printf("    AIs Used: %d\n", n->ingots);
	printf("    Charges: %d\n", n->charges);
    } else {
        printf("Error printing node.\n");
    }
    return;
}

void node_print_dealt(Node *n) {
	if (n->player != NULL) {
		printf("%s: %1.1f\n", n->player, n->dealt);
	}
}

void node_print_taken(Node *n) {
	if (n->player != NULL) {
		printf("%s: %1.1f\n", n->player, n->taken);
	}
}
