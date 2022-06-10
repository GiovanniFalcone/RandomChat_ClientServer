/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#include "Menu.h"

#define MESSAGE_SIZE 256                    /* size of message that clients sends with each other */
#define ARRAY_CONFIG_SIZE 5                 /* for config file */
#define MAX_LENGTH 20                       /* string size read it from config file*/
#define BUFFSIZE 256                        /* size of message sended from server */
#define NICKNAME_SIZE 20                    /* max size of user's nickname */
#define MESSAGE_CONNECTION 22
#define STDIN 0

/**
 * @brief This function check if the user has write "@user" command. 
 * It is true then server return a message with number of client in each room
 * 
 * @param choice to check if choice is "@user"
 * @param socket_server socket descriptor of server
 * @return char* the room to send to server 
 */
char *user_Count(char *choice, int socket_server);

/**
 * @brief This function is called when the username is not unique. 
 * So, the user have to retry.
 * 
 * @param socket_server the server's socket descriptor
 * @param room the room choosen by user
 */
void user_Check(int socket_server, char room);

/**
 * @brief Get the Config object (IP and Server Port), then save them.
 * 
 * @param server_address 
 */
void getConfig(struct sockaddr_in *server_address);



// GLOBAL VARIABLES
struct timeval tv;

int main(int argc, char **argv)
{
    char nickname[NICKNAME_SIZE];       /* the user's nickname */                
    char buff[MESSAGE_CONNECTION];      /* the message of the form: room + space + nickname */
    char buffer[BUFFSIZE];              /* the message from the server */
    struct sockaddr_in serverAddress;
    int socket_fd, conn_fd;
    ssize_t n_bytes;
    fd_set readfds;

    tv.tv_sec = 9500;
    tv.tv_usec = 500000;

    FD_ZERO(&readfds);

    char *choice = menu();

    if(choice[0] == '5')
        exit(EXIT_SUCCESS);

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    getConfig(&serverAddress);

    // ignore CTRL+C 
    if(signal(SIGINT, SIG_IGN) == SIG_ERR){
        perror("signal"), exit(EXIT_FAILURE);
    }

    if(connect(socket_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }
   
    // read a message from server
    if((n_bytes = read(socket_fd, buffer, MESSAGE_SIZE)) < 0){
             perror("read"), exit(EXIT_FAILURE);
    }
    buffer[n_bytes] = '\0';
    printf("%s\n", buffer);
    memset(buffer, '\0', n_bytes);

    // @user command control during menu choice
    strcpy(choice, user_Count(choice, socket_fd));

    // if user has digit '5' in order to exit
    if(choice[0] == '5'){
        // send the char in order to close the connection from server
        if(write(socket_fd, "5", 1) != 1)
            perror("write"), exit(EXIT_FAILURE);
        close(socket_fd);
        exit(EXIT_SUCCESS);
    }

    buff[0] = choice[0];
    buff[1] = '\0';
    // debug for room's number printf("Your choice was %s\n", choice);

    printf("Enter a nickname: ");
    scanf(" %19[^\n]s%*c", nickname);
    //debug printf("Nickname entered is %s\n", nickname);

    // Now we concatanate the nickname to the room choosed
    strcat(buff, " ");
    strcat(buff, nickname);
    // debug: printf("Complete buffer is = %s\n", buff);

    // send message in form ROOM + SPACE + NICKNAME
    if(write(socket_fd, buff, strlen(buff)) != strlen(buff)){
        perror("write");
        exit(EXIT_FAILURE);
    }

    // check if nickname is valid:
    //  - strcmp(buffer, "OK") == 0 if nickname is valid
    //  - "KO" otherwise
    if((n_bytes = read(socket_fd, buffer, MESSAGE_SIZE)) < 0){
        perror("read"), exit(EXIT_FAILURE);
    }
    buffer[n_bytes] = '\0';
    //debug printf("Server: %s\n", buffer);

    if(strcmp(buffer, "KO") == 0)
        user_Check(socket_fd, choice[0]);

    // wait message after everything is OK
    if((n_bytes = read(socket_fd, buffer, MESSAGE_SIZE)) < 0){
        perror("read"), exit(EXIT_FAILURE);
    }
    buffer[n_bytes] = '\0';
    printf("%s\n", buffer);
    
     // in case of queue full close client
    if(strcmp(buffer, "Queue full, try later...") == 0)
    {
       printf("Closing connection...\n");
       close(socket_fd);
       exit(0);
    }
     
    int exit_chat = 0;
    while(exit_chat != 1){
        printf("****************************************************\n");
        do {
            FD_SET(STDIN_FILENO, &readfds);
            FD_SET(socket_fd, &readfds);

            if(select(socket_fd + 1, &readfds, NULL, NULL, &tv) < 0){
                perror("select");
                exit(EXIT_FAILURE); 
            }

            // read from keyboard and write to server
            if (FD_ISSET(STDIN, &readfds)){
                fgets(buffer, MESSAGE_SIZE, stdin);
                if(buffer[strlen(buffer) - 1] == '\n')
                    buffer[strlen(buffer) - 1] = '\0';

                if(write(socket_fd, buffer, strlen(buffer)) != strlen(buffer)){
                    perror("write"), exit(EXIT_FAILURE);
                }

                if(strcmp(buffer,"@stop") == 0 || strcmp(buffer,"@exit") == 0 || strcmp(buffer,"@user") == 0){
                    if (strcmp(buffer, "@exit") == 0){ 
                        exit_chat = 1; 
                        break;
                    } else if(strcmp(buffer, "@user") == 0)
                        printf("\n");
                }
            // read from server and print the message
            } else if (FD_ISSET(socket_fd, &readfds)) {
                if((n_bytes = read(socket_fd, buffer, MESSAGE_SIZE)) < 0){ 
                    perror("write"), exit(EXIT_FAILURE);
                }
                else if ( n_bytes == 0) {printf("Server has close connection ...\n");close(socket_fd);exit(0);}
                buffer[n_bytes] = '\0';
                printf("%s\n", buffer);
            } else {
                printf("timeout\n");
                strcpy(buffer,"@exit");
                if(write(socket_fd, buffer, strlen(buffer)) != strlen(buffer)){
                    perror("write"), exit(EXIT_FAILURE);
                }
                exit_chat = 1;
                break;
            }

            FD_ZERO(&readfds);
        } while (strcmp(buffer, "@stop") != 0);
        
    }

    printf("Goodbye!\n");

    close(socket_fd);

    return 0;
}

char *user_Count(char* choice, int sock_sd){
    char buffer[MESSAGE_SIZE];
    ssize_t n_bytes;

    while(strcmp(choice, "@user") == 0){

        if(write(sock_sd, choice, strlen(choice))<0){
            perror("write");
            exit(EXIT_FAILURE);
        }

        if((n_bytes = read(sock_sd, buffer, MESSAGE_SIZE)) < 0){
                perror("read"), exit(EXIT_FAILURE);
        }
        buffer[n_bytes] = '\0';
        memset(choice, '\0', strlen(choice));
        
        printf("%s\n", buffer);
        choice = menu();
    }
    
    return(choice);
}

void user_Check (int sock_sd, char room){
   char buffer[MESSAGE_SIZE];
   char nickname[NICKNAME_SIZE];
   char buffer_Nickname[MESSAGE_CONNECTION];
   ssize_t n_bytes;
   
   strcpy(buffer, "KO");

   while(strcmp(buffer ,"OK") != 0){
        memset(nickname, '\0', NICKNAME_SIZE);

        printf("Enter a new nickname: ");
        scanf(" %19[^\n]s%*c", nickname);

        buffer_Nickname[0] = room;
        buffer_Nickname[1] = '\0';
        strcat(buffer_Nickname, " ");
        strcat(buffer_Nickname, nickname);

        // send to server the message in form ROOM + SPACE + NICKNAME
        if(write(sock_sd, buffer_Nickname, strlen(buffer_Nickname)) != strlen(buffer_Nickname)){
           perror("read"), exit(EXIT_FAILURE);
        }

        if((n_bytes = read(sock_sd, buffer , MESSAGE_SIZE)) < 0){
                perror("read"), exit(EXIT_FAILURE);
        }  
        buffer[n_bytes] = '\0';
    }
    return;
}

void getConfig(struct sockaddr_in* ser_address){
    FILE *fp;
    char buffer[BUFFSIZE];
    char *token;
    const char delim[2] = "=";
    char array[ARRAY_CONFIG_SIZE][MAX_LENGTH];

    if ((fp = fopen("clientConfig.txt","rt"))==NULL) {
            printf("fopen");
            exit(EXIT_FAILURE);
    }


   int i = 0;
   while(fgets(buffer, BUFFSIZE, fp) != NULL) {
        /* primo token prende il primo campo a sinistra del uguale*/
        token = strtok(buffer, delim);

        /* secondo token prende il campo a destra dell uguale che è quello che ci interessa*/
        token = strtok(NULL, delim);

        strcpy(array[i], token);
        i++;
          
        memset(buffer,'\0', BUFFSIZE);
    }             
    
    fclose(fp);

   ser_address->sin_family = PF_INET;
   ser_address->sin_port = htons(atoi(array[1]));
   inet_aton(array[0], &ser_address->sin_addr);
}

