/***************************
 ** File:    show.cpp
 ** Project: CMSC 341, proj2, Spring 2023
 ** Author:  William Whatley
 ** Date:    03/08/2023
 ** Section: Marron
 ** Email:   w59@umbc.edu
 **
 ** This file defines the functions laid out in show.h. Every function is initialized here
 **
 ** All functions were created by William Whatley excluding dumpTree and dump, whch was provided in the project
*****************************/

#include "show.h"

//Default constructor
//Sets m_root to nullptr
Show::Show(){
    m_root = nullptr;
}

//Destructor
//Calls upon clear to entirely clear and delete the object
Show::~Show(){
    clear(); //clear called
}

//insert
//adds a new drone into the function
void Show::insert(const Drone& aDrone){
    //if statement checks to ensure that the ID is within the valid range and that the drone ID does not exist already by calling findDrone
    if ((aDrone.getID() >= MINID) && (aDrone.getID() <= MAXID) && !(findDrone(aDrone.getID()))){
        //next if statement checks to ensure the light colors are valid
        if ((aDrone.getType() == RED) || (aDrone.getType() == BLUE) || (aDrone.getType() == GREEN)){
            //if statement checks to ensure that the state is valid
            if ((aDrone.getState() == LIGHTON) || (aDrone.getState() == LIGHTOFF)){
                //if statement firsts check if m_root is a nullptr, and thus an empty object
                if (m_root == nullptr){
                    //m_root is dynamically allocated to be a new object
                    m_root = new Drone(aDrone.getID(), aDrone.getType(), aDrone.getState());
                }
                //else, recursiveInsert is called, and m_root is sent as the starting parameter
                else{
                    m_root = recursiveInsert(aDrone, m_root); //recursiveinsert with m_root sent
                }
            }
        }
    }
}

//clear
//deletes the entire tree
void Show::clear(){
    recursiveClear(m_root);
    m_root = nullptr;
}

//remove
//removes a drone from a tree
void Show::remove(int id){
    //if statement checks to ensure the id is within the tree
    if (findDrone(id)){
        m_root = recursiveRemove(id, m_root); 
    }
}

//dumpTree and dump
//output the tree
void Show::dumpTree() const {dump(m_root);}
void Show::dump(Drone* aDrone) const{
    if (aDrone != nullptr){
        cout << "(";
        dump(aDrone->m_left);//first visit the left child
        cout << aDrone->m_id << ":" << aDrone->m_height;//second visit the node itself
        dump(aDrone->m_right);//third visit the right child
        cout << ")";
    }
}

//listDrones
//list all of the drones within the tree
void Show::listDrones() const {
    //if statement checks to ensure that the tree is not empty
    if (m_root == nullptr){
        cout << "Empty tree" << endl;
    }
    else{
        //else, inorderList is called to list all drones within the tree
        inorderList(m_root);
    }
}

//setState
//finds the ID, and changes the state
bool Show::setState(int id, STATE state){
    //if statement checks to ensure that the state is valid
    if ((state == LIGHTON) || (state == LIGHTOFF)){
        Drone* curr = m_root;
        //while loop goes until curr equals nullptr, meaning the ID isn't in the list
        while (curr != nullptr){
            //if id is less than the current ID, goes to left
            if (id < curr->getID()){
                curr = curr->getLeft();
            }
            //else if id is greater than curr->getID, goes to right
            else if (id > curr->getID()){
                curr = curr->getRight();
            }
            //else, id is equal to curr->getID(), meaning that the state will be changed
            else{
                //if statement checks to ensure that the states are different, or else nothing is changed and false returned
                if (state != curr->getState()){
                    curr->setState(state);
                    return true;
                }
                else{
                    return false;
                }

            }
        }
    }
    return false; //if reaches end of while loop, returns false
}

//removeLightOff
//removes any drones with their lights off
void Show::removeLightOff(){
    //ensures m_root is not a nullptr
    if (m_root != nullptr){
        m_root = lightsOff(m_root); //lightsOff called with m_root
    }
}

