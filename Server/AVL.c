/**
 * @author Giovanni Falcone N86/2329
 * @author Nicola Esposito N86/2206
 *
 */

#include "AVL.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define COUNT 10    //for print tree in 2D

/************************ PROTOTYPES ******************************/
static int max(int, int);

static int tree_height(Tree *);

static Tree *rotate_left(Tree *);

static Tree *rotate_right(Tree *);

static Tree *rotate_double_left(Tree *);

static Tree *rotate_double_right(Tree *);

static Tree *balanceLeft(Tree *);

static Tree *balanceRight(Tree *);

static int is_SS(Tree *);

static int is_SD(Tree *);

static Tree *detach_min(Tree *, Tree *);

static Tree *delete_root(Tree *);

static int children(Tree *);

static Tree *randomNodeUtil(Tree* , int);


/******************************************************************/

static int max(int left, int right){
    return (left > right)? left : right;
}

static int tree_height(Tree *tree){
    return (tree != NULL)? tree -> height : -1;
}

static Tree *rotate_left(Tree *tree){
    Tree *new_root = tree -> left;

    tree -> left = new_root -> right;
    new_root -> right = tree;

    tree -> height = 1 + max(tree_height(tree -> left), tree_height(tree -> right));
    new_root -> height = 1 + max(tree_height(tree -> left), tree_height(tree -> right));

    return new_root;
}

static Tree *rotate_right(Tree *tree){
    Tree *new_root = tree -> right;

    tree -> right = new_root -> left;
    new_root -> left = tree;

    tree -> height = 1 + max(tree_height(tree -> left), tree_height(tree -> right));
    new_root -> height = 1 + max(tree_height(tree -> left), tree_height(tree -> right));

    return new_root;
}

static Tree *rotate_double_left(Tree *tree){
    tree -> left = rotate_right(tree -> left);
    return rotate_left(tree);
}

static Tree *rotate_double_right(Tree *tree){
    tree -> right = rotate_left(tree -> right);
    return rotate_right(tree);
}

static int is_SS(Tree *tree){
    return tree_height(tree -> left) > tree_height(tree -> right);
}

static int is_SD(Tree *tree){
    return tree_height(tree -> right) > tree_height(tree -> left);
}

static Tree *balanceLeft(Tree *tree){
    if(tree != NULL){
        if(tree_height(tree -> left) - tree_height(tree -> right) == 2){
            if(is_SS(tree -> left))
                tree = rotate_left(tree);
            else
                tree = rotate_double_left(tree);
        } else {
            tree -> height = 1 + max(tree_height(tree -> left), tree_height(tree -> right));
        }
    }

    return tree;
}

static Tree *balanceRight(Tree *tree){
    if(tree != NULL){
        if(tree_height(tree -> right) - tree_height(tree -> left) == 2){
            if(is_SD(tree -> right))
                tree = rotate_right(tree);
            else
                tree = rotate_double_right(tree);
        } else {
            tree -> height = 1 + max(tree_height(tree -> left), tree_height(tree -> right));
        }
    }

    return tree;
}

/**
 * strcmp(s1,s2) will return
 * - 0 if they are equal,
 * - positive integer if s1 is greater than s2
 * - and negative integer if s1 is lesser than s2.
 * */
Tree *insert(Tree *tree, char key[], char room, int sd, char state, pthread_t tid){
    if(tree == NULL){
        tree = (Tree *)malloc(sizeof(Tree));
        strcpy(tree -> key.nickname, key);
        strcpy(tree -> key.nickname_partner, "");
        tree -> key.room = room;
        tree -> key.user_sd = sd;
        tree -> key.state = state;
        tree -> key.stop = tree -> key.exit = 0;
        tree -> key.client_tid = tid;
        tree -> height = tree -> children = 0;
        tree -> left = tree -> right = NULL;
    } else {
        if(strcmp(tree -> key.nickname, key) > 0){
            tree -> left = insert(tree -> left, key, room, sd, state, tid);
            tree = balanceLeft(tree);
        } else if(strcmp(tree -> key.nickname, key) < 0){
            tree -> right = insert(tree -> right, key, room, sd, state, tid);
            tree = balanceRight(tree);
        }
    }

    return tree;
}

