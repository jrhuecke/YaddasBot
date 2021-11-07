#include "ll.h"
#include "node.h"
#include "dict.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "hmtdisp:"
#define BLOCK 4096

int main(int argc, char **argv) {
   int opt = 0;
   bool move_to_front = true;
   bool damage_taken = false;
   bool damage_dealt = false;
   bool individual = false;
   bool print_stats = false;
   char *player_name;
   bool print_player_stats = false;
   while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            printf("SYNOPSIS\n  YaddasBot prints stats for fights.\n");
            printf("  Given a fight's file as input through stdin, prints all the stats on it.\n\n");
            printf("USAGE\n  ./yaddas < input_file.txt [-hmtdis] [-p player name]\n\n");
            printf("OPTIONS\n");
            printf("  -h          Program usage and help.\n");
            printf("  -m          Disables move-to-front rule.\n");
            printf("  -t          Prints damage taken logs.\n");
            printf("  -d          Prints damage logs.\n");
            printf("  -i          Prints individual player stats.\n");
            printf("  -s          Prints overall items used stats.\n");
            printf("  -p          Prints lists of players who did/recieved damage from given player.\n");
            return 0;
        case 'm': move_to_front = false; break;
        case 't': damage_taken = true; break;
	case 'd': damage_dealt = true; break;
	case 'i': individual = true; break;
	case 's': print_stats = true; break;
	case 'p': print_player_stats = true; player_name = optarg; break;
        default:
            printf(
                "Invalid command-line option. Use -h for info on accepted command-line options.\n");
            return 0;
        }
    }

   LinkedList *ll = ll_create(move_to_front);

   char time[512];
   char name[512];
   float hp;
   char keyword[512];
   char name2[512];
   float hp2;
   float damage;
   char line[BLOCK];
   uint32_t total_food = 0;
   uint32_t total_AI = 0;
   uint32_t total_pots = 0;
   uint32_t total_pearls = 0;
   uint8_t battle_over = 0; //gets incremented when battle starts/finishes messages are read in, checks if == 2 to see if battle is over
   //read in line at a time
   while (fgets(line, BLOCK, stdin) != NULL) {
       printf("hey\n");
       if (sscanf(line, "%s %s (%f hp) %s %s", time, name, &hp, keyword, name2) >= 2) {
	   //checks for battle started/finished lines
           if (strcmp(name, "===") == 0) {
               battle_over += 1;
	       if (battle_over >= 2) {
		   break;
	       } else {
		   time[0] = '\0';
		   name[0] = '\0';
		   keyword[0] = '\0';
		   name2[0] = '\0';
		   line[0] = '\0';
		   continue;
	       }
	   }
	   //making sure only players are added to ht (some lines have stuff like town names)
	   if ((strcmp(keyword, "overloaded") != 0) && (strcmp(keyword, "died") != 0) && (strcmp(keyword, "killed") != 0) && (strcmp(keyword, "fired") != 0) && (strcmp(keyword, "warped") != 0) && (strcmp(keyword, "hit") != 0) && (strcmp(keyword, "shot") != 0) && (strcmp(keyword, "took") != 0) && (strcmp(keyword, "consumed") != 0) && (strcmp(keyword, "repaired") != 0)) {
		   time[0] = '\0';
                   name[0] = '\0';
                   keyword[0] = '\0';
                   name2[0] = '\0';
                   line[0] = '\0';
		   continue;
	   }
	   //inserts player into ht if they arent in it already
           ll_insert(ll, name);
	   //throwing pearl or potion
	   if (strcmp(keyword, "fired") == 0) {
                   if (sscanf(line, "%s %s (%f hp) fired a %s)", time, name, &hp, name2) == 4) {
		       //throwing potion
		       if (strcmp(name2, "Splash") == 0) {
		           (ll_lookup(ll, name))->potions += 1;
			   total_pots += 1;
		       //throwing pearl
		       } else if (strcmp(name2, "Thrown") == 0) {
			   (ll_lookup(ll, name))->pearls += 1;
			   total_pearls += 1;
		       }
		   }
	   //dealing damage to someone (sword or bow)
	   } else if ((strcmp(keyword, "hit") == 0) || (strcmp(keyword, "shot") == 0)) {
                   if (sscanf(line, "%s %s (%f hp) %s %s (%f hp) for %f)", time, name, &hp, keyword, name2, &hp2, &damage) == 7) {
		       (ll_lookup(ll, name))->dealt += damage;
		       if (print_player_stats) {
		           dict_update((ll_lookup(ll, name))->recievers, name2, damage, 0);
		           dict_update((ll_lookup(ll, name2))->hitters, name, damage, 1);
		       }
		   }
           //taking damage
           } else if (strcmp(keyword, "took") == 0) {
                   if (sscanf(line, "%s %s (%f hp) %s %f)", time, name, &hp, keyword, &damage) == 5) {
		       (ll_lookup(ll, name))->taken += damage;
		   }
           //eating food
	   } else if (strcmp(keyword, "consumed") == 0) {
		   (ll_lookup(ll, name))->food += 1;
		   total_food += 1;
           //using ancient ingots
	   } else if (strcmp(keyword, "repaired") == 0) {
		   (ll_lookup(ll, name))->ingots += 1;
		   total_AI += 1;
	   //killed someone
	   } else if (strcmp(keyword, "killed") == 0) {
		   //making sure they killed a player (not just a mob)
		   if (ll_lookup(ll, name2)) {
			   (ll_lookup(ll, name))->kills += 1;
			   (ll_lookup(ll, name2))->deaths += 1;
		   }
	   } else if (strcmp(keyword, "overloaded") == 0) {
		   (ll_lookup(ll, name))->charges += 1;
	   }
	   time[0] = '\0';
           name[0] = '\0';
           keyword[0] = '\0';
           name2[0] = '\0';
           line[0] = '\0';

       }
   }
   //prints individual stats
   if (individual) {	   
       ll_sort(ll, 0);
       ll_print(ll, 0);
   }
   //prints damage taken logs
   if (damage_taken) {
	   ll_sort(ll, 1);
	   ll_print(ll, 1);
   }
   //prints damage dealt logs
   if (damage_dealt) {
	   ll_sort(ll, 2);
	   ll_print(ll, 2);
   }
   //print items used stats
   if (print_stats) {
          printf("Pearls: %d AIs: %d Potions: %d Food: %d\n", total_pearls, total_AI, total_pots, total_food);
   }
   if (print_player_stats) {
	   printf("People who damaged %s:\n", player_name);
	   dict_print((ll_lookup(ll, player_name)->hitters), 1);
	   printf("\nPeople who %s damaged:\n", player_name);
	   dict_print((ll_lookup(ll, player_name)->recievers), 0);
   }
   ll_delete(&ll);
   return 0;
}

