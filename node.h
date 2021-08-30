#ifndef __NODE_H__
#define __NODE_H__

#include "dict.h"

#include <stdint.h>

typedef struct Node Node;

struct Node {
    char *player;
    uint32_t potions;
    uint32_t pearls;
    float dealt;
    float taken;
    uint32_t food;
    uint32_t ingots;
    uint32_t kills;
    uint32_t deaths;
    uint32_t charges;
    Node *next;
    Node *prev;
    Dict *hitters;
    Dict *recievers;
};

Node *node_create(char *player);

void node_delete(Node **n);

void node_swap(Node *a, Node *b);

void node_print(Node *n);

void node_print_dealt(Node *n);

void node_print_taken(Node *n);

#endif