//findDrone
//Finds the drone within the tree
bool Show::findDrone(int id) const {
    //if ID is not within the bounds or m_root is a nullptr, then false returned immediately
    if ((id < MINID) || (id > MAXID) || (m_root == nullptr)){
        return false;
    }
    //else, recursiveFind is used
    else{
        Drone* curr = m_root;
        //while loop goes until curr equals nullptr, meaning the ID isn't in the list
        while (curr != nullptr){
            //if id is less than the current ID, goes to left
            if (id < curr->getID()){
                curr = curr->getLeft();
            }
            //else if id is greater than curr->getID, goes to right
            else if (id > curr->getID()){
                curr = curr->getRight();
            }
            //else, id is equal to curr->getID(), meaning that the ID is in the list
            else{
                return true;
            }
        }
        return false; //if reaches end of while loop, returns false
    }
}

//operator=
//assigns an rhs to another, avoids self assignment
const Show & Show::operator=(const Show & rhs){
    if (this != &rhs){
        clear(); //this is cleared

        //if statement checks to see if m_root is a nullptr, and if so leaves this empty
        if (rhs.m_root == nullptr){
            return *this;
        }
        //else, preOrderAssignment called with rhsDrone's m_root
        else{
            m_root = preOrderAssignment(rhs.m_root);
        }
    }
    return *this;
}

//countDrones
//counts the number of drones of a certain color 
int Show::countDrones(LIGHTCOLOR aColor) const{
    //if m_root is equal to nullptr, means that no drones exist of that color due to the list being empty
    if (m_root == nullptr){
        return 0;
    }
    else{
        return postOrderCount(aColor, m_root);
    }
}

//recursiveInsert
//Helper function which exists to insert the drone recursively
Drone* Show::recursiveInsert(const Drone& aDrone, Drone* curr){
   //if statement checks to see if curr is a nullptr, and if so newDrone is returned
   //this is the base case of the recursive function
   if (curr == nullptr){
        //newDrone declared and initialized
        Drone* newDrone = new Drone(aDrone.getID(), aDrone.getType(), aDrone.getState());
        return newDrone; //newDrone returned
    }
    //else if aDrone's Id is less than curr's ID, yet it doesn't exist, it recurses to the left
    else if ((aDrone.getID() < curr->getID())){
        curr->setLeft(recursiveInsert(aDrone, curr->getLeft())); //goes to the left by calling getLeft of curr
    }
    //else, if the aDrone's ID is greater than curr's ID, then it recurses to the right
    else{
        curr->setRight(recursiveInsert(aDrone, curr->getRight())); //goes to the right by calling getRight of curr
    }

    //height for curr is updated, since it found it's place and needs to be set to zero
    updateHeight(curr);

    //int height difference initialized to checkBalance to see what the difference is
    int heightDifference = checkBalance(curr);

    //if the height difference is greater than 1 or less than -1, that means that it is imbalanced
    //it returns the trinode restructure of curr to the recursive function
    if ((heightDifference < -1) || (heightDifference > 1)){
        return trinodeRestructure(curr);
    }
    
    return curr; //curr is returned if no rebalancing is needed
}

//recursiveClear
//uses recursion to completely delete the tree, using postorder traversal
void Show::recursiveClear(Drone* aDrone){
    //if statement checks to ensure that aDrone is not a nullptr
    if (aDrone != nullptr){
        //postorder traversal used to completely delete the tree
        recursiveClear(aDrone->getLeft());
        recursiveClear(aDrone->getRight());
        delete aDrone;
    }
}

