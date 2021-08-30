#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
} LinkedList;

//creates a linked list, filling it with two sentinal nodes to act as head/tail
LinkedList *ll_create(bool mtf) {
    LinkedList *l = (LinkedList *) malloc(sizeof(LinkedList));
    if (l) {
        l->length = 0;
        l->head = node_create(NULL);
        l->tail = node_create(NULL);
        (l->head)->next = l->tail;
        (l->tail)->prev = l->head;
        l->mtf = mtf;
        return l;
    } else {
        return NULL;
    }
}

void ll_delete(LinkedList **ll) {
    if (*ll) {
        //deleting all the nodes
        Node *current = (*ll)->head;
        //keeps deleting nodes until it reaches tail node
        while (current->next != NULL) {
            current = current->next;
            node_delete(&(current->prev));
        }
        //deletes tail node and rest of struct
        node_delete(&current);
        free(*ll);
        *ll = NULL;
    }
    return;
}

//returns length of ll
uint32_t ll_length(LinkedList *ll) {
    return ll->length;
}

//checks if a word exists in a linked list, returns the node that it resides in
//if it does, otherwise returns NULL
Node *ll_lookup(LinkedList *ll, char *player) {
    Node *current = (ll->head)->next;
    //goes through each node in the LL checking if the word
    //exists in it, stopping once it reaches the tail
    while (current != ll->tail) {
        if (strcmp(player, current->player) == 0) {
            //moves node to head if mtf was enabled
            if (ll->mtf) {
                (current->prev)->next = current->next;
                (current->next)->prev = current->prev;
                current->prev = ll->head;
                current->next = (ll->head)->next;
                ((ll->head)->next)->prev = current;
                (ll->head)->next = current;
            }
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void ll_insert(LinkedList *ll, char *player) {
    //checking to see if already in list
    if (ll_lookup(ll, player)) {
        return;
    }
    //creates new node and puts it at front of list
    Node *new = node_create(player);
    new->next = (ll->head)->next;
    new->prev = ll->head;
    (ll->head)->next = new;
    (new->next)->prev = new;
    ll->length += 1;
    return;
}

//bubble sort to sort given linked list by damage dealt, taken, or kills
void ll_sort(LinkedList *ll, uint32_t sort) {
	bool swapped = true;
	Node *current;
	Node *last = ll->tail;
	while (swapped) {
		swapped = false;
		//sets current node to be sorted as first in ll
		current = (ll->head)->next;
		//goes through linked list until it reaches last
		while (current->next != last) {
			//sorts either based on damage dealt, taken, or kills
			if (sort == 0) {
			    if (current->kills >= (current->next)->kills) {
				//sorts players with equal kills but less deaths
				if (current->kills == (current->next)->kills) {
					if (current->deaths >= (current->next)->deaths) {
					        if (current->deaths == (current->next)->deaths) {
							if (current->charges <= (current->next)->charges) {
			                                    current = current->next;
						            continue;
							}
						} else {
						    current = current->next;
						    continue;
						}
					}
				}
				node_swap(current, current->next);
				swapped = true;
			    }
			} else if (sort == 1) {
			    if (current->taken > (current->next)->taken) {
				node_swap(current, current->next);
				swapped = true;
			    }
			} else if (sort == 2) {
			    if (current->dealt > (current->next)->dealt) {
				node_swap(current, current->next);
				swapped = true;
			    }
			}
			current = current->next;
		}
		last = current;
	}
}


//prints out ll
void ll_print(LinkedList *ll, uint32_t format) {
    Node *current = (ll->head)->next;
    uint32_t size = ll_length(ll);
    //prints out individual stats
    if (format == 0) {
        while (current->next != NULL) {
	    node_print(current);
            current = current->next;
        }
    //prints out damage taken logs
    } else if (format == 1) {
        while (current->next != NULL) {
	    if (size > 9) {
	        printf("%d. ", size);
	    } else {
		printf(" %d. ", size);
	    }
            node_print_taken(current);
            current = current->next;
	    size -= 1;
	}
    //prints out damage logs
    } else if (format == 2) {
        while (current->next != NULL) {
	    if (size > 9) {
	        printf("%d. ", size);
	    } else {
		printf(" %d. ", size);
	    }
            node_print_dealt(current);
            current = current->next;
	    size -= 1;
	}
    }
    return;
}


