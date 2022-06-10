
/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/poll.h>

#include "Global.h"
#include "Room.h"
#include "Mutex.h"

#define MESSAGE_SIZE 256

/* ************************************************************************************************************ */
/* ********************************************* PROTOTYPES *************************************************** */
/* ************************************************************************************************************ */

/**
 * @brief the thread that manage their client
 * 
 * @param client_sd sd of client
 * @return void* 
 */
static void *manageClient(void *client_sd);

/**
 * @brief a thread that read from keyboard in order to see a menu 
 * 
 * @return void* 
 */
static void *manageServer(void *);

/**
 * @brief Utility function that sends out some  messages to the clients of the current chat
 * 
 * @param firstUser 
 * @param secondUser 
 */
static void welcomeToTheChat(Tree *firstUser, Tree *secondUser);

/**
 * @brief a thread that reads @exit of the current user who has waiting
 * 
 * @param firstUser the current user
 * @return void* 
 */
static void *exitUser(void *firstUser);

/**
 * This function initialize chat between two client using select function.
 * @param first_User_Node first user node selected randomly
 * @param second_User_Node second user node selected randomly
 * @return nickname of user who write "exit" into the chat, NULL otherwise
 */
static struct T_user init_chat(Tree *first_User_Node, Tree *second_User_Node);

/**
 * This function manage the chat between two client using send and receive functions.
 * @param from_Client the user who has write
 * @param to_Client the user who will have to read what the first one wrote
 * @return
 */
static struct T_user manage_chat(Tree *from_Client, Tree *to_Client);

/**
 * This function creates a random chat by selecting random nodes from the tree.
 * It does not modify the tree but changes the information of the two selected nodes.
 * @param room chosen room by client
 * @return nickname of user who write "exit" into the chat, NULL otherwise
 */
struct T_user create_Chat(char room, char nickname_current_user[]);

/* ************************************************************************************************************ */

// GLOBAL VARIABLES
struct timeval timeout;
time_t ora;

void handler(int signum){
    if(signum == SIGUSR1){
        pthread_exit(EXIT_SUCCESS);
    }
}

