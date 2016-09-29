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
   struct ppd_node *current;
   struct ppd_stock *item;

   printf("\n\nItems:\n");
   name_len = get_largest_name(system);

   printf("%-5s|%-*s|%-9s|%-5s\n", "ID", name_len, "Name",
          "Available", "Price");
   for (i = 0; i < name_len; i++) {
      printf("=");
   }
   printf("=======================\n");

   current = system->item_list->head;
   while (current != NULL) {
      item = current->data;
      printf("%-5s|%-*s| %-8d|$%2d.%-2.2d\n", item->id, name_len, item->name,
             item->on_hand, item->price.dollars, item->price.cents);

      current = current->next;
   }

   printf("Press %s to go back to menu", ENTER_COLOUR);
   /* Used to require an Enter */
   read_rest_of_line();
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
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
   int cents_paid, cents_due;
   BOOLEAN no_quit = TRUE, valid_denom;
   struct price amount;

   printf("Purchase Item\n-------------\nEnter the id of the item you wish to"
                  " purchase:");
   no_quit = read_user_input(id, IDLEN);
   if (!no_quit) {
      return TRUE;
   }
   item = find_id(system->item_list->head, id)->data;
   while (item == NULL) {
      printf("ID not found. Please try again:");
      no_quit = read_user_input(id, IDLEN);
      if (!no_quit) {
         return TRUE;
      }
      item = find_id(system->item_list->head, id)->data;
   }
   if (item->on_hand == 0) {
      printf("The item you have selected is out of stock.\n");
      return TRUE;
   }
   printf("You have selected %s: %s\nThis will cost you $%2d.%-2.2d\n",
          item->name,
          item->desc, item->price.dollars, item->price.cents);

   printf("Please type the value of each coin or note in cents or press %s "
                  "on a new empty line to cancel purchase.\n", ENTER_COLOUR);

   no_quit = price_to_int(&item->price, &cents_due);
   if (!no_quit) {
      printf("Invalid Price. Returning to menu\n");
      return FALSE;
   }

   while (cents_due > 0) {
      do {
         amount = coins_to_price(cents_due);
         printf("There is $%d.%2.2d left: ", amount.dollars, amount.cents);
         no_quit = read_int(&cents_paid);
         if (!no_quit) {/*TODO COIN LOGIC HERE*/
            return TRUE;
         }

         valid_denom = is_valid_denom(cents_paid);


         if (!valid_denom) {
            printf("Was not a valid denomination of money \n");
         }
         add_coin(system, cents_paid);

      } while (!valid_denom);

      cents_due -= cents_paid;

   }
   cents_due *= -1;
   amount = coins_to_price(cents_due);
   printf("Here is your %s, and $%d.%d change", item->name, amount.dollars,
          amount.cents);
   item->on_hand--;
   /*todo COIN LOGIC HERE*/
   return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system *system) {
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   return FALSE;
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
   int on_hand;

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

   printf("Please enter the amount of the item:");
   works = read_int(&on_hand);
   if (!works) {
      return FALSE;
   }
   while (on_hand < 0) {
      works = read_int(&on_hand);
      if (!works) {
         return FALSE;
      }
   }
   new_stock.on_hand = on_hand;

   printf("Please enter the cost of the item in the form dollars.cents:");

   works = read_user_input(temp_price, COSTLEN);
   if (!works) {
      return FALSE;
   }

   sprintf(new_stock.id, "I%4d", get_next_id(system));


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
   struct ppd_node *item;
   char id[IDLEN + EXTRACHARS];
   char yes_no_input[YESNOLEN + EXTRACHARS], yes_no;
   printf("Remove Item\n-------------\nEnter the id of the item you wish to"
                  " remove:");
   no_quit = read_user_input(id, IDLEN);


   if (!no_quit) {
      return TRUE;
   }
   item = find_id(system->item_list->head, id);
   while (item == NULL) {
      printf("ID not found. Please try again:");
      no_quit = read_user_input(id, IDLEN);
      if (!no_quit) {
         return TRUE;
      }
      item = find_id(system->item_list->head, id);
   }
   printf("Do you want to remove %s. You cannot undo this action. (Y/N)",
          item->data->name);
   no_quit = read_user_input(yes_no_input, YESNOLEN);
   if (!no_quit) {
      return TRUE;
   }
   if (strlen(yes_no_input) == 2) {
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
      return remove_stock(system, id);
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
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system *system) {
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */

   return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system *system) {
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   return FALSE;
}

BOOLEAN save_exit(struct ppd_system *system) {
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   save_system(system);
   return FALSE;
}