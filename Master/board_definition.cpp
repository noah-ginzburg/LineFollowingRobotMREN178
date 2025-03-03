#include "board_definition.h"
#include "linked_list.h"

Node *p_head;  // pointer to the first node in Linked list
Node *p_tail;  // pointer to the last node in Linked list
Button button;


LiquidCrystal* InitLCD( int pin_rs, int pin_en, int pin_d4, int pin_d5, 
                        int pin_d6, int pin_d7, int lcd_width, int lcd_height) {                         
  LiquidCrystal *lcd = new LiquidCrystal(pin_rs, pin_en, pin_d4, pin_d5, pin_d6, pin_d7);
  lcd->begin(lcd_width, lcd_height);
  return lcd; 
}

/**
 * @brief Handles 4 digit number inputs by user
 * @return int user entered value
 */ 
void Check() {
    int x = analogRead(A0);
    if (x<60) button = right;
    else if (x>60 && x<200) button = up;
    else if (x>200 && x<400) button = down;
    else if (x>400 && x<600) button = left;
    else if (x<800 && x>=600) button = select;
    else button = none;
    //Serial.println(x);
}

//Print MENU title
int Menu(LiquidCrystal *lcd){
    int menu_option = 1; // 1:Add 2:Show 3:Delete

    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Select Action");
    lcd->setCursor(10,2);
    lcd->print("MENU");
    
    lcd->setCursor(0, 1);
    lcd->print("Insert    ");

    while (1) {
        //Serial.print("In Menu");
        //Serial.println(menu_option);

         Check();
        lcd->setCursor(0, 1);

        if ( button != none ) {
            if ( button == right || button == up) {
                menu_option++;
                if (menu_option > 3) menu_option = 1;
            }
            else if ( button == left || button == down) {
                menu_option--;
                if (menu_option < 1) menu_option = 3;
            }
            else if ( button == select ) {
                delay(250);
                return menu_option;
            }

            // print menu choices on Row 2
            lcd->setCursor(0, 1);
            switch (menu_option) {
                case 1:
                    lcd->print("Insert    ");
                    break;
                case 2:
                    lcd->print("Show      ");
                    break;
                case 3:
                    lcd->print("Delete    ");
                    break;
            } 
            delay(250);
        }
    }
}

/**
 * @brief Handles 4 digit number inputs by user
 * @return int user entered value
 */ 
int NumberHandling(LiquidCrystal *lcd){
    int cursor_pos = 0; // cursor_pos can only be between 0 and 3
    int val = 0;
    int digits[4] = {0};
    
    lcd->cursor();
    lcd->setCursor(0, 1);
    lcd->print("0000            ");

    //Awaiting button response
    while (true) {
        lcd->setCursor(cursor_pos, 1);
         Check();
        
        // Check buttons
        if (button != none) {
            lcd->setCursor(0, 1);
            if (button == up) {
                digits[cursor_pos]++;
                if (digits[cursor_pos] > 9) digits[cursor_pos] = 0; 
            }
            else if (button == down) {    
                digits[cursor_pos]--;
                if (digits[cursor_pos] < 0) digits[cursor_pos] = 9; 
            }
            else if (button == right) {
                cursor_pos += 1;
                if (cursor_pos > 3) cursor_pos = 0;
            }
            else if (button == left) {
                cursor_pos -= 1;
                if (cursor_pos < 0) cursor_pos = 3;
            }
            else if (button == select) {
                delay(250);
                val = digits[0]*1000 + digits[1]*100 + digits[2]*10 + digits[3];
                return val;
            }
    
            //printing logic
            lcd->setCursor(0, 1);
            for (int i=0; i <= 3; i++)
                lcd->print(digits[i]);
            lcd->setCursor(cursor_pos, 1);    
            delay(200);
        }
    }
}

/**
 * @brief Handles insertion menu options
 * @return InsertOption 
 */ 
InsertOption InsertMenu(LiquidCrystal *lcd){
    InsertOption menu_option = ins_tail;
    
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Insert Node   ");
    lcd->setCursor(0, 1);
    lcd->print("Insert @Tail  ");
    
    while (1) {
         Check();
        if (button != none) {
            if (button == right || button == up ) {
                if (menu_option == ins_middle) menu_option = ins_tail;
                else menu_option = (InsertOption)((int)menu_option + 1);
            }
            else if (button == left || button == down) {
                if (menu_option == ins_tail) menu_option = ins_middle;
                else menu_option = (InsertOption)((int)menu_option - 1);
            }
            else if (button == select) {
                delay(250);
                return menu_option;
            }
            
            // printing            
            lcd->setCursor(0, 1);
            switch (menu_option) {
                case ins_tail:
                    lcd->print("Insert @Tail  ");
                    break;
                case ins_middle:
                    lcd->print("Insert @Middle");
                    break;
                case ins_head:
                    lcd->print("Insert @Head  ");
                    break;
            }
            delay(250);
        } 
    } 
}

/**
 * @brief Handles delete menu options
 * @return DeleteOption 
 */ 