int main() {
    struct sockaddr_in server_addr,  client_addr;
    int server_sd, client_sd, *thread_sd;
    socklen_t client_len;
    pthread_t tid;

    size_tree1 = 0;
    size_tree2 = 0;
    size_tree3 = 0;
    size_tree4 = 0;

    queue1 = init(queue1);
    queue2 = init(queue2);
    queue3 = init(queue3);
    queue4 = init(queue4);
    
    ora = time(NULL);
    printf("MULTICHAT SERVER START PROCESSING :%s\n", asctime(localtime(&ora)));

    // In order to get randomly new nodes from the user tree of a certain room
    srand(time(NULL));

    // signal for a thread that manage @exit when user is not in talking state
    signal(SIGUSR1, handler);

    getConfig(&server_addr);
    printf("Room capacity = %d\n", ROOM_CAPACITY);

    if((server_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Socket error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(bind(server_sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(listen(server_sd, 10) < 0){
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Allow for fast restart
    int option_value = 1;
    setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));

    // Create a thread for debugging
    int err;
    if(((err = pthread_create(&tid, NULL, manageServer, (void*) thread_sd)) != 0)){
            fprintf(stderr, "thread create: %s\n", strerror(err));
            exit(EXIT_FAILURE);
    }

    while(1){
        client_len = sizeof(client_addr);

        if((client_sd = accept(server_sd, (struct sockaddr*)&client_addr, &client_len)) < 0){
            fprintf(stderr, "accept error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        //not thread safe, used just for debug
        fprintf(stderr, "\nNew connection from : %s\n", inet_ntoa(client_addr .sin_addr));

        thread_sd = (int*)malloc(sizeof(int));
        *thread_sd = client_sd;

        if(((err = pthread_create(&tid, NULL, manageClient, (void*) thread_sd)) != 0)){
            fprintf(stderr, "thread create: %s\n", strerror(err));
            exit(EXIT_FAILURE);
        }
        
        if((err = pthread_detach(tid)) != 0){
            fprintf(stderr, "thread detach: %s\n", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

static void *manageClient(void *client_sd) {
    struct T_user user;
    int sd, err;
    ssize_t n_bytes;
    
    sd = *((int *) client_sd);
    printf("%ld: manage sd %d\n", pthread_self(), sd);

    user = getUserClt(sd);
    user.user_sd = sd;

    printf("User %s for room number %c\n", user.nickname, user.room);
    if(user.room == '5'){
        printf("The client close the connection...\n");
        close(sd);
        free((int*)client_sd);
        pthread_exit(0);
    }

    mutex_lock('M', user.room);
    if(getSizeTreeByRoom(user.room) < ROOM_CAPACITY){
        insert_Tree(user, pthread_self());
    } else {
        int res = manageEnqueue(user);
        if(res == 1){
            printf("[%s] Queue full, close client...\n", user.nickname);
            sendMsg("Queue full, try later...", sd);
            close(sd);
            free((int*) client_sd);
            pthread_exit(0);
        }
        while(getSizeTreeByRoom(user.room) == ROOM_CAPACITY){ 
            printf("[%s] Room full\n", user.nickname);
            sendMsg("Room full, you have to wait...\n", sd);
	        cond_wait('Q', user.room);
        }
        user = manageDequeue(user.room); 
        printf("[%s] user dequeued\n" ,user.nickname);
        insert_Tree(user, pthread_self());
    }    
    cond_broadcast('C', user.room); 
    cond_broadcast('S', user.room);
    mutex_unlock('M', user.room);

    struct T_user tmp_user;
          
    while(1){
        tmp_user = create_Chat(user.room, user.nickname);
        sleep(1);
        if(tmp_user.exit == 1){  
            printf("[%d] %s left the chat\n", sd, tmp_user.nickname);
            fflush(stdout);

            // delete the user from the tree and wake up who were waiting in queue
            mutex_lock('M', tmp_user.room);
            deletefromTree(tmp_user.room, tmp_user.nickname);
            cond_broadcast('Q', tmp_user.room);
            mutex_unlock('M', tmp_user.room);

            close(tmp_user.user_sd);

            // if the sd is equal to the sd of the user who has write "@exit" then break and exit from current thread
            if(tmp_user.user_sd == sd) {
                break;
	        }  
        } 
    }

    printf("[%d] exit from server \n",sd);
    free((int*) client_sd);
    pthread_exit(0);
}

static void *manageServer(void *args){
    char buffer[256];
    while(1){
        if (fgets(buffer, 256, stdin) != buffer) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }
        // Remove final \n, if present
        int len = strlen(buffer);
        if (buffer[len-1] == '\n')
            buffer[len-1] = '\0';

        if(strcmp(buffer, "@menu") == 0)
            serverMenu();    
    }
}

struct T_user create_Chat(char room, char nickname_current_user[]){
    pthread_t tid;
    Tree *firstUser = NULL, *secondUser = NULL;
    struct T_user user;
    int err, people_condition, t_kill = 0;
    
    printf("\nCreate chat in room %c\n", room);
    
    mutex_lock('C', room);
    firstUser = search_Tree(room, nickname_current_user);
    printf("[%s] First client data: sd: %d room: %c, state: %c\n", firstUser -> key.nickname, firstUser -> key.user_sd,
           firstUser -> key.room, firstUser -> key.state);
           
    mutex_lock('M', room);       
    people_condition = getSizeTreeByRoom(room) - people_ChattingFunc(room, 0); 
    mutex_unlock('M', room);     
    
    while(people_condition < 2 || firstUser -> key.state == 'T'){
        // create a thread in order to read @exit while user is waiting
        if(firstUser -> key.state == 'W'){
                t_kill = 1;
                if(((err = pthread_create(&tid, NULL, exitUser, (void*) firstUser)) != 0)){
                    fprintf(stderr, "thread create: %s\n", strerror(err));
                    exit(EXIT_FAILURE);
                }
            
                if((err = pthread_detach(tid)) != 0){
                    fprintf(stderr, "thread detach: %s\n", strerror(err));
                    exit(EXIT_FAILURE);
                }

                sendMsg("Wait for chat...\n", firstUser -> key.user_sd);
        }

        printf("[%s] cond_wait \n", firstUser -> key.nickname);
        cond_wait('C', room);

        // if a thread was created then send a signal to kill it
        if(t_kill != 0) pthread_kill(tid, SIGUSR1);

        // if user has write @exit while they were waiting then return them in order to delete their node from tree
        if(firstUser -> key.exit == 1) return firstUser -> key;

        printf("\n[%s] Possibile chat?\n", firstUser -> key.nickname);

        mutex_lock('M', room);       
        people_condition = getSizeTreeByRoom(room) - people_ChattingFunc(room, 0);
        mutex_unlock('M', room); 
    }
    mutex_unlock('C', room);
    
    mutex_lock('M', room);
    insertChildrenByRoom(room);
    secondUser = randomNodeRoom(room);
    mutex_unlock('M', room);
    
    mutex_lock('S', room);
    t_kill = 0;
    while(secondUser -> key.state != 'W' ||  firstUser -> key.state != 'W' || secondUser -> key.exit != 0
		    || (strcmp(secondUser -> key.nickname, firstUser -> key.nickname_partner) == 0)
                    || (strcmp(secondUser -> key.nickname, firstUser -> key.nickname) == 0)) {
        while(clientRecalculation(firstUser -> key.room, firstUser, secondUser)) { 
            // create a thread in order to read @exit while user is waiting
            if(firstUser -> key.state == 'W'){
                t_kill = 1;
                if(((err = pthread_create(&tid, NULL, exitUser, (void*) firstUser)) != 0)){
                    fprintf(stderr, "thread create: %s\n", strerror(err));
                    exit(EXIT_FAILURE);
                }
            
                if((err = pthread_detach(tid)) != 0){
                    fprintf(stderr, "thread detach: %s\n", strerror(err));
                    exit(EXIT_FAILURE);
                }

                sendMsg("Wait for chat...\n", firstUser -> key.user_sd);
            }
            
            printf("[%s] wait for second user \n", firstUser -> key.nickname);
            cond_wait('S',room);

            // send signal to thread who handle the @exit in order to close it
            if(t_kill != 0) pthread_kill(tid, SIGUSR1);
        
            // check if the user, during the waiting, has write @exit
            if(firstUser -> key.exit == 1)  return firstUser -> key;

            printf("[%s] possible chat \n", firstUser -> key.nickname);
        }
        
        mutex_lock('M', room);
        insertChildrenByRoom(room);  
	    secondUser = randomNodeRoom(room);
        mutex_unlock('M', room);
    }
    mutex_unlock('S',room);

    printf("\n[%s] Chat with partner %s\n", firstUser -> key.nickname, secondUser -> key.nickname);

    /* update info of nodes chosen */
    firstUser -> key.state = 'T';
    secondUser -> key.state = 'T';

    strcpy(firstUser -> key.nickname_partner, secondUser -> key.nickname);
    strcpy(secondUser -> key.nickname_partner, firstUser -> key.nickname);

    firstUser -> key.stop = 0;
    secondUser -> key.stop = 0;
    firstUser -> key.exit = 0;
    secondUser -> key.exit = 0;
    /* end update */

    mutex_lock('C', user.room);
    mutex_lock('S', user.room);
    people_ChattingFunc(room, 2);
    mutex_unlock('S', user.room);
    mutex_unlock('C', user.room);

    welcomeToTheChat(firstUser, secondUser);

    user = init_chat(firstUser, secondUser);

    mutex_lock('C', user.room);
    mutex_lock('S', user.room);
    people_ChattingFunc(room, -2);
    cond_broadcast('C', user.room);
    cond_broadcast('S', user.room);
    mutex_unlock('S', user.room);
    mutex_unlock('C', user.room);

    return user;
}

static void *exitUser(void *arg){
    Tree *firstUser = (Tree*) arg;
    char buff[81];

    memset(buff, '\0', 81);
    strcpy(buff, "\nDigit @exit if you want to exit...\n");
    if((write(firstUser -> key.user_sd, buff, strlen(buff)) != strlen(buff))){
        perror("send first user");
        exit(EXIT_FAILURE);
    }
    memset(buff, '\0', 81);

    if((read(firstUser -> key.user_sd, buff, 5)) < 0) {
        perror("read first user");
        exit(EXIT_FAILURE);
    }
    
    if (strcmp(buff, "@exit") == 0){
	    /* Wake up the thread who manage the user who has write @exit 
        *  in order to return their name and delete them from tree */
        firstUser -> key.exit = 1;
        cond_broadcast('C', firstUser -> key.room);
        cond_broadcast('S', firstUser -> key.room);
    }

    pthread_exit(EXIT_SUCCESS);
}

static void welcomeToTheChat(Tree *firstUser, Tree *secondUser){
    char writebuf[20];
    char writebuf2[128];
    char writebuf3[128];

    strcpy(writebuf, "Now you can chat!\n");
    strcpy(writebuf2, writebuf);
    strcat(writebuf2, "You are connected with ");
    strcat(writebuf2, secondUser -> key.nickname);
    strcat(writebuf2, "\n");

    if( (write(firstUser -> key.user_sd, writebuf2, strlen(writebuf2)) != strlen(writebuf2))) {
        perror("write first user");
        exit(EXIT_FAILURE);
    }

    strcpy(writebuf3, writebuf);
    strcat(writebuf3,"You are contacted by ");
    strcat(writebuf3, firstUser -> key.nickname);
    strcat(writebuf3, "\n");
    if((write(secondUser -> key.user_sd, writebuf3, strlen(writebuf3)) != strlen(writebuf3))){
        perror("send second user");
        exit(EXIT_FAILURE);
    }
}

static struct T_user init_chat(Tree *first_User_Node, Tree *second_User_Node){
    fd_set readfds;
    struct T_user user;              // string eventually return if a user write "exit" in chat

    timeout.tv_sec = 9000;
    timeout.tv_usec = 0;

    printf("Chat started between %s and %s\n", first_User_Node -> key.nickname, second_User_Node -> key.nickname);
    FD_ZERO(&readfds);              // initialize set (empty)

    int max_sd;
    max_sd = (first_User_Node -> key.user_sd > second_User_Node -> key.user_sd) ?
            first_User_Node -> key.user_sd : second_User_Node -> key.user_sd;    

    while(1){
        FD_SET(first_User_Node -> key.user_sd,  &readfds);  //Insert file descriptor of first user into the set
        FD_SET(second_User_Node -> key.user_sd, &readfds);  //Insert file descriptor of second user into the set

        if(select(max_sd + 1, &readfds, NULL, NULL, &timeout) < 0){
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(first_User_Node -> key.user_sd, &readfds)){
            user = manage_chat(first_User_Node, second_User_Node);
        } else if(FD_ISSET(second_User_Node -> key.user_sd, &readfds)){
            user = manage_chat(second_User_Node, first_User_Node);
        } else {
            printf("TIMEOUT\n");
            return first_User_Node -> key; 
        }

        FD_ZERO(&readfds);

        // return the user who has write @stop or @exit
        if(user.exit == 1 || user.stop == 1){
            return user;
        }
    }
}

static struct T_user manage_chat(Tree *from_Client, Tree *to_Client){
    char read_buffer[MESSAGE_SIZE];
    char write_buffer[MESSAGE_SIZE + NICKNAME_SIZE + 2];
    ssize_t n_read;
   
    printf ("From %s to %s\n", from_Client -> key.nickname, to_Client -> key.nickname);

    // read from first client
    if ((n_read = read(from_Client -> key.user_sd, read_buffer, MESSAGE_SIZE)) < 0){
        perror("recv first user");
        exit(EXIT_FAILURE);
    }
    read_buffer[n_read] = '\0';
    printf("%s 's message: %s\n", from_Client -> key.nickname, read_buffer);

    if(strcmp(read_buffer, "@user") == 0) {
        numberUsers(from_Client -> key.user_sd);
    } else {
        /* write on second client */
        sprintf(write_buffer, "%s: %s", from_Client -> key.nickname, read_buffer);
        if((write(to_Client -> key.user_sd, write_buffer, strlen(write_buffer))) != strlen(write_buffer)){
            perror("send second user");
            exit(EXIT_FAILURE);
        }
    }
   
    if(strcmp(read_buffer, "@exit") == 0){
	    from_Client -> key.state = 'W';
        from_Client -> key.exit = 1;
        from_Client -> key.stop = 0;
        to_Client -> key.state = 'W';
        to_Client -> key.exit = 0;
        to_Client -> key.stop = 0;
    } else  if(strcmp(read_buffer, "@stop") == 0) {
        from_Client -> key.state = 'W';
        from_Client -> key.stop = 1;
        from_Client -> key.exit = 0;
	    to_Client -> key.state = 'W';
        to_Client -> key.exit = 0;
        to_Client -> key.stop = 0;
    }

    return from_Client -> key;
}
