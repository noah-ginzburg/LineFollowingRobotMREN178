#include <stdlib.h>
#include <stdio.h>
#include <hand.h>

//Linked List implementation of the player hand

//Creates a new node for the card
CardNode* createNode(Card* newCard){
    CardNode* new = (CardNode*)malloc(sizeof(CardNode));
    new->card = newCard;
    new->left = NULL;
    new->right = NULL;
    return new;
}

//Initialize head node
CardNode* head = NULL;

//Insert the node in the player hand linked list
//Node is to be added at the end
void insertNode(CardNode* insert){
    //check for errors
    if (insert == NULL) return;

    if (head == NULL){
        head = insert;
        head->right = insert;
        head->left = insert;
        return;
    }

    //previous node
    CardNode* prev = head->left;
    //set prev node right to insert
    prev->right = insert;
    //set insert left to prev
    insert->left = prev;
    //set insert right to head
    insert->right = head;
    //set head left to insert
    head->left = insert;
}

//deletes a node and returns the head pointer
//requires a pointer to the node to be deleted
CardNode* deleteNode(CardNode* node){

    //check for errors
    if (node == NULL) return head;

    //start with head
    //the head is the only element in the linked list
    //return NULL
    
    if (node == head && head->right == head) {
        free(head);
        return NULL;
    }

    //get the nodes prev and next nodes
    CardNode* prev = node->left;
    CardNode* next = node->right;

    //re-link the node
    prev->right = next;
    next->left = prev;
    
    //if the deleted node is the head node, return the next node
    if (node == head){
        free(node);
        return next;
    }

    return head;
}

//empties the hand array
void emptyHand(){
    while (head != NULL){
        head = deleteNode(head);
    }
}