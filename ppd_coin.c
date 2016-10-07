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

/** Voids all the balances and sets the denoms to descending values */
BOOLEAN void_balances(struct coin cash_register[NUM_DENOMS]) {
   enum denomination i;
   /* Loops through the array from largest denom to smallest denom */
   for (i = 0; i < NUM_DENOMS; i++) {
      cash_register[i].denom = i;
      cash_register[i].count = 0;
   }
   return TRUE;
}

/** Resets all coins to the DEFAULT coin count.*/
void reset_coins_imp(struct ppd_system *system) {
   int i;
   /* Loops through the cash register and sets everything to default coin
    * count */
   for (i = 0; i < NUM_DENOMS; i++) {
      system->cash_register[i].count = DEFAULT_COIN_COUNT;
   }
}

/**
 * Add amount of coins from the cash register passed in of the type
 * denom.
 */
BOOLEAN add_coin_denom(struct coin cash_register[NUM_DENOMS], enum
        denomination denom, int amount) {
   int i;

/* Loop through the cash register */
   for (i = 0; i < NUM_DENOMS; i++) {
      /* Check that we aren't adding too many coins, and that the denom is
       * correct */
      if (cash_register[i].denom == denom && (cash_register[i].count + amount)
                                             <= MAX_COIN_COUNT) {
         cash_register[i].count += amount;
         return TRUE;
      } else if (cash_register[i].denom == denom) {
         printf("The type of coins you just put in is full, and can not be "
                        "added. Please try a different denomination\n");
         return FALSE;
      }
   }

   printf("Register does not take that type of money\n");
   return FALSE;
}

/** A wrapper function for add_coin_denom() that takes values instead **/
BOOLEAN add_coin_val(struct coin cash_register[NUM_DENOMS], int value, int
amount) {
   return add_coin_denom(cash_register, value_to_denom(value), amount);
}

/**
 * Removes amount of coins from the cash register passed in of the type
 * denom.
 */
BOOLEAN remove_coin_denom(struct coin cash_register[NUM_DENOMS], enum
        denomination denom, int amount) {
   int i;

/* Could have validated the denom before the loop, but the extra overhead
 * when denom should be valid isn't worth it when validation is done anyway.
 * Loop through denoms*/
   for (i = 0; i < NUM_DENOMS; i++) {
      /* If the current one is the correct denom and there are more than the
       * amount of coins we want to remove, subtract them and return true */
      if (cash_register[i].denom == denom &&
          cash_register[i].count >= amount) {
         cash_register[i].count -= amount;
         return TRUE;
         /* If the denom was correct, but count was less than the amount
          * required return false */
      } else if (cash_register[i].denom == denom) {
         return FALSE;
      }
   }

   printf("Invalid denomination\n");
   return FALSE;

}

/** A wrapper function for remove_coin_denom() that takes values instead **/
BOOLEAN remove_coin_val(struct coin cash_register[NUM_DENOMS], int value,
                        int amount) {
   return remove_coin_denom(cash_register, value_to_denom(value), amount);

}

/** Checks if the denomination passed in is a valid one that we have a value
 * for.*/
BOOLEAN is_valid_denom(enum denomination denom) {
   /* Check if the value isn't equal to the error value (-1) for denom valuer */
   if (denom_valuer(denom) != -1) {
      return TRUE;
   }
   return FALSE;
}

/** Checks if the value passed in is a valid denomination*/
BOOLEAN is_valid_value(int value) {
   if (value_to_denom(value) != -1) {
      return TRUE;
   }
   return FALSE;
}

/* Returns the number of coins of type denom in the cash register */
int count_coins(struct coin cash_register[NUM_DENOMS], enum denomination
denom) {
   int i;
   /* If the register is ordered correctly, return the amount straight away.
    * If it is not, brute force search the array. */
   if (cash_register[denom].denom == denom) {
      return cash_register[denom].count;
   } else {
      for (i = 0; i < NUM_DENOMS; i++) {
         if (cash_register[i].denom == denom) {
            return cash_register[i].count;
         }
      }
   }
   /* If we don't hold that type of currency, return zero as we have none */
   return 0;
}

/** Displays the coins in the order they are in the cash register */
void display_coins_imp(struct ppd_system *system) {
   struct coin *coins = NULL;
   int i, padd_name;
   unsigned int value;
   char *name, dollars[] = "dollars", cents[] = "cents";

   coins = system->cash_register;

   /* Print the header thing */
   printf("Denomination | Count\n\n");
   /* Loop through them and print them */
   for (i = 0; i < NUM_DENOMS; i++) {
      value = denom_valuer(coins[i].denom);

      /* Check the type of denom */
      if (type_of_denom(&value) == DOLLARS) {
         name = dollars;
      } else {
         name = cents;
      }

      /* Calculate the padding for the left hand value so they all line up */
      padd_name = DISPLAY_LEFT_PADDING - num_places(value);

      printf("%u %-*s | %5u\n", value, padd_name, name,
             count_coins(coins, coins[i].denom));
   }

   printf("Press %s to go back to menu", ENTER_COLOUR);
   /* Used to require an Enter */
   read_rest_of_line();
}

