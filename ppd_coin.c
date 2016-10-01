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


BOOLEAN is_valid_denom(enum denomination denom) {
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

BOOLEAN remove_coin_val(struct coin cash_register[NUM_DENOMS], int value,
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

BOOLEAN add_coin_val(struct coin cash_register[NUM_DENOMS], int value, int
amount) {
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

BOOLEAN remove_coin_denom(struct coin cash_register[NUM_DENOMS], enum
        denomination denom, int amount) {
   int i;


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

   printf("Invalid denomination\n");
   return FALSE;

}

BOOLEAN add_coin_denom(struct coin cash_register[NUM_DENOMS], enum
        denomination denom, int amount) {
   int i;

   for (i = 0; i < NUM_DENOMS; i++) {
      if (cash_register[i].denom == denom) {
         cash_register[i].count += amount;
         return TRUE;
      }
   }

   printf("Register does not take that type of money\n");
   return FALSE;

}

int count_coins(struct coin cash_register[NUM_DENOMS], enum denomination
denom) {
   int i;
   if (cash_register[denom].denom == denom) {
      return cash_register[denom].count;
   } else {
      for (i = 0; i < NUM_DENOMS; i++) {
         if (cash_register[i].denom == denom) {
            return cash_register[i].count;
         }
      }
   }
   return 0;
}

void display_coins_imp(struct ppd_system *system) {
   struct coin *coins;
   int i, padd_name;
   unsigned int value;
   char *name, dollars[] = "dollars", cents[] = "cents";

   coins = system->cash_register;

   printf("Denomination | Count\n\n");
   for (i = 0; i < NUM_DENOMS; i++) {
      value = denom_valuer(coins[i].denom);

      if (type_of_denom(&value) == DOLLARS) {
         name = dollars;
      } else {
         name = cents;
      }

      padd_name = DISPLAY_LEFT_PADDING - num_places(value);

      printf("%u %-*s | %5u\n", value, padd_name, name,
             count_coins(coins, coins[i].denom));
   }

   printf("Press %s to go back to menu", ENTER_COLOUR);
   /* Used to require an Enter */
   read_rest_of_line();
}

/** Calculates the change required for the coin amount (change_amount),
 * using the change values in the ppd_system. The values are returned in the
 * coin_change array.
 */
BOOLEAN calculate_change(struct coin change[NUM_DENOMS], int change_amount,
                         struct ppd_system *system) {

   int useable_coin_count, denom_value;
   enum denomination denom;
   struct coin *coins;

   coins = system->cash_register;
/* Sets the change given to zero */
   void_balances(change);

   /* While we still need to give people change, loop */
   while (change_amount != 0) {
      /* At the start of each iteration, set the number of potential coins to
       * zero
       */
      useable_coin_count = 0;

      /* Loop through denoms to find the first one that is valid*/
      for (denom = NUM_DENOMS - 1; denom > 0; denom--) {
         denom_value = denom_valuer(denom);

         if ((change_amount - denom_value) >= 0) {
            if (count_coins(coins, denom) > 0) {
               useable_coin_count++;
               change_amount -= denom_value;
               add_coin_denom(change, denom, 1);
               remove_coin_denom(coins, denom, 1);
               break;
            }
         }

      }
      if (useable_coin_count == 0) {
         for (denom = 0; denom < NUM_DENOMS; denom++) {
            add_coin_denom(coins, denom, count_coins(change, denom));
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
   struct coin *coins;
   coins = system->cash_register;
   if (system->coin_from_file) {
      rename_file(system->coin_file_name, FALSE);
      coin_file = fopen(system->coin_file_name, "w");
      if (coin_file != NULL) {
         for (i = 0; i < NUM_DENOMS; i++) {
            fprintf(coin_file, "%d,%u\n", denom_valuer
                    (coins[i].denom), coins[i].count);
         }
         fclose(coin_file);
         return TRUE;
      } else {
         rename_file(system->coin_file_name, TRUE);
         return FALSE;
      }
   }
   return TRUE;
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