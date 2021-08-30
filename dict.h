#ifndef __DICT_H__
#define __DICT_H__

#include "player.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Dict Dict;

Dict *dict_create(uint32_t size);

void dict_delete(Dict **d);

uint32_t dict_length(Dict *d);

Player *dict_lookup(Dict *d, char *name);

bool dict_update(Dict *d, char *name, float damage, uint32_t format);

void dict_sort(Dict *d, uint32_t format);

void dict_print(Dict *d, uint32_t format);

#endif
