/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#ifndef NEW_SERVER_CIRCULARBUFFER_H
#define NEW_SERVER_CIRCULARBUFFER_H

#define QUEUE_SIZE 10                         // Circular buffer size

struct queue{
    int front;
    int rear;
};
typedef struct queue Queue;

#include "AVL.h"
#include <stdbool.h>

struct T_user queue_room_1[QUEUE_SIZE];      // User who waiting to enter in room 1
struct T_user queue_room_2[QUEUE_SIZE];      // User who waiting to enter in room 2
struct T_user queue_room_3[QUEUE_SIZE];      // User who waiting to enter in room 3
struct T_user queue_room_4[QUEUE_SIZE];      // User who waiting to enter in room 4

/**
 * @brief initialize front and rear of a queue in order to get different indexes
 * 
 * @param q a queue
 * @return Queue* 
 */
Queue *init(Queue *q);

/**
 * @brief enqueue a user in the queue
 * 
 * @param q the queue struct with indexes for the correct array
 * @param user the user who has to enqueue
 * @param queue the room where user have to wait
 * 
 * @return false if overflow, true otherwise
 */
bool enqueue(Queue *q, struct T_user user, struct T_user queue[]);

/**
 * @brief dequeue the user from waiting queue
 * 
 * @param q queue struct with indexes for the correct array
 * @param queue the room where user is located
 * @return struct T_user 
 */
struct T_user dequeue(Queue *q, struct T_user queue[]);

/**
 * @brief check if a nickname is already present in queue
 * 
 * @param q the queue struct with indexes for the correct array
 * @param nickname nickname to find
 * @param queue current room
 * @return true if founded
 * @return false otherwise
 */
bool searchqueue(Queue *q, char *nickname, struct T_user queue[]);

void print(Queue *q, struct T_user *);

int getQueueSize(Queue *q);

#endif //NEW_SERVER_CIRCULARBUFFER_H
