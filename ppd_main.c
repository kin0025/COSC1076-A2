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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"
#include "ppd_stock.h"
#include "ppd_coin.h"
#include "ppd_shared.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv) {
   BOOLEAN keep_running = TRUE, success = TRUE, no_error;
   char *data_name = NULL;
   FILE *data_file = NULL;
   menu_function menu_choice;
   struct menu_item menu[NUM_MENU_ITEMS];
   struct ppd_system system;
   system.coin_from_file = FALSE;
   system.coin_file_name = NULL;
   system.stock_file_name = NULL;

   /* validate command line arguments */
   switch (argc) {
      case 3:
         system.coin_file_name = argv[2];
         system.coin_from_file = TRUE;
      case 2:
         data_name = argv[1];
         data_file = fopen(data_name, "r");

         if (data_file == NULL) {
            fprintf(stderr, "Unable to open file\n");
            printf("Data file failed to load\n");
            return EXIT_FAILURE;
         }

         system.stock_file_name = data_name;
         fclose(data_file);

         break;
      default:
         printf("You need to provide one or two arguments to the "
                        "location of the files to load.\n");
         return EXIT_FAILURE;

   }



   /* init the system */
   success = system_init(&system);
   if (!success) {
      printf("Initialisation Failed\n");
      return EXIT_FAILURE;
   }
   /* load data */
   success = load_stock(&system, data_name);

   if (!success) {
      printf("Stock loading failed\n");
      return EXIT_FAILURE;
   }

   /*load_coins/init them*/
   success = load_coins(&system, system.coin_file_name);

   if (!success) {
      printf("Coin loading failed\n");
      return EXIT_FAILURE;
   }
   /* test if everything has been initialised correctly */

   if (system.item_list->count == 0) {
      printf("Nothing was loaded from file. Check your input formatting and "
                     "file name.\n");
   }

   /* initialise the menu system */
   init_menu(menu);
   /* loop, asking for options from the menu */
   do {
      menu_choice = get_menu_choice(menu);
      /* run each option selected */

      /* If they want to exit quit the loop*/
      if (menu_choice == NULL) {
         keep_running = FALSE;
         break;
      }

      no_error = menu_choice(&system);
      if (!no_error) {
         printf("The previous choice did not complete successfully. "
                        "Try again.\n");
      }
      if (menu_choice == &save_system && no_error == TRUE) {
         keep_running = FALSE;
      }
      printf("\n\n=======================\n");
      /* until the user quits */
      if (!keep_running) {

         save_system(&system);
      }
   } while (keep_running);
   /* make sure you always free all memory and close all files
    * before you exit the program
    */
   system_free(&system);

   return EXIT_SUCCESS;
}
