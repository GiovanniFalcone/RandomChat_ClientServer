/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#include "Menu.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

char *menu()
{
    char *choice = (char *) malloc(sizeof(choice));

    int done = 0;

    do
    {   
        printf("\n**********************************************************\n");
        printf("\t\t\tCOMMANDS\n");
        printf("**********************************************************\n");
        printf(" - @stop to suspend chat \n - @exit to exit from the chat \n - @user to know how many users are connected in each room\n");
        printf("**********************************************************\n\n");
        
        printf("\n**********************************************************\n");
        printf("\t \t \t  MENU\n");
        printf("**********************************************************\n");
        printf("1. Topical room\n");
        printf("2. Sport room\n");
        printf("3. Music room\n");
        printf("4. University room\n");
        printf("5. Exit\n");

        printf("Enter a choice: ");
        scanf(" %s", choice);
        while((getchar()) != '\n');

        switch(choice[0]){
            case '1':
                printf("You choose topical room!\n");
                done = 1;
                break;

            case '2':
                printf("You choose sport room!\n");
                done = 1;
                break;

            case '3':
                printf("You choose music room\n");
                done = 1;
                break;

            case '4':
                printf("You choose university room\n");
                done = 1;
                break;
                
            case '5':
            	printf("Goodbye\n");
            	break;

            default:
                if (strcmp(choice, "@user") == 0) return choice;
                else{
    		        printf("\e[1;1H\e[2J");
    		        printf("Choice not valid...\n");
                }	
            }
    } while(choice[0] != '5' && done != 1);

    printf("\n");
	
    return(choice);
}







