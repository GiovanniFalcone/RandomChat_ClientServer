/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#ifndef NEW_MUTEX_H
#define NEW_MUTEX_H 

#include <pthread.h>

extern char OPZIONE_LOCK;  // T PER TRILOCK  ;   DIVERSO DA T PER LOCK ESCLUSIVO

extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern pthread_mutex_t mutex3;
extern pthread_mutex_t mutex4;

extern pthread_mutex_t chat_mutex_1;
extern pthread_mutex_t chat_mutex_2;
extern pthread_mutex_t chat_mutex_3;
extern pthread_mutex_t chat_mutex_4;

extern pthread_mutex_t chat_mutex2_1;
extern pthread_mutex_t chat_mutex2_2;
extern pthread_mutex_t chat_mutex2_3;
extern pthread_mutex_t chat_mutex2_4;

extern pthread_cond_t q_cond1;
extern pthread_cond_t q_cond2;
extern pthread_cond_t q_cond3;
extern pthread_cond_t q_cond4;

extern pthread_cond_t chat_cond1;
extern pthread_cond_t chat_cond2;
extern pthread_cond_t chat_cond3;
extern pthread_cond_t chat_cond4;

extern pthread_cond_t chat_cond2_1; // wait cond per il secondario
extern pthread_cond_t chat_cond2_2;
extern pthread_cond_t chat_cond2_3;
extern pthread_cond_t chat_cond2_4;
 
int mutex_lock(char, char);
int mutex_unlock(char, char);
int cond_wait(char, char );
int cond_broadcast(char, char);
int cond_signal(char, char);

#endif //NEW_MUTEX__H