//recursiveRemove
//finds a drone within a system then deletes it
Drone* Show::recursiveRemove(int id, Drone* curr){
    //if statement checks to see if the ID is greater, less than, or equal to the curr drone
    //Not checked if equal since checked in remove whether or not it is found using find()
    if (id > curr->getID()){
        curr->setRight(recursiveRemove(id, curr->getRight()));
    }
    else if (id < curr->getID()){
        curr->setLeft(recursiveRemove(id, curr->getLeft()));
    }
    //else statement is when id == curr->getID(), since we know it exists in the tree since find() was used
    else{
        //if statement checks if everything is nullptr
        if ((curr->getLeft() == nullptr) && (curr->getRight() == nullptr)){
            delete curr;
            return nullptr; //nullptr simply returned
        }
        //else statement checks if curr->getLeft is nullptr but getRight isn't
        else if ((curr->getLeft() == nullptr) && (curr->getRight() != nullptr)){
            Drone* temp = curr->getRight(); //temp ptr holds onto curr->getRight
            delete curr;
            return temp; //temp, or curr's right node returned since height will always be zero due to rebalancing
        }
        //else statement checks if curr->getLeft() isn't nullptr but getRight is
        else if ((curr->getLeft() != nullptr) && (curr->getRight() == nullptr)){
            Drone* temp = curr->getLeft(); //temp ptr holds onto curr->getleft()
            delete curr; //curr deleted
            return temp; //temp returned
        }
        //else is for both left and right not being nullptrs
        else{
            //drone pointers tempLeft and tempRight declared
            Drone* tempLeft;
            Drone* tempRight;

            Drone* replacement = curr->getRight(); //replacement is the node which replaces curr

            //while loop goes through and finds the furthest left node from right as the successor
            while (replacement->getLeft() != nullptr){
                replacement = replacement->getLeft();
            }


            //tempLeft and tempRight initialized to hold the subtrees of curr
            tempLeft = curr->getLeft();
            tempRight = curr->getRight();

            delete curr; //curr is deleted

            //curr is set to a new Drone, and the left and right subtrees are placed
            curr = new Drone(replacement->getID(), replacement->getType(), replacement->getState());
            curr->setLeft(tempLeft); //left subtree is tempLeft since no changes occured
            
            //replacement needs to be deleted, so setRight is the right subtree recursed through until replacement is reached and deleted
            //done recursively to check if any height imbalances occur due to deleting replacement
            curr->setRight(recursiveRemove(replacement->getID(), tempRight));

            replacement = nullptr;
        }
    }

    //height for curr is updated, since it needs to be deleted from the tree
    updateHeight(curr);

    //int height difference initialized to checkBalance to see what the difference is
    int heightDifference = checkBalance(curr);

    //if the height difference is greater than 1 or less than -1, that means that it is imbalanced
    //it returns the trinode restructure of curr to the recursive function
    if ((heightDifference < -1) || (heightDifference > 1)){
        return trinodeRestructure(curr);
    }
    
    return curr;
}

