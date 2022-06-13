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
#include <string.h>
#include <signal.h>
#include <time.h>

#include "Mutex.h"

int mutex_lock(char tipomutex, char r)
{
   if (OPZIONE_LOCK=='T') {   // UTILIZZIAMO IL TRYLOCK per evitare situazioni di DEADLOCK
      if (tipomutex=='M')
         switch(r){
            case '1' :  return pthread_mutex_lock(&mutex1);
            case '2' :  return pthread_mutex_lock(&mutex2);
            case '3' :  return pthread_mutex_lock(&mutex3);
            case '4' :  return pthread_mutex_lock(&mutex4);
         }
      else if (tipomutex=='C')
         switch(r) {
            case '1' :  return pthread_mutex_lock(&chat_mutex_1);
            case '2' :  return pthread_mutex_lock(&chat_mutex_2);
            case '3' :  return pthread_mutex_lock(&chat_mutex_3);
            case '4' :  return pthread_mutex_lock(&chat_mutex_4);
         }
      else if (tipomutex=='S')
         switch(r)  {
            case '1' :  return pthread_mutex_lock(&chat_mutex2_1);
            case '2' :  return pthread_mutex_lock(&chat_mutex2_2);
            case '3' :  return pthread_mutex_lock(&chat_mutex2_3);
            case '4' :  return pthread_mutex_lock(&chat_mutex2_4);
         }
      }
}

 int mutex_unlock(char tipomutex, char r)
{
  if (tipomutex=='M') 	
  	switch(r)  {
   	case '1' :  return pthread_mutex_unlock(&mutex1);
   	case '2' :  return pthread_mutex_unlock(&mutex2);
   	case '3' :  return pthread_mutex_unlock(&mutex3);
   	case '4' :  return pthread_mutex_unlock(&mutex4);
   }

  else if (tipomutex=='C') 	
  	switch(r)  {
   	case '1' :  return pthread_mutex_unlock(&chat_mutex_1);
   	case '2' :  return pthread_mutex_unlock(&chat_mutex_2);
   	case '3' :  return pthread_mutex_unlock(&chat_mutex_3);
   	case '4' :  return pthread_mutex_unlock(&chat_mutex_4);
	}
  else if (tipomutex=='S') 
  	switch(r) {
   	case '1' :  return pthread_mutex_unlock(&chat_mutex2_1);
   	case '2' :  return pthread_mutex_unlock(&chat_mutex2_2);
   	case '3' :  return pthread_mutex_unlock(&chat_mutex2_3);
   	case '4' :  return pthread_mutex_unlock(&chat_mutex2_4);
     }
}

int  cond_wait (char tipocond, char r)
{
  if (tipocond=='Q')
        switch(r)  {
        case '1' :  return pthread_cond_wait(&q_cond1, &mutex1);
        case '2' :  return pthread_cond_wait(&q_cond2, &mutex2);
        case '3' :  return pthread_cond_wait(&q_cond3, &mutex3);
        case '4' :  return pthread_cond_wait(&q_cond4, &mutex4);
   }

  else if (tipocond=='C')
        switch(r)  {
        case '1' :  return pthread_cond_wait(&chat_cond1, &chat_mutex_1);
        case '2' :  return pthread_cond_wait(&chat_cond2, &chat_mutex_2);
        case '3' :  return pthread_cond_wait(&chat_cond3, &chat_mutex_3);
        case '4' :  return pthread_cond_wait(&chat_cond4, &chat_mutex_4);
        }
  else if (tipocond=='S')
        switch(r) {
        case '1' :  return pthread_cond_wait(&chat_cond2_1, &chat_mutex2_1);
        case '2' :  return pthread_cond_wait(&chat_cond2_2, &chat_mutex2_2);
        case '3' :  return pthread_cond_wait(&chat_cond2_3, &chat_mutex2_3);
        case '4' :  return pthread_cond_wait(&chat_cond2_4, &chat_mutex2_4);
     }
 }

 int cond_broadcast (char tipocond, char r)
{
  if (tipocond=='Q')
        switch(r)  {
        case '1' :  return pthread_cond_broadcast(&q_cond1);
        case '2' :  return pthread_cond_broadcast(&q_cond2);
        case '3' :  return pthread_cond_broadcast(&q_cond3);
        case '4' :  return pthread_cond_broadcast(&q_cond4);
   }

  else if (tipocond=='C')
        switch(r)  {
        case '1' :  return pthread_cond_broadcast(&chat_cond1);
        case '2' :  return pthread_cond_broadcast(&chat_cond2);
        case '3' :  return pthread_cond_broadcast(&chat_cond3);
        case '4' :  return pthread_cond_broadcast(&chat_cond4);
        }
  else if (tipocond=='S')
        switch(r) {
        case '1' :  return pthread_cond_broadcast(&chat_cond2_1);
        case '2' :  return pthread_cond_broadcast(&chat_cond2_2);
        case '3' :  return pthread_cond_broadcast(&chat_cond2_3);
        case '4' :  return pthread_cond_broadcast(&chat_cond2_4);
     }
 }

 int cond_signal (char tipocond, char r)
{
  if (tipocond=='Q')
        switch(r)  {
        case '1' :  return pthread_cond_signal(&q_cond1);
        case '2' :  return pthread_cond_signal(&q_cond2);
        case '3' :  return pthread_cond_signal(&q_cond3);
        case '4' :  return pthread_cond_signal(&q_cond4);
   }

  else if (tipocond=='C')
        switch(r)  {
        case '1' :  return pthread_cond_signal(&chat_cond1);
        case '2' :  return pthread_cond_signal(&chat_cond2);
        case '3' :  return pthread_cond_signal(&chat_cond3);
        case '4' :  return pthread_cond_signal(&chat_cond4);
        }
  else if (tipocond=='S')
        switch(r) {
        case '1' :  return pthread_cond_signal(&chat_cond2_1);
        case '2' :  return pthread_cond_signal(&chat_cond2_2);
        case '3' :  return pthread_cond_signal(&chat_cond2_3);
        case '4' :  return pthread_cond_signal(&chat_cond2_4);
     }
 }
