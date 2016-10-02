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

#include "ppd_options.h"
#include "ppd_utility.h"
#include "ppd_shared.h"
#include "ppd_coin.h"
/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system *system) {
   int name_len, i;
   struct ppd_node *current = NULL;
   struct ppd_stock *item;
   BOOLEAN is_node;

   printf("\n\nItems:\n");

   /* Get the length of the longest name for formatting. All other input is
    * always shorter then the description on the top, so does not need to be
    * dynamically scaled */
   name_len = get_largest_name(system);

   /* Print the title bar */
   printf("%-5s|%-*s|%-9s|%-5s\n", "ID", name_len, "Name",
          "Available", "Price");

   /* Scale the divider based on the length of the table */
   for (i = 0; i < name_len; i++) {
      printf("=");
   }
   printf("=======================\n");

   /* Init our node, and check that it worked */
   is_node = init_node(&current, system);
   if (is_node) {
      /* If the node initialised, there is at least one item. Print the first
       * item, and as long as there are more items keep going */
      do {
         item = current->data;

         printf("%-5s|%-*s| %-8u|$%2u.%-2.2u\n", item->id, name_len, item->name,
                item->on_hand, item->price.dollars, item->price.cents);

      } while (next_node(&current));
   } else {
      /* If initialisation failed, there were no items in the list :( Tell
       * the user */
      printf("No items in inventory \n");
   }

   printf("Press %s to go back to menu", ENTER_COLOUR);
   /* Used to require an enter / EOL*/
   read_rest_of_line();

   return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system *system) {
   char id[IDLEN + 1];
   struct ppd_stock *item = NULL;
   int cents_paid, cents_due, i, coins_left;
   unsigned int coin_value;
   BOOLEAN no_quit, valid_denom;
   struct price amount;
   struct ppd_node *node = NULL;
   struct coin coins_taken[NUM_DENOMS], coins_change[NUM_DENOMS];


   void_balances(coins_taken);
   void_balances(coins_change);

   /* If there are no items, quit, and tell the user why. */
   no_quit = init_node(&node, system);
   if (!no_quit) {
      printf("There are no items in inventory to purchase. Sorry!\n");
      return FALSE;
   }

   /* Read the ID that the user wants to purchase */
   printf("Purchase Item\n-------------\nEnter the id of the item you wish to"
                  " purchase:");
   no_quit = read_user_input(id, IDLEN);
   if (!no_quit) {
      printf("Returning to menu\n");
      /* As we are not quitting due to an error, instead due to user choice,
       * return true */
      return TRUE;
   }
   item = find_id(node, id)->data;
   while (item == NULL) {
      printf("ID not found. Please try again:");
      no_quit = read_user_input(id, IDLEN);
      if (!no_quit) {
         printf("Returning to menu\n");
         /* As we are not quitting due to an error, instead due to user choice,
      * return true */
         return TRUE;
      }
      item = find_id(node, id)->data;
   }
   if (item->on_hand == 0) {
      printf("The item you have selected is out of stock.\n");
      return TRUE;
   }

   no_quit = price_to_int(&item->price, &cents_due);
   if (!no_quit) {
      printf("Invalid Price. Returning to menu\n");
      return FALSE;
   }

   printf("You have selected %s: %s\nThis will cost you $%2u.%-2.2u\n",
          item->name,
          item->desc, item->price.dollars, item->price.cents);

   printf("Please type the value of each coin or note in cents or press %s "
                  "on a new empty line to cancel purchase.\n", ENTER_COLOUR);


   while (cents_due > 0) {
      do {
         amount = coins_to_price(cents_due);
         printf("There is $%u.%2.2u left: ", amount.dollars, amount.cents);
         no_quit = read_int(&cents_paid);
         if (!no_quit) {
            return TRUE;
         }

         valid_denom = is_valid_value(cents_paid);


         if (!valid_denom) {
            printf("Was not a valid denomination of money \n");
         } else {
            add_coin_val(system->cash_register, cents_paid, 1);
            add_coin_val(coins_taken, cents_paid, 1);
         }
      } while (!valid_denom);

      cents_due -= cents_paid;

   }
   cents_due *= -1;
   amount = coins_to_price(cents_due);

   if (calculate_change(coins_change, cents_due, system)) {
      printf("Here is your %s%s%s, and $%u.%2.2u change:", F_CYAN, item->name,
             COLOUR_RESET, amount.dollars, amount.cents);
      item->on_hand--;
      for (i = 0; i < NUM_DENOMS; i++) {
         if (coins_change[i].count > 0) {
            remove_coin_denom(system->cash_register,
                              coins_change[i].denom,
                              coins_change->count);
            for (coins_left = 0; coins_left < coins_change[i].count;
                 coins_left++) {
               coin_value = denom_valuer(coins_change[i].denom);
               if (type_of_denom(&coin_value) == DOLLARS) {
                  printf("$%u ", coin_value);
               } else {
                  printf("%uc ", coin_value);
               }
            }
         }
      }
   } else {
      for (i = 0; i < NUM_DENOMS; i++) {
         remove_coin_denom(system->cash_register,
                           coins_change[i].denom,
                           coins_taken[i].count);
      }
      printf("Change could not be given. All money has been refunded. "
                     "Please try again with exact change, or different coins"
                     ".\n");
      return FALSE;
   }
   printf("\nPlease come back soon\n");
   return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system *system) {
   BOOLEAN no_error_stock, no_error_coin;
   no_error_stock = save_stock(system);

   if (system->coin_from_file) {
      no_error_coin = save_coins(system);

   }
   if (!no_error_stock) {
      printf("Error when saving stock\n");
   }
   if (!no_error_coin) {
      printf("Error when saving coins\n");
   }
   if (!no_error_coin || !no_error_stock) {
      return FALSE;
   }

   return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system *system) {
   struct ppd_stock new_stock;
   BOOLEAN works;
   char temp_price[COSTLEN + EXTRACHARS];
   char *ptr = NULL;
   int id;
/*
   int on_hand;
*/

   printf("Please enter the name of the item:");
   works = read_user_input(new_stock.name, NAMELEN);
   if (!works) {
      return FALSE;
   }

   printf("Please enter the description of the item:");
   works = read_user_input(new_stock.desc, DESCLEN);
   if (!works) {
      return FALSE;
   }

/*   printf("Please enter the amount of the item:");
   works = read_int(&on_hand);
   if (!works) {
      return FALSE;
   }
   while (on_hand < 0) {
      works = read_int(&on_hand);
      if (!works) {
         return FALSE;
      }
   }*/
   new_stock.on_hand = DEFAULT_STOCK_LEVEL;

   printf("Please enter the cost of the item in the form dollars.cents:");

   do {
      if (!works) {
         printf("Wrong format. Cents must be less than 100 and divisible by "
                        "%d Please try again. Dollars must be less than 100: ",
                MINUMUM_DENOM);
      }
      works = read_user_input(temp_price, COSTLEN);
      if (!works) {
         return FALSE;
      }
      works = string_to_price(&new_stock.price, temp_price);
   } while (!works);

   id = get_next_id(system);
   if (id <= 9999) {
      sprintf(new_stock.id, "I%4d", id);
   } else {
      printf("%s WARNING. THE DATABASE IS NOW FULL %s\n YOU WILL NOT BE ABLE "
                     "TO ADD NEW ITEMS UNTIL THE SYSTEM'S STOCK IS RESET OR "
                     "MORE IDS ARE ALLOCATED\n", B_YELLOW, COLOUR_RESET);
   }

   for (ptr = &new_stock.id[1]; *ptr == ' '; ptr++) {
      *ptr = '0';
   }
   return add_stock(new_stock, system);
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system *system) {
   BOOLEAN no_quit;
   struct ppd_node *item = NULL;
   char id[IDLEN + EXTRACHARS];
   char yes_no_input[YESNOLEN + EXTRACHARS], yes_no;
   struct ppd_node *node = NULL;

   no_quit = init_node(&node, system);
   if (!no_quit) {
      return FALSE;
   }

   printf("Remove Item\n-------------\nEnter the id of the item you wish to"
                  " remove:");
   no_quit = read_user_input(id, IDLEN);


   if (!no_quit) {
      return TRUE;
   }
   item = find_id(node, id);
   while (item == NULL) {
      printf("ID not found. Please try again:");
      no_quit = read_user_input(id, IDLEN);
      if (!no_quit) {
         return TRUE;
      }
      item = find_id(node, id);
   }
   printf("Do you want to remove %s. You cannot undo this action. (Y/N)",
          item->data->name);
   no_quit = read_user_input(yes_no_input, YESNOLEN);
   if (!no_quit) {
      return TRUE;
   }
   if (strlen(yes_no_input) != 2) {
      yes_no = tolower(yes_no_input[0]);
   }
   while (yes_no != 'y' && yes_no != 'n') {
      no_quit = read_user_input(yes_no_input, YESNOLEN);
      if (!no_quit) {
         return TRUE;
      }
      if (strlen(yes_no_input) == 2) {
         yes_no = tolower(yes_no_input[0]);
      }
   }

   if (yes_no == 'y') {
      if (remove_stock(system, id)) {
         printf("Removed Successfully\n");
         return TRUE;
      } else { return FALSE; }
   } else {
      return FALSE;
   }
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system *system) {
   struct ppd_node *node = NULL;
   BOOLEAN no_error;
   no_error = init_node(&node, system);
   if (!no_error) {
      return FALSE;
   }
   while (next_node(&node)) {
      node->data->on_hand = DEFAULT_STOCK_LEVEL;
   }
   printf("Stock counts reset to %d\n", DEFAULT_STOCK_LEVEL);
   return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system *system) {

   reset_coins_imp(system);
   printf("Coin counts reset to %d.\n", DEFAULT_COIN_COUNT);
   return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system *system) {
   display_coins_imp(system);

   return TRUE;
}