//trinodeRestructure
//helper function which restructures the tree based on the trinode restructure theorem
Drone* Show::trinodeRestructure(Drone* zDrone){
    //drone pointers subtree0, subtree1, subtree2, and subtree3 are declared
    Drone* subtree0; //subtree0 is the least subtree
    Drone* subtree1; //subtree1 is the second to least subtree
    Drone* subtree2; //subtree2 is the second to greatest subtree
    Drone* subtree3; //subtree3 is the greatest subtree
    
    Drone* yDrone = tallestChild(zDrone); //yDrone declared and initialized based on y's place in trinode restructure theorem
    Drone* xDrone = tallestChild(yDrone); //tallestChild of yDrone is used as the xDrone within the trinode restructure
    
    
    //drones a, b, c are declared. They are initialized below with the if else statements
    Drone* a;
    Drone* b;
    Drone* c;

    //if statement checks to see which drone has the lowest ID
    //based on inorder traversal, a is assigned to the lowest ID
    if ((xDrone->getID() < yDrone->getID()) && (xDrone->getID() < zDrone->getID())){
        a = xDrone;
    }
    //else if yDrone is less than everything else, it becomes the a
    else if ((yDrone->getID() < xDrone->getID()) && (yDrone->getID() < zDrone->getID())){
        a = yDrone;
    }
    //if none of the others are true, zDrone becomes a
    else{
        a = zDrone;
    }

    //second if statement checks to see which is assigned to b
    //first thing checked is to see what is assigned to a, then lowest ID is checked between the two remaining
    if (xDrone == a){
        //if statement checks to see which of the two are lesser
        if (yDrone->getID() < zDrone->getID()){
            b = yDrone;
        }
        else{
            b = zDrone;
        }
    }
    //else if rules out yDrone if yDrone is equal to a
    else if (yDrone == a){
        //if statement checks to see which of the two are lesser, and assigns b based on that
        if (xDrone->getID() < zDrone->getID()){
            b = xDrone;
        }
        else{
            b = zDrone;
        }
    }
    //else is used when zDrone is equal to a
    else{
        //if statement checks if a or b are lesser and assigns based on that
        if (xDrone->getID() < yDrone->getID()){
            b = xDrone;
        }
        else{
            b = yDrone;
        }
    }

    //if statement checks which c should be initialized to
    if (((zDrone == a) && (yDrone == b)) || ((yDrone == a) && (zDrone == b))){
        c = xDrone; //c is xDrone if zDrone is a or yDrone is b or vise versa
    }
    else if (((zDrone == a) && (xDrone == b)) || ((xDrone == a) && (zDrone == b))){
        c = yDrone; //c is yDrone if a and b are zDrone and/or xDrone
    }
    else{
        c = zDrone; //otherwise if yDrone is a or xDrone is b, or vice versa, then zDrone must be c
    }
    //if statement checks which variation of the trinode restructure it is
    //checks if left-left, right-left, right-right, left-right, and makes the subtrees based on that
    //first statement checks if it is a left-left rotation
    if ((zDrone == a) && (yDrone == b)){
        subtree0 = zDrone->getLeft(); //z's left is the least subtree
        subtree1 = yDrone->getLeft(); //yDrone left is second to least
        subtree2 = xDrone->getLeft(); //xDrone left is second to greatest
        subtree3 = xDrone->getRight(); //xDrone right is the greatest
    }
    //this checks if it is a right-left rotation
    else if ((zDrone == a) && (yDrone == c)){
        subtree0 = zDrone->getLeft(); //z's left is the least subtree
        subtree1 = xDrone->getLeft(); //xDrone left is the second to least subtree
        subtree2 = xDrone->getRight(); //xDrone right second to greatest
        subtree3 = yDrone->getRight(); //yDrone right is the greatest subtree
    }
    //else if checks if it is a right-right rotation
    else if ((zDrone == c) && (yDrone == b)){
        subtree0 = xDrone->getLeft(); //xDrone left is the least child
        subtree1 = xDrone->getRight(); //xDrone left is second to least
        subtree2 = yDrone->getRight(); //yDrone right is second to greatest
        subtree3 = zDrone->getRight(); //z's right is the greatest subtree
    }
    //final else statement is that it is a left-right rotation
    else{
        subtree0 = yDrone->getLeft(); //yDrone left is the least subtree
        subtree1 = xDrone->getLeft(); //xDrone left is second to least
        subtree2 = xDrone->getRight(); //xDrone right is second to greatest
        subtree3 = zDrone->getRight();  //z's right is the greatest subtree
    }

    //b's setLeft is the a drone, and the setRight is c based on trinode theorem
    b->setLeft(a);
    b->setRight(c);

    //a's left subtree is set to subtree 0, and a's right subtree is set to subtree1
    a->setLeft(subtree0);
    a->setRight(subtree1);

    //c's left subtree is set to subtree3, and c's right subtree is subtree3
    c->setLeft(subtree2);
    c->setRight(subtree3);

    //the heights of a, b, and c are updated
    updateHeight(a);
    updateHeight(c);
    updateHeight(b); //b done last since height of a and c done first

    return b; //b is returned since it replaces zDrone
}

//checkBalance
//helper function which returns the int to indicate the difference in heights
int Show::checkBalance(Drone* curr){
    if (curr->getLeft() == nullptr && curr->getRight() == nullptr){
        return 0;
    }
    else if (curr->getLeft() == nullptr && curr->getRight() != nullptr){
        return curr->getRight()->getHeight() + 1;
    }
    else if ((curr->getLeft() != nullptr) && (curr->getRight() == nullptr)){
        return curr->getLeft()->getHeight() + 1;
    }
    else{
        return curr->getLeft()->getHeight() - curr->getRight()->getHeight();
    }
}

