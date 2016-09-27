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
      menu[i]->function = NULL;
      menu[i]->name = NULL;
   }
   menu[0].name = "Display Items";
   menu[0].function = *display_items((*ppd_system));

   menu[1].name = "Purchase Items";
   menu[1].function = *purchase_item((*ppd_system));

   menu[2].name = "Save and Exit";
   menu[2].function = *save_system((*ppd_system));

   menu[3].name = "Add Item";
   menu[3].function = *add_item((*ppd_system));

   menu[4].name = "Remove Item";
   menu[4].function = *remove_item((*ppd_system));

   menu[5].name = "Display Coins";
   menu[5].function = *display_coins((*ppd_system));

   menu[6].name = "Reset Stock";
   menu[6].function = *reset_stock((*ppd_system));

   menu[7].name = "Reset Coins";
   menu[7].function = *reset_coins((*ppd_system));

   menu[8].name = "Abort Program";
   menu[8].function = *save_system((*ppd_system));

}

/**
 * @return a menu_function that defines how to perform the user's
 * selection
 **/
menu_function get_menu_choice(struct menu_item *menu) {
   int i,choice;
   printf("Main Menu:\n");
   for(i = 0;i < NUM_MENU_ITEMS;i++){
      if(i == 2){
         printf("Administrator Menu:\n");
      }
      printf("%d\. %s\n",i+1,menu[i].name);

   }
   do{
      choice = read_int();
   }while(0<choice<=NUM_MENU_ITEMS);

   return menu[choice-1].function;
}
