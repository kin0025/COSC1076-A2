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

#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu(struct menu_item *menu) {
   int i;
   for (i = 0; i < NUM_MENU_ITEMS; i++) {
      menu[i].function = NULL;
   }
   if (NUM_MENU_ITEMS >= 9) {
      strcpy(menu[0].name, "Display Items");
      menu[0].function = &display_items;

      strcpy(menu[1].name, "Purchase Items");
      menu[1].function = &purchase_item;

      strcpy(menu[2].name, "Save and Exit");
      menu[2].function = &save_system;

      strcpy(menu[3].name, "Add Item");
      menu[3].function = &add_item;

      strcpy(menu[4].name, "Remove Item");
      menu[4].function = &remove_item;

      strcpy(menu[5].name, "Display Coins");
      menu[5].function = &display_coins;

      strcpy(menu[6].name, "Reset Stock");
      menu[6].function = &reset_stock;

      strcpy(menu[7].name, "Reset Coins");
      menu[7].function = &reset_coins;

      strcpy(menu[8].name, "Abort Program");
      menu[8].function = &save_system;
   }
}

/**
 * @return a menu_function that defines how to perform the user's
 * selection
 **/
menu_function get_menu_choice(struct menu_item *menu) {
   int i, choice;
   BOOLEAN quit = FALSE;
   printf("Main Menu:\n");
   for (i = 0; i < NUM_MENU_ITEMS; i++) {
      if (i == 2) {
         printf("Administrator Menu:\n");
      }
      printf("%s%d.%s %s\n", F_LIGHT_GREEN, i + 1, COLOUR_RESET, menu[i].name);

   }
   i = 0;
   do {
      if (i == 0) {
         i++;
      } else {
         printf("Input must be between 1 and %d\n", NUM_MENU_ITEMS);
      }
      quit = read_int(&choice);
      if (!quit) {
         return FALSE;
      }
   } while (!(choice > 0 && choice <= NUM_MENU_ITEMS));
   return menu[choice - 1].function;
}
