#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <Arduino.h>
#include <card.h>
/**
 * @brief The node structure used in Lab 1 for linked-list
 * 
 */

#define EXIT_OK         0
#define EXIT_ERROR     -1

typedef struct node {
    struct node *p_next_node; // a pointer pointing to the next node in the list
    struct node *p_prev_node;
    Card card;             // node data - an int ranged 0000 - 9999
}CardNode;

typedef struct node Node; 

extern Node *p_head;  // pointer to the first node in Linked list


// Function Prototypes
Node* create_node (Card);
int insert_data_at_head (Card);
int insert_data_at_middle (Card, Card);
int insert_data_at_tail (Card);
int find_and_delete_data (Card);
int delete_all_data ();

#endif 
