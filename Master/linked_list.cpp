/******************************************************************************
FILENAME:     linked_list.cpp 
COURSE:       MREN 178
LAB:          1
STUDENTS:    Noah Ginzburg (20454733), Lucas Dejmek (20491640)
DATE:         

ATTRIBUTIONS: [LIST ANY EXTERNAL CONTIBUTORS/CONTRIBUTIONS HERE] 
******************************************************************************/

////////////////////////////////////////////////////////////////////////////////    
////////////////////////////////////////////////////////////////////////////////    
/* MREN 178 - Code is missing from the implmentation of this linked list.  
 * Fill in the missing code to get the linked list working on the Arduino!  
 */
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "linked_list.h"

// Function to create a new node
Node* create_node (Card card) {
    // Declaration of local node pointer, with mem allocation
    Node *p_new = (Node *) malloc (sizeof (Node));

    if (p_new != NULL) {
        // Now fill with data. Notice the syntax. You can read this as "assign 
        // value in d to data_val field in Node pointed to by p_new".
        p_new->card = card;
        p_new->p_next_node = NULL; 
        p_new->p_prev_node = NULL;
    }
    
    // Get here with pn pointing to a filled-in structure, or with p_new equal
    // to NULL because malloc() failed. In either case, return p_new.
    return p_new;
}

// Function to insert node at front of the list
int insert_data_at_head (Card card){
    Node *p_new = create_node (card);
    // Make sure node creation worked
    if (p_new == NULL) {
        return EXIT_ERROR;
    }

    // New node goes at the head of the list. The current head of the list is 
    // pointed to by p_head, so we make the new node point to the place that 
    // p_head points to and make p_head point to the new node. So the new node 
    // is at the 'beginning' and points to the remainder of the list. Remember 
    // to check to see if new node is the very first node added - if so, we have 
    // to update the p_end pointer as well.

    /*-------------------------insert your code here--------------------------*/
    
    if (p_head == NULL) //list is empty
    {
      p_head = p_new;
    }
    else
    {
      if (p_head->p_prev_node != NULL)
      {
        p_new->p_prev_node = p_head->p_prev_node;
        p_head->p_prev_node->p_next_node = p_new;
      }

       
        p_new->p_next_node = p_head;
        p_head->p_prev_node = p_new;
      
    }
  
    return EXIT_OK;   
}
 
  // Insert new item of data at back of linked list
  int insert_data_at_tail (int val) {
      Node *p_new = create_node (val);
      // Make sure node creation worked
      if (p_new == NULL) {
          return EXIT_ERROR;
      }

      // New node goes at back of list. If the list is currently empty,
      // then both the head and back pointers need to be updated.

      if (p_head == NULL)
      {
        p_head = p_new;
        p_tail = p_new;
      }
      else
      { 
        p_tail->p_next_node = p_new;
        p_tail = p_new;
      }

      /*-------------------------insert your code here--------------------------*/
    
      return EXIT_OK;
  }

int insert_data_at_middle(int search_val, int val) {
    Node *p_temp;      // Pointer to one of the LL nodes
    p_temp = p_head;   // pointer to first node

    Node *p_new = create_node (val);
    // Make sure node creation worked
    if (p_new == NULL) {
        return EXIT_ERROR;
    }
    
    // Loop through all nodes
    while (p_temp != NULL) {
        // Does ptemp point to node with sought value?
        if (p_temp->data_val == search_val) {
            // Yes it does. This is node we want to add a node after.

            /*---------------------insert your code here-----------------------*/
            if (p_head == NULL)
            {
              insert_data_at_head(val);
            }
            else if (p_temp == p_tail)
            {
              insert_data_at_tail(val);
            }
            else
            {
              p_new->p_next_node = p_temp->p_next_node;
              p_temp->p_next_node = p_new;
            }

            return EXIT_OK;
       }
       
       // if we've reached here, node was not found yet - move on to next node
       
       /*-------------------------insert your code here--------------------------*/
      p_temp = p_temp->p_next_node;
    }
    return EXIT_ERROR;
}

// Find node that holds the value val, and remove the node from the list, 
// without breaking the list of course. Returns ITEM_ERROR if the number 
// could not be found, and EXIT_OK if the item was found and deleted.
int find_and_delete_data (int val) {
    Node *p_temp;      // Pointer to one of the LL nodes

    Node *p_prev;

    Node **pp_node;    // pointer to a pointer to a node
    p_temp = p_head;   // pointer to first node
    pp_node = &p_head; // pointer to pointer that points to first node
    
    // Loop through all nodes
    while (p_temp != NULL) {
        // Does ptemp point to node with sought value?
       if (p_temp->data_val == val) {
           // Yes it does. This is node we want to delete from list.
           
           // Remember: it's possible that we're deleting the first, a middle,
           // or the last node in the list. It is also possible we're about to
           // delete the only item in the list. Handle each of these cases. 
            
           /*---------------------insert your code here-----------------------*/
            
            if (p_temp->data_val == p_head->data_val)
            {
              p_head = p_head->p_next_node;
              p_temp->p_next_node = NULL;
            }
            else
            {
                p_prev->p_next_node = p_temp->p_next_node;
                p_temp->p_next_node = NULL;

                if (p_prev->p_next_node == NULL)
                {
                  p_tail = p_prev;
                }

                free(p_temp);
            }        

           return EXIT_OK;
        }
       
        // if we've reached here, node was not found yet - move on to next node
    
        /*------------------------insert your code here-----------------------*/
      p_prev = p_temp;
      p_temp = p_temp->p_next_node;
    }
    // get here only if we searched whole list and found nothing
    return EXIT_ERROR;
}

// We want to delete all nodes in the linked list. Rememeber to take care of both 
// the head and tail pointers. 
int delete_all_data() {
    Node *p_temp;

    // Loop through all nodes
    while (p_head != NULL) {
        /*-----------------------insert your code here-------------------------*/
        p_temp = p_head;
        p_head = p_head->p_next_node;
        free(p_temp);
    }

    return EXIT_OK;
}