/** Save the coins to a file and return whether it succeed */
BOOLEAN save_coins(struct ppd_system *system) {
   FILE *coin_file = NULL;
   int i;
   struct coin *coins = NULL;
   coins = system->cash_register;
   /* If we are loading a coin from file save to thing*/
   if (system->coin_from_file) {
      /* Rename so we have a backup if something goes wrong */
      rename_file(system->coin_file_name, FALSE);
      /* OPen the coin file and validate */
      coin_file = fopen(system->coin_file_name, "w");
      if (coin_file != NULL) {
         /* Write the strings to the file in the correct format */
         for (i = 0; i < NUM_DENOMS; i++) {
            fprintf(coin_file, "%d%s%u\n", denom_valuer(coins[i].denom),
                    COIN_DELIM, coins[i].count);
         }
         /* Close the file and shit */
         fclose(coin_file);
         return TRUE;
      } else {
         rename_file(system->coin_file_name, TRUE);
         return FALSE;
      }
   }
   return TRUE;
}

/** Calculates the change required for the coin amount (change_amount),
 * using the change values in the ppd_system. The values are returned in the
 * coin_change array.
 */
BOOLEAN calculate_change(struct coin change[NUM_DENOMS], int change_amount,
                         struct ppd_system *system) {

   int useable_coin_count, denom_value;
   enum denomination denom;
   struct coin *coins = NULL;

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
         /* If we don't go into negative change (giving the user more money
          * back than they should get) it is valid */
         if ((change_amount - denom_value) >= 0) {
            /* If removing succeeds (we have coins to give) */
            if (remove_coin_denom(coins, denom, 1)) {
               /* Increment the coins used so we keep going in the loop */
               useable_coin_count++;
               /* Make change_amount work properly */
               change_amount -= denom_value;
               /* Add the coin to the return array */
               add_coin_denom(change, denom, 1);
               break;
            }
         }

      }
      /* If the loop did not subtract any coins, return FALSE, as we don't
       * have the correct change */
      if (useable_coin_count == 0) {
         /* Replace all the coins we have taken so far */
         for (denom = 0; denom < NUM_DENOMS; denom++) {
            add_coin_denom(coins, denom, count_coins(change, denom));
         }
         return FALSE;
      }


   }
   return TRUE;
}

/** See http://stackoverflow.com/a/1068937
 * Originally went there looking for a valid Log10, but got this instead.
 * Unmodified from original. Credit to paxdiablo
 * Comments my own*/
static int num_places(int n) {
   /* Check if the input is less than zero. If it is, make it positive and
    * keep going. If it is MIN_INT, pass INT_MAX (may not be the same),
    * otherwise use -n*/
   if (n < 0) return num_places((n == INT_MIN) ? INT_MAX : -n);
   /* If it is greater then 0, but less than 10, we have 1 decimal place */
   if (n < 10) return 1;
   /* Otherwise recursively drop down decimal places, adding one every time. */
   return 1 + num_places(n / 10);
}

/** Get the type of denom from the unit value*/
enum denom_type type_of_denom(unsigned int *unit_value) {
   /* If the value is greater than the minimum dollar amount, return
    * dollars, and check that is divisible by the number of cents in dollar */
   if (*unit_value >= CENTS_IN_DOLLAR && (*unit_value % CENTS_IN_DOLLAR) == 0) {
      *unit_value = (int) *unit_value / CENTS_IN_DOLLAR;
      return DOLLARS;
   } else {
      return CENTS;
   }
}

/** Returns the cents value of the denom inputted*/
int denom_valuer(enum denomination denom) {
   /* An array (predefined) of the values of denoms, with the index
    * corresponding to the denom number*/
   int denom_values[NUM_DENOMS] = VALID_DENOMS;

   /* If the denom is valid (within bounds of array), return it's value */
   if (denom >= FIVE_CENTS && denom <= TEN_DOLLARS) {
      return denom_values[denom];
   } else {
      fprintf(stderr, "Invalid Denom\n");
      return -1;
   }
}

/** Converts a value in cents to the appropriate denomination */
enum denomination value_to_denom(int value) {
   int i, denom_values[NUM_DENOMS] = VALID_DENOMS;
   for (i = 0; i < NUM_DENOMS; i++) {
      /* If the cents value is equal to the appropriate cents value, the
       * value of the denom is the current array indexx */
      if (value == denom_values[i]) {
         return i;
      }
   }
   return -1;
}

/*
BOOLEAN next_denom(struct coin **coin_pos) {
   struct coin *coins = *coin_pos;
   if (coins->denom++ == (coins++)->denom) { return TRUE; } else return FALSE;


}*/
