/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include "Room.h"

#include "Mutex.h"

// SUPPORT FUNCTIONS
static bool search_Queue(struct T_user user);

/* ******************************************************* */
/*                      TREE FUNCTIONS
/* ******************************************************* */

int getSizeTreeByRoom(char room){
    switch(room){
        case '1' : return size_tree1;
        case '2' : return size_tree2;
        case '3' : return size_tree3;
        case '4' : return size_tree4;
    }
}

Tree *deletefromTree(char room, char* nick){
   switch (room){
        case '1': 
            if(search(tree_room_1, nick) != NULL){
                size_tree1 --; 
                tree_room_1 = deleteFromAVL(tree_room_1, nick);
            }
            return tree_room_1;

        case '2': 
            if(search(tree_room_2, nick) != NULL){
                size_tree2 --; 
                tree_room_2 = deleteFromAVL(tree_room_2, nick);
            }
            return tree_room_2;

        case '3': 
            if(search(tree_room_3, nick) != NULL){
                size_tree3 --; 
                tree_room_3 = deleteFromAVL(tree_room_3, nick);
            }
            return tree_room_3;

        case '4': 
            if(search(tree_room_4, nick) != NULL){
                size_tree4 --; 
                tree_room_4 = deleteFromAVL(tree_room_4, nick);
            }
            return tree_room_4;
    }
 }

Tree *insert_Tree(struct T_user user, pthread_t  tid)
{
    switch (user.room){
        case '1': 
            tree_room_1 = insert(tree_room_1, user.nickname, user.room, user.user_sd, 'W', tid);
            size_tree1++; 
            return tree_room_1;

        case '2' : 
            tree_room_2 = insert(tree_room_2, user.nickname, user.room, user.user_sd, 'W', tid);
            size_tree2 ++; 
            return tree_room_2;

        case '3': 
            tree_room_3 = insert(tree_room_3, user.nickname, user.room, user.user_sd, 'W', tid);
            size_tree3 ++; 
            return tree_room_3;

        case '4': 
            tree_room_4 = insert(tree_room_4, user.nickname, user.room, user.user_sd, 'W', tid);
            size_tree4 ++; 
            return tree_room_4;
    }
}

Tree *search_Tree(char room, char *nick)
{
    switch (room)
    {
        case '1' : return search(tree_room_1, nick);
        case '2' : return search(tree_room_2, nick);
        case '3' : return search(tree_room_3, nick);
        case '4' : return search(tree_room_4, nick);
    }
}

Tree *insertChildrenByRoom(char room)
{
   switch (room){
      case '1' : return insertChildrenCount(tree_room_1);
      case '2' : return insertChildrenCount(tree_room_2);
      case '3' : return insertChildrenCount(tree_room_3);
      case '4' : return insertChildrenCount(tree_room_4);
    }
}

Tree *randomNodeRoom(char room)
{
   switch (room)
   {
      case '1' : return randomNode(tree_room_1);
      case '2' : return randomNode(tree_room_2);
      case '3' : return randomNode(tree_room_3);
      case '4' : return randomNode(tree_room_4);
    }
}

void printTreeByRoom(char room)
{
    printf ("Print Tree room =%c\n(nickname, room, sd, state, last partener)\n\n", room);
    switch (room)
    {
        case '1' : printTree(tree_room_1, 0); printf("\n"); break;
        case '2' : printTree(tree_room_2, 0); printf("\n"); break;
        case '3' : printTree(tree_room_3, 0); printf("\n"); break;
        case '4' : printTree(tree_room_4, 0); printf("\n"); break;
    }
}

/* ******************************************************* */



/* ******************************************************* */
/*                      QUEUE FUNCTIONS
/* ******************************************************* */

int manageEnqueue(struct T_user user){
    int queue_old_size;

    switch(user.room){
        case '1': return enqueue(queue1, user, queue_room_1);
            
        case '2': return enqueue(queue2, user, queue_room_2);
            
        case '3': return enqueue(queue3, user, queue_room_3);

        case '4': return enqueue(queue4, user, queue_room_4);
    }
}

struct T_user manageDequeue(char room){
    struct T_user extracted;

