
#include "hand.h"

// Linked List implementation of the player hand

// Creates a new node for the card
CardNode *createNode(Card *newCard)
{
    CardNode *new = (CardNode *)malloc(sizeof(CardNode));
    if (new == NULL) // Check malloc success
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    new->card = newCard;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

// Insert the node at the end of the circular doubly linked list
CardNode *insertNode(CardNode *head, CardNode *insert)
{
    if (insert == NULL)
        return head; // If the insert node is NULL, do nothing

    if (head == NULL) // Empty list case
    {
        insert->next = insert;
        insert->prev = insert;
        return insert;
    }

    CardNode *prev = head->prev;
    prev->next = insert;
    insert->prev = prev;
    insert->next = head;
    head->prev = insert;

    return head; // Head remains unchanged
}

// Deletes a node and returns the new head pointer
CardNode *deleteNode(CardNode *head, CardNode *node)
{
    if (node == NULL || head == NULL)
        return head;

    // Single node case
    if (node == head && head->next == head)
    {
        free(head);
        return NULL;
    }

    CardNode *prev = node->prev;
    CardNode *next = node->next;
    prev->next = next;
    next->prev = prev;

    if (node == head) // If head is deleted, move it to next node
        head = next;

    free(node);
    return head;
}

// Search for a node with a given card
CardNode *searchNode(CardNode *head, Card card)
{
    if (head == NULL)
        return NULL;

    CardNode *current = head;
    do
    {
        if (current->card->rank == card.rank && current->card->suit == card.suit)
            return current;
        current = current->next;
    } while (current != head);

    return NULL; // Card not found
}

// Empties the hand (frees all nodes)
CardNode *emptyHand(CardNode *head)
{
    if (head == NULL)
        return NULL;

    CardNode *current = head;
    CardNode *nextNode;

    do
    {
        nextNode = current->next;
        free(current);
        current = nextNode;
    } while (current != head);

    return NULL; // Hand is now empty
}
