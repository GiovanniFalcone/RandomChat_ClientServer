/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#ifndef NEW_SERVER_AVL_H
#define NEW_SERVER_AVL_H

#define NICKNAME_SIZE 20

#include <pthread.h>


struct T_user{
    pthread_t client_tid;                   /* client's tid */
    char nickname[NICKNAME_SIZE];           /* nickname inserted by user client */
    char room;                              /* chosen room by user client*/
    char state;                             /* 'W' for user who waiting to chat and 'T' for user who talking */
    char nickname_partner[NICKNAME_SIZE];   /* nickname of the last user he chatted with or is chatting with */
    int user_sd;                            /* socket descriptor of current user */
    int stop;                               /* flag used to know if user has write "@stop". 1 if pressed, 0 otherwise */
    int exit;                               /* flag used to know if user has write "@exit". 1 if pressed, 0 otherwise */
};

struct tree{
    struct T_user key;
    int height;                             /*h(x) = 1 + Max(h(x -> sx), h(x -> dx)) */
    int children;                           /* for random node */
    struct tree *left;
    struct tree *right;
};


typedef struct tree Tree;

Tree *insert(Tree *, char [], char, int, char, pthread_t);

Tree *deleteFromAVL(Tree *, char[]);

void preOrder(Tree *);

void printTree(Tree *, int);

Tree *search(Tree *tree, char item[]);

Tree *randomNode(Tree* root);

Tree *insertChildrenCount(Tree *root);

int getElements(Tree* root);

#endif //NEW_SERVER_AVL_H
