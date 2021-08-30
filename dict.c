#include "player.h"


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dict {
    uint32_t size;
    uint32_t length;
    Player **players;
} Dict;

//creates a linked list, filling it with two sentinal nodes to act as head/tail
Dict *dict_create(uint32_t size) {
    Dict *d = (Dict *) malloc(sizeof(Dict));
    if (d) {
        d->size = size;
	d->length = 0;
	d->players = (Player **) calloc(size, sizeof(Player));
        return d;
    } else {
        return NULL;
    }
}

uint32_t dict_length(Dict *d) {
	if (d) {
	   return d->length;
	}
	return 0;
}

void dict_delete(Dict **d) {
    if (*d) {
        //deleting all the nodes
	for (uint32_t i = 0; i < (*d)->length; i++) {
		player_delete(&((*d)->players[i]));
	}
        free(*d);
        *d = NULL;
    }
    return;
}

Player *dict_lookup(Dict *d, char *name) {
	if (d) {
		for (uint32_t i = 0; i < d->length; i++) {
			if (strcmp((d->players[i])->name, name) == 0) {
				return d->players[i];
			}
		}
		return NULL;
	}
	return NULL;
}

bool dict_update(Dict *d, char *name, float damage, uint32_t format) {
	if (d->length == d->size) {
		return false;
	}
	if (dict_lookup(d, name) == NULL) {
		d->players[d->length] = player_create(name);
		d->length += 1;
	} 
	if (format == 0) {
		dict_lookup(d, name)->dealt += damage;
	} else if (format == 1) {
		dict_lookup(d, name)->taken += damage;
	}
	return true;
}

void dict_sort(Dict *d, uint32_t format) {
	uint32_t length = d->length;
	bool swapped = true;
	while (swapped) {
		swapped = false;
		for (uint32_t i = 1; i < length; i++) {
			if (format == 0) {
				if ((d->players[i])->dealt < (d->players[i - 1])->dealt) {
					Player *temp = d->players[i];
					d->players[i] = d->players[i - 1];
					d->players[i - 1] = temp;
					swapped = true;
				}
			}
			if (format == 1) {
				if ((d->players[i])->taken < (d->players[i - 1])->taken) {
					Player *temp = d->players[i];
					d->players[i] = d->players[i - 1];
					d->players[i - 1] = temp;
					swapped = true;
				}
			}
		}
		length -= 1;
	}
}

void dict_print(Dict *d, uint32_t format) {
	if (d) {
		dict_sort(d, format);
		uint32_t size = d->length;
		for (uint32_t i = 0; i < d->length; i++) {
			if (format == 0) {
	                    if (size > 9) {
	                        printf("  %d. ", size);
	                    } else {
		                printf("   %d. ", size);
	                    }
	                    if ((d->players[i])->name != NULL) {
		                printf("%s: %1.1f\n", (d->players[i])->name, (d->players[i])->dealt);
	                    }
			    size -= 1;
			} else if (format == 1) {
	                    if (size > 9) {
	                        printf("  %d. ", size);
	                    } else {
		                printf("   %d. ", size);
	                    }
	                    if ((d->players[i])->name != NULL) {
		                printf("%s: %1.1f\n", (d->players[i])->name, (d->players[i])->taken);
	                    }
			    size -= 1;
			}
		}
		
	}
	return;
}