Tree *deleteFromAVL(Tree *tree, char key[]){
    if(tree != NULL){
        if(strcmp(tree -> key.nickname, key) > 0){
            tree -> left = deleteFromAVL(tree -> left, key);
            tree = balanceRight(tree);
        } else if(strcmp(tree -> key.nickname, key) < 0){
            tree -> right = deleteFromAVL(tree -> right, key);
            tree = balanceLeft(tree);
        } else {
            tree = delete_root(tree);
        }
    }

    return tree;
}

static Tree *delete_root(Tree *tree){
    Tree *tmp;

    if(tree != NULL){
        if((tree -> left == NULL) || (tree -> right == NULL)){
            if(tree -> left != NULL)
                tmp = tree -> left;
            else
                tmp = tree -> right;
            free(tree);
            return tmp;
        } else {
            tmp = detach_min(tree -> right, tree);
            strcpy(tree -> key.nickname, tmp -> key.nickname);
            strcpy(tree -> key.nickname_partner, tmp -> key.nickname_partner);
            tree -> key.room = tmp -> key.room;
            tree -> key.user_sd = tmp -> key.user_sd;
            tree -> key.state = tmp -> key.state;
            tree = balanceLeft(tree);
            free(tmp);
        }
    }

    return tree;
}

static Tree *detach_min(Tree *tree, Tree *parent){
    Tree *ret, *new_root;

    if(tree != NULL){
        if(tree -> left != NULL){
            ret = detach_min(tree -> left, tree);
            new_root = balanceRight(tree);
        } else{
            ret = tree;
            new_root = tree -> right;
        }

        if(parent -> left == tree)
            parent -> left = new_root;
        else
            parent -> right = new_root;
    }

    return ret;
}

/*PreOrder view: root, left subtree, right subtree*/
void preOrder(Tree *tree)
{
    if(tree)
    {
        printf("%s \t", tree -> key.nickname);
        preOrder(tree -> left);
        preOrder(tree -> right);
    }
}

void printTree(Tree *tree, int space){
    if(tree == NULL) return;

    space += COUNT;

    printTree(tree -> right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s, %c %d %c, %s\n", tree -> key.nickname, tree -> key.room, tree -> key.user_sd, tree -> key.state,
        tree -> key.nickname_partner);

    // Process left child
    printTree(tree -> left, space);
}

/* Search for a given item */
Tree *search(Tree *tree, char item[])
{
    if(tree != NULL)
    {
        if(strcmp(tree -> key.nickname, item) == 0) return tree;

        else if(strcmp(tree -> key.nickname, item) > 0) tree = search(tree ->left, item);

        else tree = search(tree -> right, item);
    } return tree;
}

// This is used to fill children counts.
int getElements(Tree* root){
    if (!root)
        return 0;

    return getElements(root -> left) + getElements(root -> right) + 1;
}

// Inserts Children count for each node
Tree *insertChildrenCount(Tree *root)
{
    if (root != NULL){
        root -> children = getElements(root) - 1;
        root -> left = insertChildrenCount(root -> left);
        root -> right = insertChildrenCount(root -> right);
    } else return NULL;

    return root;
}

// returns number of children for root
static int children(Tree *root)
{
    if (root == NULL) return 0; 

    return root -> children + 1;
}

// Helper Function to return a random node
static Tree *randomNodeUtil(Tree* root, int count)
{
    if (!root)
        return 0;

    if (count == children(root -> left))
        return root;

    if (count < children(root -> left))
        return randomNodeUtil(root -> left, count);

    return randomNodeUtil(root -> right,count - children(root -> left) - 1);
}

// Returns Random node
Tree *randomNode(Tree* root)
{
    if(root == NULL)
        return 0;

    int count = rand() % (root -> children + 1);
    // for debugging
    // printf("Nodo candidato: %d\n", count);
    return randomNodeUtil(root, count);
}