    switch(room){
        case '1':
            extracted = dequeue(queue1, queue_room_1);
            return extracted;

        case '2':
            extracted = dequeue(queue2, queue_room_2);
            return extracted;

        case '3':
            extracted = dequeue(queue3, queue_room_3);
            return extracted;

        case '4':
            extracted = dequeue(queue4, queue_room_4);
            return extracted; 
    }
}

/* ******************************************************* */



/* ******************************************************* */
/*                    UTILITY FUNCTIONS
/* ******************************************************* */

void numberUsers(int sd)
{
    printf("Received @user\n");
    int  user1 = getSizeTreeByRoom('1') + getQueueSize(queue1);           
	int  user2 = getSizeTreeByRoom('2') + getQueueSize(queue2);
    int  user3 = getSizeTreeByRoom('3') + getQueueSize(queue3);
    int  user4 = getSizeTreeByRoom('4') + getQueueSize(queue4);

    char utenti1[62];
	char utenti2[62];
    char utenti3[62];
    char utenti4[62];
    char msg[248];

    memset(msg,'\0', 248);
    sprintf(utenti1, "\nLa stanza tematica 1 contiene %d utenti di cui %d in coda;\n", user1, getQueueSize(queue1));
    sprintf(utenti2, "La stanza tematica 2 contiene %d utenti di cui %d in coda;\n", user2, getQueueSize(queue2));
    sprintf(utenti3, "La stanza tematica 3 contiene %d utenti di cui %d in coda;\n", user3, getQueueSize(queue2));
    sprintf(utenti4, "La stanza tematica 4 contiene %d utenti di cui %d in coda;\n", user4, getQueueSize(queue3));
    
    strcpy(msg, utenti1);
    strcat(msg, utenti2);
    strcat(msg, utenti3);      
    strcat(msg, utenti4);

    if((write(sd, msg, strlen(msg))) != strlen(msg)){
        perror("write");
        exit(EXIT_FAILURE);
	}
	
    return;
}

struct T_user getUserClt(int sd) {
    int SIZE_INIT_MESSAGE = NICKNAME_SIZE + 3;  //room + space + nickname + \0

    char buffer[60]; 
    char msg_client[SIZE_INIT_MESSAGE];
    ssize_t n_bytes = 0;
    struct T_user user;
    bool userFound;                 // to check if nickname is already present in a queue
    Tree *userSearch = NULL;        // to check if nickname is already present in a tree

    sendMsg("Welcome to the multichat server!", sd);
    
    userFound = true;