//tallestChild
//Compares the height of both of the children, and returns which one has the greater height
Drone* Show::tallestChild(Drone* parentDrone){
    //first two cases check if getLeft and getRight are nullptrs returns the opposite
    //both CANNOT be nullptrs since trinode restructuring would fix issues before getting to that point
    if (parentDrone->getLeft() == nullptr){
        return parentDrone->getRight();
    }
    else if(parentDrone->getRight() == nullptr){
        return parentDrone->getLeft();
    }
    //else statement checks if the left height is less than the right height
    else if(parentDrone->getLeft()->getHeight() < parentDrone->getRight()->getHeight()){
        return parentDrone->getRight();
    }
    //else, if the left height is greater than or equal to the left height, then it returns the left height
    else{
        return parentDrone->getLeft();
    }
}

//updateHeight
//updates the height of the structure
void Show::updateHeight(Drone* aDrone){
    //if statement checks to ensure that aDrone is not a nullptr
    if (aDrone != nullptr){
        int leftHeight = ((aDrone->getLeft() == nullptr) ? -1 : aDrone->getLeft()->getHeight());
        int rightHeight = ((aDrone->getRight() == nullptr) ? -1 : aDrone->getRight()->getHeight());

        //newHeight is one plus the ternary operator depending on which height is greater
        //if leftHeight is greater, that is used, otherwise the rightHeight is used
        int newHeight = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
        aDrone->setHeight(newHeight); //aDrone's new height is set
    }
}

//inorderList
//Helper function which recurses and prints out each drone of the list in order
void Show::inorderList(const Drone* curr) const{
    //if curr is a nullptr, that means that it will seg fault, so if statement checks for that
    if (curr == nullptr){
        return;
    }
    else{
        //follows left, middle, then right principle of inorder traversal
        inorderList(curr->getLeft());
        //node output
        //getStateStr and getTypeStr used since it displays the data associated with the enum
        cout << curr->getID() << ":" << curr->getStateStr() << ":" << curr->getTypeStr() << endl;
        inorderList(curr->getRight());
    }
}

//lightsOff
//Helper function to remove drone with lights off
Drone* Show::lightsOff(Drone* curr){
    //if curr is nullptr, that means that there's nothing to be done
    if (curr == nullptr){
        return nullptr;
    }
    else{
        //postorder traversal used
        curr->setLeft(lightsOff(curr->getLeft()));
        curr->setRight(lightsOff(curr->getRight()));
        //if the state of curr is lightsoff, then the drone is removed
        updateHeight(curr);
        if (curr->getState() == LIGHTOFF){
            return recursiveRemove(curr->getID(), curr);
        }
        else{
            return curr;
        }
    }
}

//preorderAssignment
//Helper function of the assignment operator, assigns a new drone to curr for the lhs tree
Drone* Show::preOrderAssignment(const Drone* rhsDrone){
    //if rhsDrone is nullptr, nothing is done
    if (rhsDrone == nullptr){
        return nullptr;
    }
    else{
        //curr set to newDrone
        Drone* curr= new Drone(rhsDrone->getID(), rhsDrone->getType(), rhsDrone->getState());
        
        //preorderAssignment called with rhsDrone going to the left first then right
        //set as left and right subtrees of curr
        curr->setLeft(preOrderAssignment(rhsDrone->getLeft()));
        curr->setRight(preOrderAssignment(rhsDrone->getRight()));
        
        
        updateHeight(curr); //curr height updated at end to ensure height updated properly
        return curr; //curr is returned
    }
}

//postOrderCount
//helper function which counts the number of drones of a certain type
int Show::postOrderCount(LIGHTCOLOR color, Drone* curr) const{
    if (curr == nullptr){
        return 0;
    }
    else{
        //left count and right count are declared and initialized
        int leftCount = postOrderCount(color, curr->getLeft());
        int rightCount = postOrderCount(color, curr->getRight());
        
        //count declared and initlaized
        int count = leftCount + rightCount;
        
        //if statement checks if the type is equal to the type provided, and adds one to the count based on that
        if (curr->getType() == color){
            return ++count;
        }
        else{
            return count;
        }
    }
}