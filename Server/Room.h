/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#ifndef NEW_ROOMX_H
#define NEW_ROOM_H

#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "CircularBuffer.h"
#include "AVL.h"

#define BUFFSIZE 256
#define NUM_STRING 5
#define MAX_LENGTH 20

/* ******************************************************* */
/*                      GLOBAL VARIABLE
/* ******************************************************* */
extern int ROOM_CAPACITY;
extern int p_chatting_1;
extern int p_chatting_2;
extern int p_chatting_3;
extern int p_chatting_4;

extern Tree *tree_room_1;
extern Tree *tree_room_2;
extern Tree *tree_room_3;
extern Tree *tree_room_4;

extern int size_tree1;
extern int size_tree2;
extern int size_tree3;
extern int size_tree4;

extern Queue *queue1;
extern Queue *queue2;
extern Queue *queue3;
extern Queue *queue4;
/* ******************************************************* */



/* ******************************************************* */
/*                      PROTOTYPES
/* ******************************************************* */

/**
 * @brief Returns tree size by room
 * @param room the room relating to the tree
 * @return int 
 */
int getSizeTreeByRoom(char room);

/**
 * @brief remove from the correct tree the user who has write @exit
 * 
 * @param room the user's room
 * @param nick the user's nickname 
 * @return Tree* 
 */
Tree *deletefromTree(char room, char* nick);

/**
 * @brief insert into correct tree the client by their room
 * 
 * @param user the client's data
 * @param tid the thread's id who manage the current client
 * @return Tree* 
 */
Tree* insert_Tree(struct T_user user , pthread_t tid);

/**
 * @brief search the node of the user into the correct tree by their nickname
 * 
 * @param room the room of the tree where is located the user
 * @param nick the user's nickname
 * @return Tree* 
 */
Tree *search_Tree(char room, char *nick);

/**
 * @brief update all nodes child field of the correct tree  
 * 
 * @param room the room relating to the tree that needs updating
 * @return Tree* 
 */
Tree *insertChildrenByRoom(char room);

/**
 * @brief get random node of the correct tree
 * 
 * @param room the room relating to the tree
 * @return Tree* 
 */
Tree *randomNodeRoom(char room);

/**
 * @brief print the correct tree
 * 
 * @param room the room relating to the tree
 */
void  printTreeByRoom(char room);

/**
 * @brief enquque the user in the correct queue (by their room)
 * 
 * @param user 
 * 
 * @return false in case of overflow, true otherwise
 */
bool manageEnqueue(struct T_user user);

/**
 * @brief dequeue the user who wainting from the correct queue
 * 
 * @param room the room where user wait
 * @return struct T_user 
 */
struct T_user manageDequeue(char room);

/**
 * @brief Utility function used when the client connected to the server. 
 * The function will check if the nickname is unique and if @user command has been written
 * 
 * @param sd the client's sd
 * @return struct T_user 
 */
struct T_user getUserClt(int sd);

/**
 * @brief Function used when user write @user. 
 * The function will send to the client the number, for each room, of users who are talking and users who are waiting
 * 
 * @param sd current client's sd
 */
void numberUsers(int sd);

/**
 * @brief a simple menu where the admin can see some information such as user in a room (AVL), user who are waiting
 * 
 */
void serverMenu();

/**
 * @brief This function take an integer and increment/decrement the numbers of people who are talking
 * 
 * @param room current room where increase/decrease the number of talking people
 * @param integer a positive or negative number
 * @return int 
 */
int people_ChattingFunc(char room, int integer);

/**
 * @brief This function takes the client's message which has the form ROOM + 1_SPACE + NICKNAME and
 * split that in order to save room and nickname in the user structure
 * 
 * @return struct T_user 
 */
struct T_user getUser(char *);

/**
 * @brief This function checks if a recalculation of the random node is required or if the first user has to wait
 * 
 * @return true if recalculation is necessary
 * @return false otherwise
 */
bool clientRecalculation(int, Tree *, Tree *);

/**
 * @brief A utility function used to read from a configuration file the capacity room, the server's IP and store them
 * 
 */
void getConfig(struct sockaddr_in *); 

/**
 * @brief send a info message from server to client
 * 
 * @param msg the message to send. 
 * If the length of the message exceeds 256 bytes, the message will be truncated
 * @param sd the client's socket descriptor
 */
void sendMsg(char *msg, int sd);

#endif //NEW_MUTEX__H