/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : Alexander Chi-Cheng Kinross-Smith
 * Student Number   : s3603437
 * Course Code      : COSC1076
 * Program Code     : BH094
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_coin.h"
#include "ppd_stock.h"
#include "ppd_utility.h"
#include <limits.h>

/**
 * @file ppd_coin.c implement functions here for managing coins in the
 * "cash register" contained in the @ref ppd_system struct.
 **/
BOOLEAN void_balances(struct coin cash_register[NUM_DENOMS]) {
   int i;
   for (i = 0; i < NUM_DENOMS; i++) {
      cash_register[i].denom = i;
      cash_register[i].count = 0;
   }
   return TRUE;
}

void reset_coins_imp(struct ppd_system *system) {
   int i;
   for (i = 0; i < NUM_DENOMS; i++) {
      system->cash_register[i].count = DEFAULT_COIN_COUNT;
   }
}


BOOLEAN is_valid_denom(int denom) {
   int valid_denoms[NUM_DENOMS] = VALID_DENOMS, i;
   BOOLEAN is_valid = FALSE;
   for (i = 0; i < NUM_DENOMS; i++) {
      if (valid_denoms[i] == denom) {
         is_valid = TRUE;
      }
   }
   return is_valid;
}

struct price coins_to_price(int cents) {
   struct price return_val;
   return_val.dollars = (int) cents / CENTS_IN_DOLLAR;
   return_val.cents = (int) cents - (return_val.dollars * CENTS_IN_DOLLAR);
   return return_val;
}

BOOLEAN remove_coin(struct coin cash_register[NUM_DENOMS], int value,
                    int amount) {
   int i;
   enum denomination denom;

   denom = value_to_denom(value);

   if (denom != -1) {
      for (i = 0; i < NUM_DENOMS; i++) {
         if (cash_register[i].denom == denom &&
             cash_register[i].count > 0) {
            cash_register[i].count -= amount;
            return TRUE;
         } else if (cash_register[i].denom == denom &&
                    cash_register[i].count == 0) {
            printf("No change left to give\n");
            return FALSE;
         }
      }
   }
   printf("Invalid denomination\n");
   return FALSE;

}

BOOLEAN add_coin(struct coin cash_register[NUM_DENOMS], int value, int amount) {
   int i;
   enum denomination denom;

   denom = value_to_denom(value);

   if (denom != -1) {
      for (i = 0; i < NUM_DENOMS; i++) {
         if (cash_register[i].denom == denom) {
            cash_register[i].count += amount;
            return TRUE;
         }
      }
   }
   printf("Register does not take that type of money\n");
   return FALSE;

}

void display_coins_imp(struct ppd_system *system) {
   struct coin *coins;
   int i, padd_name;
   unsigned int value;
   char *name, dollars[] = "dollars", cents[] = "cents";

   coins = system->cash_register;

   printf("Denomination | Count\n");
   for (i = 0; i < NUM_DENOMS; i++) {
      value = denom_valuer(coins[i].denom);

      if (type_of_denom(&value) == DOLLARS) {
         name = dollars;
      } else {
         name = cents;
      }

      padd_name = DISPLAY_LEFT_PADDING - num_places(value) - strlen(name);

      printf("%u %*s | %5u\n", value, padd_name, name, coins[i].count);
   }
}


BOOLEAN calculate_change(struct coin change[NUM_DENOMS], int change_amount,
                         struct ppd_system *system) {
   int i, useable_coin_count, denom_values[NUM_DENOMS] = VALID_DENOMS;


   void_balances(change);

   while (change_amount > 0) {
      useable_coin_count = 0;
      for (i = 0; i < NUM_DENOMS; i++) {
         if ((change_amount - denom_values[i]) >= 0) {

            if (system->cash_register[i].count > 0) {
               change_amount -=
                       denom_values[i];
               change[i].count++;
               system->cash_register[i].count--;
               break;
            } else {
               useable_coin_count++;
            }
         }

      }
      if (useable_coin_count == 0) {
         for (i = 0; i < NUM_DENOMS; i++) {
            system->cash_register[i].count += change[i].count;
         }
         return FALSE;
      }


   }
   return TRUE;
}

/* See http://stackoverflow.com/a/1068937
 * Originally went there looking for a valid Log10, but got this instead.
 * Unmodified from original. Credit to paxdiablo*/
int num_places(int n) {
   if (n < 0) return num_places((n == INT_MIN) ? INT_MAX : -n);
   if (n < 10) return 1;
   return 1 + num_places(n / 10);
}

enum denom_type type_of_denom(unsigned int *unit_value) {
   if (*unit_value > CENTS_IN_DOLLAR) {
      *unit_value = (int) *unit_value / CENTS_IN_DOLLAR;
      return DOLLARS;
   } else {
      return CENTS;
   }
}

BOOLEAN save_coins(struct ppd_system *system) {
   FILE *coin_file = NULL;
   int i;
   if (system->coin_from_file) {
      rename_file(system->coin_file_name, FALSE);
      coin_file = fopen(system->coin_file_name, "w");
      if (coin_file != NULL) {
         for (i = 0; i < NUM_DENOMS; i++) {
            fprintf(coin_file, "%d,%u\n", denom_valuer
                            (system->cash_register[i].denom),
                    system->cash_register[i].count);
         }
         fclose(coin_file);
      } else {
         rename_file(system->coin_file_name, TRUE);
      }
   }

   return FALSE;
}

int denom_valuer(enum denomination denom) {
   int denom_values[NUM_DENOMS] = VALID_DENOMS;
   if (denom >= FIVE_CENTS && denom <= TEN_DOLLARS) {
      return denom_values[denom];
   } else {
      fprintf(stderr, "Invalid Denom\n");
      return -1;
   }
}

enum denomination value_to_denom(int value) {
   int i, denom_values[NUM_DENOMS] = VALID_DENOMS;
   for (i = 0; i < NUM_DENOMS; i++) {
      if (value == denom_values[i]) {
         return i;
      }
   }
   return -1;
}