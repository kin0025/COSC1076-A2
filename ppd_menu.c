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
   menu_function functions[NUM_MENU_ITEMS] = MENU_FUNCTIONS;
   char *names[NUM_MENU_ITEMS] = MENU_NAMES;

   /* Set the menu entries to NULL */
   for (i = 0; i < NUM_MENU_ITEMS; i++) {
      menu[i].function = NULL;
   }
   /* Set the menu entries to their values */
   for (i = 0; i < NUM_MENU_ITEMS; i++) {
      strcpy(menu[i].name, names[i]);
      menu[i].function = functions[i];
   }
}

/**
 * @return a menu_function that defines how to perform the user's
 * selection
 **/
menu_function get_menu_choice(struct menu_item *menu) {
   int i, choice;
   BOOLEAN quit = FALSE;
   /* Print the menu*/
   printf("Main Menu:\n");
   for (i = 0; i < NUM_MENU_ITEMS; i++) {
      if (menu[i].function == &save_system) {
         printf("Administrator Menu:\n");
      }
      printf("%s%d.%s %s\n", F_LIGHT_GREEN, i + 1, COLOUR_RESET, menu[i].name);

   }
   i = 0;
   /* Get the input (and validate) and return the function pointer contained in
    * the array*/
   do {
      if (i == 0) {
         i++;
      } else {
         printf("Input must be between 1 and %d\n", NUM_MENU_ITEMS);
      }
      /* Check the input validity */
      do {
         quit = read_int(&choice);
         /*if (!quit) {
            return NULL;
         }*/
         if (!quit) {
            printf("Invalid Input\n");
         }
      } while (!quit);
   } while (!(choice > 0 && choice <= NUM_MENU_ITEMS));
   return menu[choice - 1].function;
}