    while(userFound) {
        userFound = false;

        if((n_bytes = recv(sd, msg_client, sizeof(msg_client), 0)) < 0){
            fprintf(stderr, "receive error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        msg_client[n_bytes] = '\0';

        while(strcmp(msg_client, "@user") == 0){
          numberUsers(sd);

          if((n_bytes = recv(sd, msg_client, sizeof(msg_client), 0)) < 0){
              fprintf(stderr, "recv error: %s\n", strerror(errno));
              exit(EXIT_FAILURE);
          }
          msg_client[n_bytes] = '\0';
        }

        // the user has digit '5' (exit on client menu) after @user
        if(msg_client[0] == '5'){
            user.room = '5';
            return user;
        }

        user = getUser(msg_client);
        userSearch = search_Tree(user.room, user.nickname);
        if(userSearch != NULL) {
            printf ("Nickname already present...\n");
            userFound = true;   
        } else {
          userFound = search_Queue(user);
          if (userFound)  
            printf ("Nickname already present...\n");
       }

        if(userFound){
          sendMsg("KO", sd);
        } else {
            sendMsg("OK", sd);
        }
    }
    return(user);
}

static bool search_Queue(struct T_user user)
{
    switch(user.room){
        case '1' :
            return searchqueue(queue1, user.nickname, queue_room_1);
            break;

        case '2' :
            return searchqueue(queue2, user.nickname, queue_room_2);
            break;

        case '3' :
            return searchqueue(queue3, user.nickname, queue_room_3);
            break;

        case '4' :
            return searchqueue(queue4, user.nickname, queue_room_4);
            break;
            
        default :
            return false;
    }
}

void serverMenu(){
    int choice, flag;
    char second_choice;

    printf("\n********************************\n");
    printf("************* MENU *************\n");

    do{
        printf("1. View all user in a room.\n");
        printf("2. View users who waiting.\n");
        printf("3. Go back\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);
        // if user enter a string instead of only one character
        while((getchar()) != '\n');

        switch(choice){
            case 1:
                printTreeByRoom('1');
                printTreeByRoom('2');
                printTreeByRoom('3');
                printTreeByRoom('4');
                printf("\n");
                break;
            case 2:
                do{
                    printf("\ta. Queue of room 1\n");
                    printf("\tb. Queue of room 2\n");
                    printf("\tc. Queue of room 3\n");
                    printf("\td. Queue of room 4.\n");

                    printf("Which queue: ");
                    scanf(" %c", &second_choice);

                    while((getchar()) != '\n');

                    switch (second_choice)
                    {
                        case 'a':
                            print(queue1, queue_room_1);
                            printf("\n");
                            flag = 1;
                            break;

                        case 'b':
                            print(queue2, queue_room_2);
                            printf("\n");
                            flag = 1;
                            break;

                        case 'c':
                            print(queue3, queue_room_3);
                            printf("\n");
                            flag = 1;
                            break;

                        case 'd':
                            print(queue4, queue_room_4);
                            printf("\n");
                            flag = 1;
                            break;

                        default:
                            printf("Invalid choice.\n\n");
                            break;
                    }

                }while(flag != 1);
                break;

            case 3:
                break;

                return;

            default:
                printf("Wrong choice!\n\n");
        }

    } while (choice != 3);
    printf("********************************\n");
}

struct T_user getUser(char *msg){
    struct T_user user;
    char nickname[NICKNAME_SIZE];

    // get room chosen by client
    char *token = strtok(msg, " ");
    user.room = token[0];

    while(token != NULL){
        strcpy(nickname, token);
        token = strtok(NULL, " ");
    }

    strcpy(user.nickname, nickname);
    
    return user;
}

int people_ChattingFunc(char r, int inc)
{
    switch(r){
        case '1' : return p_chatting_1 += inc;
        case '2' : return p_chatting_2 += inc;
        case '3' : return p_chatting_3 += inc;
        case '4' : return p_chatting_4 += inc;
    }
} 

bool clientRecalculation(int r, Tree* first, Tree* second){
    mutex_lock('M', r);

    if(getSizeTreeByRoom(r) - people_ChattingFunc(r, 0) < 2) return true;

    if(getSizeTreeByRoom(r) - people_ChattingFunc(r, 0) == 2){
    mutex_unlock('M', r);
    
        if(strcmp(first->key.nickname, second -> key.nickname)==0)   
            return false;

        if(first -> key.state != 'W') 
            return true; 
        if(second -> key.state != 'W' ) 
            return false;

        if(strcmp(first->key.nickname, second -> key.nickname_partner) == 0) {
            //debug printf ("cond_wait firstuser-nixk=%s, seconduser-nickpartner=%s\n", first -> key.nickname, second -> key.nickname_partner);
            return true;
        }
    }
    return false;
}

void getConfig(struct sockaddr_in* ser_address)
{
    FILE *fp;
    char buffer[BUFFSIZE];
    char *token;
    const char delim[2] = "=";
    char array[NUM_STRING][MAX_LENGTH];

    if ((fp = fopen("serverConfig.txt", "rt")) == NULL) {
        printf("open");
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
   	 
    ser_address->sin_family = AF_INET;
    ser_address->sin_port = htons(atoi(array[0]));
    ser_address->sin_addr.s_addr = htonl(INADDR_ANY);
         
    ROOM_CAPACITY = atoi(array[1]);
}

void sendMsg(char *msg, int sd){
    char buffer[BUFFSIZE];

    int j = snprintf(buffer, sizeof(buffer), "%s", msg);
    
    if (j >= sizeof(buffer))
        fputs("Buffer length exceeded; string truncated\n",
              stderr);
    
    if(write(sd, buffer, strlen(buffer)) != strlen(buffer)){
        perror("write"), exit(EXIT_FAILURE);
    }
}