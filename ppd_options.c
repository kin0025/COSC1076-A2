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
   int desc_len;
   struct ppd_node *current;
   struct ppd_stock *item;

   printf("Items Menu\n");
   desc_len = get_largest_description(system);
   printf("%-5s|%*s|%s|%-5s\n", "ID", desc_len, "Name",
          "Available", "Price");
   current = system->item_list->head;
   while (current != NULL) {
      item = current->data;
      printf("%-5s|%*s|%-9d|$%-2d.%-2d\n", item->id, desc_len, item->name,
             item->on_hand, item->price.dollars, item->price.cents);

      current = current->next;
   }

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
   struct price amount_due;
   int cents_paid,cents_due;
   BOOLEAN found;
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   printf("Purchase Item\n-------------\nEnter the id of the item you wish to"
                  " purchase:");
   read_user_input(id, IDLEN);
   found = find_id(system->item_list->head, id , item);
   while (found == FALSE) {
      printf("ID not found. Please try again:");
      read_user_input(id, IDLEN);
      found = find_id(system->item_list->head, id,item);
   }
   printf("You have selected %s: %s\nThis will cost you $%2d.%2d", item->name,
          item->desc, item->price.dollars, item->price.cents);
   amount_due = item->price;
   printf("Please type the value of each coin or note in cents\nPress enter "
                  "on a new empty line to cancel purchase.");

   while (amount_due.cents > 0 && amount_due.dollars > 0) {
      do {
         printf("Was not a valid denomination of money\n There is $%d.%d "
                        "left", amount_due.dollars, amount_due.cents);

         cents_paid = read_int();
      } while (!is_valid_denom(cents_paid));

      cents_due = (amount_due.dollars * CENTS_IN_DOLLAR) + amount_due.cents;
      cents_due -= cents_paid;

      amount_due = coins_to_price(cents_due);
     /*todo ADD COIN LOGIC HERE*/
   }
   cents_due *= -1;
   amount_due = coins_to_price(cents_due);
   printf("Here is your %s, and $%d.%d change",item->name,amount_due
           .dollars,amount_due.cents);
/*todo COIN LOGIC HERE*/
   return FALSE;
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
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   printf("???");


   return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system *system) {
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
   return FALSE;
}