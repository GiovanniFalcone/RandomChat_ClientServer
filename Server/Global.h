/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#ifndef NEW_GLOBAL_H
#define NEW_GLOBAL_H
 
#include "AVL.h"
#include "CircularBuffer.h"

char OPZIONE_LOCK = 'T';  // per eventuale trylock
int ROOM_CAPACITY;

pthread_mutex_t mutex1 ;
pthread_mutex_t mutex2 ;
pthread_mutex_t mutex3 ;
pthread_mutex_t mutex4 ;

pthread_mutex_t chat_mutex_1 ;
pthread_mutex_t chat_mutex_2 ;
pthread_mutex_t chat_mutex_3 ;
pthread_mutex_t chat_mutex_4 ;

pthread_mutex_t chat_mutex2_1 ;
pthread_mutex_t chat_mutex2_2 ;
pthread_mutex_t chat_mutex2_3 ;
pthread_mutex_t chat_mutex2_4 ;

pthread_cond_t q_cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t q_cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t q_cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t q_cond4 = PTHREAD_COND_INITIALIZER;

pthread_cond_t chat_cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t chat_cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t chat_cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t chat_cond4 = PTHREAD_COND_INITIALIZER;

pthread_cond_t chat_cond2_1 = PTHREAD_COND_INITIALIZER; // wait cond per il secondario
pthread_cond_t chat_cond2_2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t chat_cond2_3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t chat_cond2_4 = PTHREAD_COND_INITIALIZER;
  
int size_tree1;
int size_tree2;
int size_tree3;
int size_tree4;

int p_chatting_1;
int p_chatting_2;
int p_chatting_3;
int p_chatting_4; 

Tree *tree_room_1 = NULL;
Tree *tree_room_2 = NULL;
Tree *tree_room_3 = NULL;
Tree *tree_room_4 = NULL;

Queue *queue1;
Queue *queue2;
Queue *queue3;
Queue *queue4;

#endif
