/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#include "CircularBuffer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

Queue *init(Queue *q){
    q = (Queue *)malloc(sizeof(Queue));
    q -> front = -1;
    q -> rear = -1;
    
    return q;
}

bool searchqueue(Queue *q, char *nickname, struct T_user queue[]){	
	if(q -> front == -1)     // empty queue
      	return false; 
	else {
        for (int j = q -> front; j < q -> rear + 1; j ++) {
	       if (strcmp(queue[j].nickname, nickname) == 0) {
	           return true;
		    }		
        }	
    }  

	return false;
}

bool enqueue(Queue *q, struct T_user user, struct T_user queue[]){
    if((q -> rear + 1) % QUEUE_SIZE == q -> front) {      // If circular queue is full
        return false;
    } else {
        if(q -> front == - 1)                             // If circular queue is empty
            q -> front = 0;
        q -> rear = (q -> rear + 1) % QUEUE_SIZE;
        strcpy(queue[q -> rear].nickname, user.nickname);
        queue[q -> rear].room = user.room;
        queue[q -> rear].user_sd = user.user_sd;
        return true;;
    }
}

struct T_user dequeue(Queue *q, struct T_user queue[]){
    struct T_user tmp;

    if(q -> front == -1)
        printf("underflow error\n");
    else{
        if(q -> front == q -> rear){  //if circular queue has only one element
            strcpy(tmp.nickname, queue[q -> front].nickname);
            tmp.room = queue[q -> front].room;
            tmp.user_sd = queue[q -> front].user_sd;
            q -> front = q -> rear = - 1;
            return tmp;
        } else {
            strcpy(tmp.nickname, queue[q -> front].nickname);
            tmp.room = queue[q -> front].room;
            tmp.user_sd = queue[q -> front].user_sd;
            q -> front = (q -> front + 1) % QUEUE_SIZE;
            return tmp;
        }
    }
}

void print(Queue *q, struct T_user *queue){
    int i;
    printf("Queue is: ");
    if(q -> front != -1){
        for(i = q -> front; i != q -> rear; i = (i + 1) % QUEUE_SIZE)
            printf("%s ", queue[i].nickname);
        printf("%s\n", queue[i].nickname);
    } else printf("empty.\n");
}

int getQueueSize(Queue *q){
    if(q -> rear == -1) return 0;                                               // if empty
    else if((q -> rear + 1) % QUEUE_SIZE == q -> front) return QUEUE_SIZE;      // if full
    else if(q -> rear == 0 && q -> front != 0) return q -> front;               // after dequeue
    else if(q -> rear != 0 && q -> front != 0) return q -> rear;
    else return q -> rear + 1;                                                  // other cases
}