DeleteOption DeleteMenu(LiquidCrystal *lcd){
    DeleteOption menu_option = del_one;
    
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Delete Node(s)  ");
    lcd->setCursor(0, 1);
    lcd->print("Delete One      ");
    
    while (1) {
         Check();
        if (button != none) {
            if (button == right || button == up ) {
                if (menu_option == del_all) menu_option = del_one;
                else menu_option = (DeleteOption)((int)menu_option + 1);
            }
            else if (button == left || button == down) {
                if (menu_option == del_one) menu_option = del_all;
                else menu_option = (DeleteOption)((int)menu_option - 1);
            }
            else if (button == select) {
                delay(250);
                return menu_option;
            }
            
            // printing            
            //Serial.print(menu_option);
            lcd->setCursor(0, 1);
            switch (menu_option) {
                case del_one:
                    lcd->print("Delete One      ");
                    break;
                case del_all:
                    lcd->print("Delete All      ");
                    break;
            }
            delay(250);
        } 
    } 
}

/**
 * @brief Handles insertion menu action selection handler
 * @return int EXIT_ERR or EXIT_OK
 */ 
int InsertHandle(LiquidCrystal *lcd){
    int output;
    InsertOption menu_option = InsertMenu(lcd);

    switch (menu_option) {
        case ins_tail:
            lcd->setCursor(0, 0);
            lcd->print("New Node Value  ");
            output = insert_data_at_tail( NumberHandling(lcd) );
            lcd->setCursor(0, 0);
            if(output == EXIT_OK) lcd->print("Ins @Tail Done  ");
            else lcd->print("Insert Failed   ");
            break;
        case ins_head:
            lcd->setCursor(0, 0);
            lcd->print("New Node Value  ");
            output = insert_data_at_head( NumberHandling(lcd) );
            lcd->setCursor(0, 0);
            if(output == EXIT_OK) lcd->print("Ins @Head Done  ");
            else lcd->print("Insert Failed   ");
            break;
        case ins_middle:
            lcd->setCursor(0, 0);
            lcd->print("Search Value    ");
            int search_val = NumberHandling(lcd);
            lcd->setCursor(0, 0);
            lcd->print("New Node Value  ");
            output = insert_data_at_middle( search_val, NumberHandling(lcd) );
            lcd->setCursor(0, 0);
            if(output == EXIT_OK) lcd->print("Ins @Middle Done"); 
            else lcd->print("Insert Failed   ");
            break;
    }
    delay(1000);
    //return output;
    return EXIT_OK;
}

/**
 * @brief Handles deletion menu action selection handler
 * @return int EXIT_ERR or EXIT_OK
 */ 
int DeleteHandle(LiquidCrystal *lcd){
    int output;
    DeleteOption menu_option = DeleteMenu(lcd);
    
    switch (menu_option) {
        case del_one:
            lcd->setCursor(0, 0);
            lcd->print("Del Node Value  ");
            output =  find_and_delete_data( NumberHandling(lcd) );
            lcd->setCursor(0, 0);
            if(output == EXIT_OK) lcd->print("Del Node Done  ");
            else lcd->print("Node Not Found  ");
            break;
        case del_all:
            lcd->setCursor(0, 0);
            lcd->print("Delete All Nodes");
            output = delete_all_data();
            lcd->setCursor(0, 0);
            lcd->print("Delete All Done ");
            break;
    }
    delay(1000);
    //return output;
    return EXIT_OK;
}

/**
 * @brief Handles show menu action
 * @return int EXIT_ERR if empty list or EXIT_OK 
 */ 
int ShowHandle(LiquidCrystal *lcd){ 
    Node *pn = p_head;

    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->noAutoscroll();
    lcd->print("Show List");
    lcd->setCursor(0, 1);
    lcd->print("Press Select");

    // Empty list
    if (p_head == NULL)  {
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("<EMPTY LIST>");
        delay(2000);
        return EXIT_OK;
    }

    while(pn){
         Check();
        if(button == select){
            lcd->clear();    
            lcd->setCursor(3,0);
            ShowListPrinting(pn,lcd);
            pn=pn->p_next_node;
            delay(250);
        }
    }
    return EXIT_OK;
}

/**
 * @brief Prints one item in the linked list
 * @return int EXIT_ERR if empty list or EXIT_OK 
 */ 
int ShowListPrinting(Node* ptemp, LiquidCrystal *lcd){
    int num = ptemp->data_val;
    int digits[4] = {0};
    for (int i=0; i <= 3; i++) {
        digits[i] = num / pow(10,3-i);
        num -= digits[i] * pow(10,3-i);
        lcd->print(digits[i]);
    }
    lcd->print("->");
    if (ptemp->p_next_node == NULL) lcd->print("END");

    return EXIT_OK;
}

void ReturnToMenu(LiquidCrystal *lcd){
    lcd->setCursor(0,1);
    lcd->print("Return to MENU");

    while(true){
         Check();
        if(button == select){
            delay(250);
            break;
        }
    }
}
//---------------------
