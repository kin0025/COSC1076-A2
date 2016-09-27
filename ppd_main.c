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
   BOOLEAN keep_running = TRUE;
   char *data_name = NULL,*coin_name = NULL;
   FILE * data_file = NULL, *coin_file = NULL;
menu_function * menu_choice;
   struct menu_item menu[NUM_MENU_ITEMS];


   /* validate command line arguments */
   switch (argc){
      case 3:
         coin_name = argv[2];
         coin_file = fopen(coin_name,'r');
         if(coin_file == NULL){
            fprintf(stderr,"Unable to open file",data_name);
            printf("Coin file failed to load");
            return EXIT_FAILURE;
         }
         fclose(coin_file);
         printf("We apologise, but this function is not implemented yet.");
      case 2:
         data_name = argv[1];
         data_file = fopen(data_name,'r');
         if(data_file == NULL){
            fprintf(stderr,"Unable to open file",data_name);
            printf("Data file failed to load");
            return EXIT_FAILURE;
         }
         fclose(data_file);
         break;
      default:
         printf("You need to provide one or two arguments to the "
                        "location of the files to load.");

      }


   /* represents the data structures to manage the system */
   struct ppd_system system;
   /* init the system */
   system_init(system);
   system.coin_file_name = coin_name;
   system.stock_file_name = data_name;
   /* load data */
   load_stock(system,data_name);


   /*load_coins(system,); */
   system.coin_from_file = FALSE;
   load_coins(system,coin_name);

   /* test if everything has been initialised correctly */

   /* initialise the menu system */
   init_menu(menu);
   /* loop, asking for options from the menu */
   while (keep_running) {
       menu_choice = get_menu_choice(menu);
keep_running = **menu_choice(system);
      /* run each option selected */

      /* until the user quits */
   }
   /* make sure you always free all memory and close all files
    * before you exit the program
    */
   save_system(system);
   system_free(system);

   return EXIT_SUCCESS;
}
