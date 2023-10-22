/***************************
 ** File:    mytest.cpp
 ** Project: CMSC 341, proj2, Spring 2023
 ** Author:  William Whatley
 ** Date:    3/12/2023
 ** Section: Marron
 ** Email:   w59@umbc.edu
 **
 ** This file is written as a test for show.cpp. Numerous cases are tested to ensure
 ** that the code works properly. Normal cases, edge cases, and error cases are tested, alongside
 ** other tests such as testing the assignment operator, inserting and removing nodes, and removing all drones with lights off
 **
*****************************/

#include "show.h"
#include <random>
#include <cmath> //cmath included for large insertion and deletion

//global constants
const int MAX_SIZE = 90000; //MAX_SIZE is the highest possible number of drones one can have in a tree
const int NORMAL_CASE = 15; //NORMAL_CASE is 15 and represents a perfect 15 tree

//random class taken from driver and used for testing purposes
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
    public:
        void testCondition(bool var); //basic test condition which prints output
        
        //insertion suite
        bool singleEdgeInsert(Show& show);
        bool normalInsert(Show& show);
        bool errorInsert(Show& show);
        bool testLargeBSTInsert(Show& show, int summation);
        bool largeInsert(Show& show, int summation);
        bool insertTimeComplexity(Show& show, int numAdd);

        //removal suite
        bool singleEdgeRemove(Show& show);
        bool normalRemove(Show& show);
        bool errorRemove(Show& show);
        bool testLargeBSTRemove(Show& show, int summation);
        bool largeRemove(Show& show, int summation);
        bool removeTimeComplexity(Show& show, int numAdd);

        //clear tested next, since used in upcoming functions
        bool testClear(Show& show);

        //assignment operator tests
        bool testAssignment(Show& show);
        bool errorAssignment(Show& show);

        //findDrone tests
        bool normalFindDrone(Show& show);
        bool errorFindDrone(Show& show);

        //setState tests
        bool normalSetState(Show& show);
        bool errorSetState(Show& show);

        //testing of other functions
        bool testRemoveLightOff(Show& show);
        bool testCountDrones(Show& show);

        //miscellaneous helper functions
        bool randomFill(Show& show); //helper which helps to fill tests
        
        double insertTimeMeasurement(Show& show, int addDrones); //helper for insertTimeComplexity
        double removeTimeMeasurement(Show& show, int numToBeRemoved); //helper for removeTimeComplexity
        
        int checkHeight(Drone* curr); //helper for inserting and deleting tests, as it checks to ensure the heights are correct
        bool binarySearchTest(bool result, const Drone* curr); //helper for inserting and deleting tests, as it ensures it is a binary tree
        bool changeHeight(Show& show, int id, int newHeight); //changeHeight, helper to ensure checkHeight is accurate
        bool changeID(Show& show, int id, int newID); //changeID, helper to ensure binarySearchTest is accurate
        
        bool removeLightHelper(bool result, const Drone* curr); //helper for testRemoveLightOff
        bool assignmentHelper(bool result, const Drone* lhsPtr, const Drone* rhsPtr); //helper for testAssignment and errorAssignment
};

int main(){
    Tester tester;
    Show* newShow;

    cout << "**********************" << endl;
    cout << "***** BEGIN TEST *****" << endl;
    cout << "**********************" << endl << endl;
    {
        cout << "\n*** TEST BLOCK ONE ***" << endl << endl;
        cout << "This test block will test insertion for an edge case of one node" << endl << endl;
        
        newShow = new Show(); //show initialized

        //first edge case tested 
        cout << "Single Edge Insertion: \n\t";
        bool testResult = tester.singleEdgeInsert(*newShow);
        tester.testCondition(testResult);

        delete newShow;

        newShow = new Show();

        delete newShow;
        cout << "\n***END TEST BLOCK ONE***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK TWO ***" << endl << endl;
        cout << "This test block will test insert for a normal case, which is 15 nodes" << endl << endl;
        
        newShow = new Show(); //show initialized

        //normal case tested 
        cout << "Normal Insertion: \n\t";
        bool testResult = tester.normalInsert(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK TWO***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK THREE ***" << endl << endl;
        cout << "This will test to ensure that error nodes are not inserted" << endl << endl;
        
        newShow = new Show(); //show initialized

        //error case tested 
        cout << "Error Insertion: \n\t";
        bool testResult = tester.errorInsert(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK THREE***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK FOUR ***" << endl << endl;
        cout << "This will test to ensure that insertion retains the BST property" << endl;
        cout << "This is done with three tests, of heights of 3, 5, and 7" << endl << endl;
        
        newShow = new Show(); //show initialized

        //small BST property tested
        cout << "Testing of a height of 3, or 15 drones: \n\t";
        bool testResult = tester.testLargeBSTInsert(*newShow, 3);
        tester.testCondition(testResult);
        delete newShow;
        
        //medium BST property tested
        newShow = new Show();
        cout << "Testing of a height of 6, or 127 drones: \n\t";
        testResult = tester.testLargeBSTInsert(*newShow, 6);
        tester.testCondition(testResult);
        delete newShow;

        //large BST property tested
        newShow = new Show();
        cout << "Testing of a height of 8, or 511 drones: \n\t";
        testResult = tester.testLargeBSTInsert(*newShow, 8);
        tester.testCondition(testResult);
        delete newShow;
        
        cout << "\n***END TEST BLOCK FOUR ***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK FIVE ***" << endl << endl;
        cout << "This will test to ensure that insertion retains the balanced BST property" << endl;
        cout << "This is done with three tests, of heights of 3, 5, and 7" << endl << endl;
        
        newShow = new Show(); //show initialized

        //small height property tested
        cout << "Testing of a height of 3, or 15 drones: \n\t";
        bool testResult = tester.largeInsert(*newShow, 3);
        tester.testCondition(testResult);
        delete newShow;

        //medium height property tested
        newShow = new Show();
        cout << "Testing of a height of 6, or 127 drones: \n\t";
        testResult = tester.largeInsert(*newShow, 6);
        tester.testCondition(testResult);
        delete newShow;

        //large height property tested
        newShow = new Show();
        cout << "Testing of a height of 8, or 511 drones: \n\t";
        testResult = tester.largeInsert(*newShow, 8);
        tester.testCondition(testResult);
        delete newShow;
        
        cout << "\n***END TEST BLOCK FIVE ***" << endl;  
    }
   {
        cout << "\n*** TEST BLOCK SIX ***" << endl << endl;
        cout << "This will test to ensure that insertion has a time complexity of O(log(n))" << endl;
        cout << "This is done with two tests of 1000 and 10000 drones, which are doubled within the test function" << endl << endl;
        
        newShow = new Show(); //show initialized
        
        //medium insertion time tested
        cout << "Testing of inserting 1000 drones: \n\t";
        bool testResult = tester.insertTimeComplexity(*newShow, 1000);
        tester.testCondition(testResult);
        delete newShow;

        //large insertion time tested
        newShow = new Show();
        cout << "Testing of inserting 10000 drones: \n\t";
        testResult = tester.insertTimeComplexity(*newShow, 10000);
        tester.testCondition(testResult);
        delete newShow;
        
        cout << "\n***END TEST BLOCK SIX ***" << endl;
    } 
    {
        cout << "\n*** TEST BLOCK SEVEN ***" << endl << endl;
        cout << "This test block will test removal for a single edge case" << endl << endl;
        
        newShow = new Show(); //show initialized

        //first edge case tested 
        cout << "Single Edge Removal: \n\t";
        bool testResult = tester.singleEdgeRemove(*newShow);
        tester.testCondition(testResult);

        delete newShow;

        newShow = new Show();

        delete newShow;
        cout << "\n***END TEST BLOCK SEVEN***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK EIGHT ***" << endl << endl;
        cout << "This test block will test remove for a normal case, which is 15 nodes" << endl << endl;
        
        newShow = new Show(); //show initialized

        //normal case tested
        cout << "Normal Removal: \n\t";
        bool testResult = tester.normalRemove(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK EIGHT***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK NINE ***" << endl << endl;
        cout << "This will test to ensure that remove does not work for error nodes" << endl << endl;
        
        newShow = new Show(); //show initialized

        //error case tested 
        cout << "Error Removal: \n\t";
        bool testResult = tester.errorRemove(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK NINE***" << endl;
    } 
    {
        cout << "\n*** TEST BLOCK TEN ***" << endl << endl;
        cout << "This will test to ensure that removal retains the BST property" << endl;
        cout << "This is done with three tests, of heights of 3, 5, and 7" << endl << endl;
        
        newShow = new Show(); //show initialized

        //small BST property tested
        cout << "Testing of a height of 3, or 15 drones: \n\t";
        bool testResult = tester.testLargeBSTRemove(*newShow, 3);
        tester.testCondition(testResult);
        delete newShow;
        
        //medium BST property tested
        newShow = new Show(); 
        cout << "Testing of a height of 6, or 127 drones: \n\t";
        testResult = tester.testLargeBSTRemove(*newShow, 6);
        tester.testCondition(testResult);
        delete newShow;

        //large BST property tested
        newShow = new Show();
        cout << "Testing of a height of 8, or 511 drones: \n\t";
        testResult = tester.testLargeBSTRemove(*newShow, 8);
        tester.testCondition(testResult);
        delete newShow;
        
        cout << "\n***END TEST BLOCK TEN ***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK ELEVEN ***" << endl << endl;
        cout << "This will test to ensure that removal retains the balanced height property" << endl;
        cout << "This is done with three tests, of heights of 3, 5, and 7" << endl << endl;
        
        newShow = new Show(); //show initialized

        //small height property tested
        cout << "Testing of a height of 3, or 15 drones: \n\t";
        bool testResult = tester.largeRemove(*newShow, 3);
        tester.testCondition(testResult);
        delete newShow;
        
        //medium height property tested
        newShow = new Show();
        cout << "Testing of a height of 6, or 127 drones: \n\t";
        testResult = tester.largeRemove(*newShow, 6);
        tester.testCondition(testResult);
        delete newShow;

        //large height property tested
        newShow = new Show();
        cout << "Testing of a height of 8, or 511 drones: \n\t";
        testResult = tester.largeRemove(*newShow, 8);
        tester.testCondition(testResult);
        delete newShow;
        
        cout << "\n***END TEST BLOCK ELEVEN ***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK TWELVE ***" << endl << endl;
        cout << "This will test to ensure that removal has a time complexity of O(log(n))" << endl;
        cout << "This is done with two tests of 1000 and 10000 drones, which are doubled within the test function" << endl << endl;
        
        newShow = new Show(); //show initialized
        
        //medium removal time tested
        cout << "Testing of comparing removing 500 and 1000 drones: \n\t";
        bool testResult = tester.removeTimeComplexity(*newShow, 1000);
        tester.testCondition(testResult);
        delete newShow;

        //large removal time tested
        newShow = new Show();
        cout << "Testing of comparing removing 5000 and 10000 drones: \n\t";
        testResult = tester.removeTimeComplexity(*newShow, 10000);
        tester.testCondition(testResult);
        delete newShow;
        
        cout << "\n***END TEST BLOCK TWELVE***" << endl;
    }
    {
        cout << "\n*** TEST BLOCK THIRTEEN ***" << endl << endl;
        cout << "This will test to ensure that clear works properly" << endl << endl;
        
        newShow = new Show(); //show initialized

        //clear() function tested
        cout << "Clear function tested: \n\t";
        bool testResult = tester.testClear(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK THIRTEEN***" << endl;      
    }
    {
        cout << "\n*** TEST BLOCK FOURTEEN ***" << endl << endl;
        cout << "This will test to ensure that the assignment operator works properly" << endl << endl;
        
        newShow = new Show(); //show initialized

        //normal assignment tested 
        cout << "Assignment Operator: \n\t";
        bool testResult = tester.testAssignment(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK FOURTEEN***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK FIFTEEN ***" << endl << endl;
        cout << "This will test to ensure that the assignment operator works properly in error cases" << endl << endl;
        newShow = new Show();

        //error case tested 
        cout << "Error Assignment Operator: \n\t";
        bool testResult = tester.errorAssignment(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK FIFTEEN***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK SIXTEEN ***" << endl << endl;
        cout << "This will test to ensure that findDrone works for normal cases" << endl << endl;
        
        newShow = new Show(); //show initialized

        //normal case tested 
        cout << "Normal findDrone: \n\t";
        bool testResult = tester.normalFindDrone(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK SIXTEEN***" << endl; 
    }
    {
        cout << "\n*** TEST BLOCK SEVENTEEN ***" << endl << endl;
        cout << "This will test to ensure that findDrone can detect error cases" << endl << endl;
        
        newShow = new Show(); //show initialized

        //error cases tested 
        cout << "Error findDrone: \n\t";
        bool testResult = tester.errorFindDrone(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK SEVENTEEN***" << endl;        
    } 
    {
        cout << "\n*** TEST BLOCK EIGHTEEN ***" << endl << endl;
        cout << "This will test to ensure that setState works for normal cases" << endl << endl;
        
        newShow = new Show(); //show initialized

        //normal case tested 
        cout << "Normal setState: \n\t";
        bool testResult = tester.normalSetState(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK EIGHTEEN***" << endl;         
    }
    {
        cout << "\n*** TEST BLOCK NINETEEN ***" << endl << endl;
        cout << "This will test to ensure that setState can detect error cases" << endl << endl;
        
        newShow = new Show(); //show initialized

        //error cases tested 
        cout << "Error setState: \n\t";
        bool testResult = tester.errorSetState(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK NINETEEN***" << endl;    
    }
    {
        cout << "\n*** TEST BLOCK TWENTY ***" << endl << endl;
        cout << "This will test to ensure that removeLightsOff works properly" << endl << endl;
        
        newShow = new Show(); //show initialized

        //removeLightOff tested 
        cout << "testRemoveLightOff: \n\t";
        bool testResult = tester.testRemoveLightOff(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK TWENTY***" << endl;        
    }
    {
        cout << "\n*** TEST BLOCK TWENTY ONE ***" << endl << endl;
        cout << "This will test to ensure that countDrones work properly" << endl << endl;
        
        newShow = new Show(); //show initialized

        //countDrones tested 
        cout << "Testing countDrones: \n\t";
        bool testResult = tester.testCountDrones(*newShow);
        tester.testCondition(testResult);

        delete newShow;
        cout << "\n***END TEST BLOCK TWENTY ONE ***" << endl;
    } 
    cout << "\n**********************" << endl;
    cout << "***** END TEST *******" << endl;
    cout << "**********************" << endl;

    return 0; //0 is returned
}

//testCondition
//displays the output of the test whether it passed or failed
void Tester::testCondition(bool var){
    if (var){
        cout << "This test has passed" << endl << endl;
    }
    else{
        cout << "This test has failed" << endl << endl;
    }
}

//insertion

//edgeInsertion
//inserts for a single edge case
bool Tester::singleEdgeInsert(Show& show){
    bool result = true;

    result = result && (show.m_root == nullptr); //ensures tree is empty
    Drone myDrone(MINID); //myDrone declared and initialized
    show.insert(myDrone); //myDrone is inserted
    result = result && (show.m_root != nullptr); //checks if the tree is no longer empty
    result = result && show.findDrone(MINID); //result checks if the drone is inserted by seeing if it is found
    result = result && (show.m_root->getHeight() == 0); //checks to ensure that the height of m_root is 0

    return result;

}

//normalInsert
//inserts 15 drones and sees if it passes
bool Tester::normalInsert(Show& show){
    bool result = true;
    Drone* myDrone; //myDrone declared

    //intentionally starts with a required right rotation
    myDrone = new Drone(MAXID);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(50000);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(MINID);
    show.insert(*myDrone);
    delete myDrone;

    //checks to see if trinode restructure occurs by checking heights
    //will be a right right rotation
    result = result && (show.m_root->getID() == 50000);
    result = result && (checkHeight(show.m_root) == 1);
    
    //two more drones are inserted, for a right-left rotation
    myDrone = new Drone(30000);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(35000);
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that a trinode restructure occured
    //will be a right-left rotation
    result = result && (checkHeight(show.m_root) == 2);
    result = result && (show.m_root->getLeft()->getID() == 30000);

    //two more drones are inserted
    //done for a left-right rotation
    myDrone = new Drone(55000);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(60000);
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that trinode restructure occured
    //will be a left-right rotation
    result = result && (checkHeight(show.m_root) == 2);
    result = result && (show.m_root->getRight()->getID() == 60000);

    //two more drones are inserted
    //checked to ensure that a right-right rotation will occur
    myDrone = new Drone(40000);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(45000);
    show.insert(*myDrone);
    delete myDrone;    

    //checks to ensure that trinode restructure occured
    //will be a right-right rotation
    result = result && (checkHeight(show.m_root) == 3);
    result = result && (show.m_root->getLeft()->getRight()->getID() == 40000);
    
    //two more drones are inserted
    //will cause a right-right rotation
    myDrone = new Drone(20000);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(25000);
    show.insert(*myDrone);
    delete myDrone;        

    //trinode restructure should be called
    //checks to ensure that the trinode restructure is called
    result = result && (checkHeight(show.m_root) == 3);
    result = result && (show.m_root->getLeft()->getLeft()->getID() == 20000);
    
    //two more drones inserted
    //will cause a left-left rotation
    myDrone = new Drone(56000);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(58000);
    show.insert(*myDrone);
    delete myDrone;      

    //checks to ensure that the trinode restructure is called
    result = result && (checkHeight(show.m_root) == 3);
    result = result && (show.m_root->getRight()->getLeft()->getID() == 56000);

    //two more drones inserted
    //will cause a left-right rotation    
    myDrone = new Drone(75000);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(80000);
    show.insert(*myDrone);
    delete myDrone;        

    //result checks to ensure that the trinode restructure occured
    result = result && (checkHeight(show.m_root) == 3);
    result = result && (show.m_root->getRight()->getRight()->getID() == 80000);
    
    //final checks to ensure that the tree is perfectly balanced
    result = result && (show.m_root->getID() == 50000);

    //checks to ensure the heights are two of the children
    result = result && (show.m_root->getLeft()->getHeight() == 2);
    result = result && (show.m_root->getRight()->getHeight() == 2);

    //checks to ensure that each grandchild's height is 1
    result = result && (show.m_root->getLeft()->getLeft()->getHeight() == 1);
    result = result && (show.m_root->getLeft()->getRight()->getHeight() == 1);
    result = result && (show.m_root->getRight()->getLeft()->getHeight() == 1);
    result = result && (show.m_root->getRight()->getRight()->getHeight() == 1);
    
    return result;
}

//errorInsert
//tests to ensure errored drones cannot be added to the project
bool Tester::errorInsert(Show& show){
    bool result = true;
    Drone* myDrone;

    //one drone inserted normally
    result = result && (show.m_root == nullptr); //checks to ensure the tree is empty
    myDrone = new Drone(MINID);
    show.insert(*myDrone);
    delete myDrone;

    //attempts to add another drone to the show with the same ID
    myDrone = new Drone(MINID);
    show.insert(*myDrone);
    delete myDrone;

    //attempts to add drone with an invalid light color
    myDrone = new Drone(50000,static_cast<LIGHTCOLOR>(7));
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that each child of m_root is still empty    
    result = result && (show.m_root->getLeft() == nullptr);
    result = result && (show.m_root->getRight() == nullptr);

    //attempts to add drone with invalid light colors
    myDrone = new Drone(25000,static_cast<LIGHTCOLOR>(7));
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(50000,static_cast<LIGHTCOLOR>(3));
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(75000,static_cast<LIGHTCOLOR>(-1));
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that each child of m_root is still empty    
    result = result && (show.m_root->getLeft() == nullptr);
    result = result && (show.m_root->getRight() == nullptr);

    //attempts to add drones with invalid states
    myDrone = new Drone(30000,RED, static_cast<STATE>(2));
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(35000, RED, static_cast<STATE>(100));
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(40000, BLUE, static_cast<STATE>(-1));
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that each child of m_root is still empty    
    result = result && (show.m_root->getLeft() == nullptr);
    result = result && (show.m_root->getRight() == nullptr);

    //the only drone is then removed to make the tree empty
    show.remove(MINID);

    //attempts to add two drones with invalid IDs which are outside the range
    myDrone = new Drone(9999);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(100000);
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that show is empty
    result = result && (show.m_root == nullptr);

    //attempts to add a drone with a negative ID
    myDrone = new Drone(-MINID);
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that show is empty
    result = result && (show.m_root == nullptr);

    //attempts to add two completely errored drones
    myDrone = new Drone(-30000,static_cast<LIGHTCOLOR>(3), static_cast<STATE>(2));
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(35000, static_cast<LIGHTCOLOR>(3), static_cast<STATE>(2));
    show.insert(*myDrone);
    delete myDrone;

    //checks to ensure that show is empty
    result = result && (show.m_root == nullptr);

    return result;
}

//testLargeBSTInsert
//ensures that the BST property holds for larger trees
bool Tester::testLargeBSTInsert(Show& show, int summation){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    int numAdd = 0;

    //for loop determines how many drones need to be added using summation
    //done because this will create a perfect tree, so we can test the BST property directly
    for (int i = 0; i <= summation; i++){
        numAdd += pow(2, i);
    }

    //drones are added based on numAdd
    for (int i = 0; i < numAdd; i++){
        Drone drone((i + 20000), RED);
        show.insert(drone);
    }

    //checks to ensure all drones added are the same by counting by color
    result = result && (show.countDrones(RED) == numAdd);

    //summation corresponds to height, so we check to ensure that each drone is that height
    //summation is what 2^k is, meaning that the height property should hold and that for the tree to be balanced, it must hit this property
    result = result && (checkHeight(show.m_root) == summation);

    //one more drone is added, to prove that the height will change
    //maximum possible drone, so the height of each will change
    Drone drone(MAXID);
    show.insert(MAXID);

    //binarySearchTest
    //ensures that the binary search property holds
    result = result && binarySearchTest(result, show.m_root);

    //in order to prove binarySearchTest is accurate, the max drone is changed such that it is the minimum ID, and the property does not hold
    result = result && changeID(show, MAXID, MINID);
    result = result && !(binarySearchTest(result, show.m_root));

    return result;
}

//largeInsert
//tests insertion for large cases
bool Tester::largeInsert(Show& show, int summation){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    int numAdd = 0;

    //for loop determines how many drones need to be added using summation
    //done because this will create a perfect tree, so we can test the BST property directly
    for (int i = 0; i <= summation; i++){
        numAdd += pow(2, i);
    }

    //drones are added based on numAdd
    for (int i = 0; i < numAdd; i++){
        Drone drone((i + 20000), RED);
        show.insert(drone);
    }

    //checks to ensure all drones added are the same by counting by color
    result = result && (show.countDrones(RED) == numAdd);

    //summation corresponds to height, so we check to ensure that each drone is that height
    //summation is what 2^k is, meaning that the height property should hold and that for the tree to be balanced, it must hit this property
    result = result && (checkHeight(show.m_root) == summation);

    //one more drone is added, to prove that the height will change
    //maximum possible drone, so the height of each will change
    Drone drone(MAXID);
    show.insert(MAXID);

    //height of m_root should increase by one
    //height of m_root's right child should increase by one
    //height of m_left, m_root->m_right->m_left should remain the same, or (summation -1) and (summation - 2) respectively
    result = result && (checkHeight(show.m_root) == (summation + 1));
    result = result && (checkHeight(show.m_root->getRight()) == (summation));
    result = result && (checkHeight(show.m_root->getLeft()) == (summation - 1));
    result = result && (checkHeight(show.m_root->getRight()->getRight()) == (summation - 1));
    result = result && (checkHeight(show.m_root->getRight()->getLeft()) == (summation - 2));

    //done to prove checkHeight works by setting the minimum node's height to 1
    result = result && changeHeight(show, 20000, 7);
    result = result && (checkHeight(show.m_root) != (summation + 1));

    return result;
}

//insertTimeComplexity
//measures the time complexity of inserting into a tree
bool Tester::insertTimeComplexity(Show& show, int numAdd){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //the number of drones are then added, and the time is then saved
    double firstTime = insertTimeMeasurement(show, numAdd);

    //show is then cleared
    show.clear();

    //numAdd is then doubled
    numAdd *= 2;

    //it is then done a second time to test the time complexity, with double the nodes added
    double secondTime = insertTimeMeasurement(show, numAdd);

    //the expected value is 1.1, since T2/2T1 in the algorithm is expected
    //however, due to running times, it may be different, so we have a standard error of 0.4
    double timeDifference = secondTime / (2* firstTime);
    //result then checks to ensure it is within the allocated range
    result = result && ((timeDifference > 0.7) && (timeDifference < 1.5));

    return result;
}

//removal

//singleEdgeRemove
//tests remove for a single edge case
bool Tester::singleEdgeRemove(Show& show){
    bool result = true;

    result = result && (show.m_root == nullptr); //checks to ensure that it is empty

    //result has a drone inserted by calling the singleEdgeInsert test
    result = result && singleEdgeInsert(show);

    //checks to ensure that the drone we're checking for is there
    result = result && show.findDrone(MINID);

    //the drone is then removed
    show.remove(MINID);

    //show is then checked to ensure that it's empty and that find does not work
    result = result && !(show.findDrone(MINID));
    result = result && (show.m_root == nullptr);

    return result;
}

//normalRemove
//removes in a normal case
bool Tester::normalRemove(Show& show){
    bool result = true;

    //ensures that the tree is empty
    result = result && (show.m_root == nullptr);

    //result is equal to the insert test so that show is filled
    result = result && (normalInsert(show));

    //each drone is removed in a specific way to cause an imbalance
    //each drone is removed and checked to ensure no longer in list
    //this will cause a right-left rotation to be required
    show.remove(55000);
    result = result && !(show.findDrone(55000));
    show.remove(58000);
    result = result && !(show.findDrone(58000));
    show.remove(56000);
    result = result && !(show.findDrone(56000));
    

    //trinode restructuring done, checks to ensure that the height of m_root->right is 2 and ID changes
    result = result && (checkHeight(show.m_root->getRight()) == 2);
    //id is now 75000 since imbalance occurs on 60,000's right side
    result = result && (show.m_root->getRight()->getID() == 75000);
    
    //each drone is removed in a specific way to cause an imbalance
    //each drone is removed and checked to ensure no longer in list
    //this will cause a left-left rotation to be required
    show.remove(40000);
    result = result && !(show.findDrone(40000));
    show.remove(35000);
    result = result && !(show.findDrone(35000));
    show.remove(45000);
    result = result && !(show.findDrone(45000));

    //trinode restructuring done, checks to ensure that the height of m_root->left is 2 and ID changes
    result = result && (checkHeight(show.m_root->getLeft()) == 2);
    //id is now 20000 since imbalance occurs on 30000's left side
    result = result && (show.m_root->getLeft()->getID() == 20000);
    
    //m_root is then removed, and then it is tested to see if 60000 replaces it and if trinode occurs
    show.remove(show.m_root->getID());
    result = result && !(show.findDrone(50000));

    //60000 should slide up and take over as m_root, and a right-right rotation will occur
    //this will cause 80000 to become the right child of m_root
    result = result && (show.m_root->getID() == 60000); //60000 becomes new m_root
    result = result && (show.m_root->getRight()->getID() == 80000);

    //10000 is then removed to show a left-right rotation
    show.remove(MINID);
    result = result && !(show.findDrone(MINID));

    //m_root left should become 25000, and the height of m_root should become 2
    //the height of m_root left and m_root right should be 1 each, and the height of their children should be zero
    result = result && (show.m_root->getLeft()->getID() == 25000);
    result = result && (checkHeight(show.m_root) == 2);

    //checks to ensure the heights are one of the children
    result = result && (show.m_root->getLeft()->getHeight() == 1);
    result = result && (show.m_root->getRight()->getHeight() == 1);

    //checks to ensure that each grandchild's height is 0
    result = result && (show.m_root->getLeft()->getLeft()->getHeight() == 0);
    result = result && (show.m_root->getLeft()->getRight()->getHeight() == 0);
    result = result && (show.m_root->getRight()->getLeft()->getHeight() == 0);
    result = result && (show.m_root->getRight()->getRight()->getHeight() == 0);

    return result;
}

//errorRemove
//Attempts to remove errored drones
bool Tester::errorRemove(Show& show){
    bool result = true;
    Drone* myDrone;

    //checks to ensure the tree is empty
    result = result && (show.m_root == nullptr);

    //two proper nodes are inserted
    myDrone = new Drone(MINID);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(MAXID);
    show.insert(*myDrone);
    delete myDrone;

    //invalid nodes are tested to ensure not removed
    show.remove(50000);
    show.remove(65000);
    show.remove(99998);
    show.remove(10001);
    
    //result checks to ensure that nothing changes
    result = result && (show.m_root->getID() == MINID);
    result = result && (show.m_root->getRight()->getID() == MAXID);

    //both nodes are then removed twice, and checked to ensure nothing is negatively altered
    //for loop since done twice
    for(int i = 0; i < 2; i++){   
        show.remove(MAXID);
        result = result && (show.m_root->getRight() == nullptr);
        result = result && (show.m_root->getHeight() == 0);
        result = result && !(show.findDrone(MAXID));
        result = result && (show.m_root->getID() == MINID); 
    }

    //same is then done for MINID
    for(int i = 0; i < 2; i++){   
        show.remove(MINID);
        result = result && (show.m_root == nullptr);
        result = result && !(show.findDrone(MINID));
    }

    return result; //result is then returned
}

//testLargeBSTRemove
//tests to ensure that the BST property is retained for large trees
bool Tester::testLargeBSTRemove(Show& show, int summation){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    int numAdd = 0;

    //for loop determines how many drones need to be added using summation
    //done because this will create a perfect tree, so we can test the BST property directly
    for (int i = 0; i <= summation; i++){
        numAdd += pow(2, i);
    }

    //drones are added based on numAdd
    for (int i = 0; i < numAdd; i++){
        Drone drone((i + 20000), RED);
        show.insert(drone);
    }

    //checks to ensure all drones added are the same by counting by color
    result = result && (show.countDrones(RED) == numAdd);

    //dronesRemoved deletes 2^summation of drones, so the tree remains perfect but heights are one less
    int dronesRemoved = pow(2, summation);
    
    //for loop removes that number of nodes
    for (int i = 0; i < dronesRemoved; i++){
       show.remove(i + 20001); //20001 removed so all nodes in middle
    }
    
    //checks to ensure that the number of drones still equals the summation, just minus the last element
    //since now, the summaton is from 0 to summation - 1
    int newSummation = numAdd - dronesRemoved;
    result = result && (show.countDrones(RED) == (newSummation));

    //ensures that the binary search property holds
    result = result && binarySearchTest(result, show.m_root);

    return result;
}

//largeRemove
//Checks removal for larger cases to prove height balance property remains consistent
bool Tester::largeRemove(Show& show, int summation){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    int numAdd = 0;

    //for loop determines how many drones need to be added using summation
    //done because this will create a perfect tree, so we can test the BST property directly
    for (int i = 0; i <= summation; i++){
        numAdd += pow(2, i);
    }

    //drones are added based on numAdd
    for (int i = 0; i < numAdd; i++){
        Drone drone((i + 20000), RED);
        show.insert(drone);
    }

    //checks to ensure all drones added are the same by counting by color
    result = result && (show.countDrones(RED) == numAdd);

    //dronesRemoved deletes 2^summation of drones, so the tree remains perfect but heights are one less
    int dronesRemoved = pow(2, summation);
    
    //for loop removes that number of nodes
    for (int i = 0; i < dronesRemoved; i++){
       show.remove(i + 20001); //20001 emoved so all nodes in middle
    }

    //checks to ensure that the number of drones still equals the summation, just minus the last element
    //since now, the summaton is from 0 to summation - 1
    int newSummation = numAdd - dronesRemoved;
    result = result && (show.countDrones(RED) == (newSummation));

    //height balance property checked again, all subtrees should have height of one less than summation
    result = result && (checkHeight(show.m_root) == (summation - 1));
    result = result && (checkHeight(show.m_root->getRight()) == (summation - 2));
    result = result && (checkHeight(show.m_root->getLeft()) == (summation - 2));

    //once again performed
    return result;
}

//removeTimeComplexity
//measures the time complexity of removing from a tree
bool Tester::removeTimeComplexity(Show& show, int numAdd){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //for loop inserts that many number of drones with the ID
    for (int i = MINID; i < (MINID + numAdd); i++){
        Drone myDrone(i);
        show.insert(myDrone);
    }

    //half of the number of drones inserted are then removed, and the time is then saved
    double firstTime = removeTimeMeasurement(show, (numAdd/2));

    cout << show.countDrones(RED) << endl;
    //show is then cleared
    show.clear();

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //for loop done again, inserting that many number of drones with the ID
    for (int i = MINID; i < (MINID + numAdd); i++){
        Drone myDrone(i);
        show.insert(myDrone);
    }

    //it is then done a second time to test the time complexity, with double the nodes removed
    double secondTime = removeTimeMeasurement(show, (numAdd + 1));
    
    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //the expected value is 1.1, since T2/2T1 in the algorithm is expected
    //however, due to running times, it may be different, so we have a standard error of 0.4
    double timeDifference = secondTime / (2 * firstTime);

    //result then checks to ensure it is within the allocated range
    result = result && ((timeDifference > 0.7) && (timeDifference < 1.5));

    return result;
}

//assignment and clear

//testClear
//Tests clear to ensure that it works
bool Tester::testClear(Show& show){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //one node inserted, then cleared, then checked to ensure it is empty object
    Drone myDrone(MINID);
    show.insert(myDrone);
    show.clear();
    result = result && !(show.findDrone(MINID));
    result = result && (show.m_root == nullptr);

    //show randomly filled
    result = result && randomFill(show);

    //cleared, checked to ensure to nullptr
    show.clear();
    result = result && (show.m_root == nullptr);
    
    //assigned show declared, and then filled with the nodes within the test for insertion
    Show* assignedShow = new Show();
    result = result && normalInsert(*assignedShow);

    //show then assigned with assigned Show;
    show = *assignedShow;
    delete assignedShow;

    //cleared, checked to ensure to nullptr
    show.clear();
    result = result && (show.m_root == nullptr);
    
    //empty object cleared again to ensure it remains empty and no errors occur
    show.clear();
    result = result && (show.m_root == nullptr);

    return result;
}

//testAssignment
//tests the assignment operator 
bool Tester::testAssignment(Show& show){
    bool result = true;
    Drone* myDrone;
    Show* assignedShow;
    Show* comparisonShow;

    //checks to ensure that show is empty
    result = result && (show.m_root == nullptr);

    //two nodes inserted within show, neither of which will be in the ID
    myDrone = new Drone(71111);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(81111);
    show.insert(*myDrone);

    //assigned show declared, and then filled with the nodes within the test for insertion
    assignedShow = new Show();
    result = result && normalInsert(*assignedShow);

    //assignment performed
    show = *assignedShow;

    //m_root checked, ensure in different locations but have same value
    result = result && (show.m_root != assignedShow->m_root);
    result = result && (show.m_root->getID() == assignedShow->m_root->getID());
    result = result && assignmentHelper(result, show.m_root, assignedShow->m_root);

    //checks to ensure the two nodes previously in the tree before assignment are gone
    result = result && !(show.findDrone(71111));
    result = result && !(show.findDrone(81111));

    //comparisonShow set up, then assigned to show and compared
    comparisonShow = new Show();
    *comparisonShow = show;
    result = result && assignmentHelper(result, show.m_root, comparisonShow->m_root);

    //assignedShow then set to randomFill
    delete assignedShow; //assignedShow deleted
    assignedShow = new Show();
    result = result && randomFill(*assignedShow);

    //extra drone assigned in the offchance the data for randomFill is exactly equal to the one for insertion
    assignedShow->insert(*myDrone);
    delete myDrone;

    //assignment performed
    show = *assignedShow;

    //m_root checked, ensure in different locations but have same value
    result = result && (show.m_root != assignedShow->m_root);
    result = result && (show.m_root->getID() == assignedShow->m_root->getID());
    result = result && assignmentHelper(result, show.m_root, assignedShow->m_root);
    
    //show compared with comparisonShow to ensure all of the data is the same
    result = result && !(assignmentHelper(result, show.m_root, comparisonShow->m_root));
    
    delete assignedShow; //assignedShow deleted
    delete comparisonShow; //comparisonShow deleted
    return result;
}

//errorAssignment
//Checks both assigning two empty objects to each other and also tests self assignment
bool Tester::errorAssignment(Show& show){
    bool result = true;

    //checks to ensure the tree is empty beforehand
    result = result && (show.m_root == nullptr);

    //assignedShow declared and initialized
    Show* assignedShow = new Show();

    //result fills show
    result = result && randomFill(show);

    *assignedShow = show;

    //self assignment
    show = show;

    //checked to ensure show is not empty, then assignedShow ensures that everything is the same
    result = result && (show.m_root != nullptr);
    //assignmentHelper used to prove all data is the same
    result = result && assignmentHelper(result, show.m_root, assignedShow->m_root);
    
    //assignmentShow deleted, then reininitialized to an empty object
    delete assignedShow;
    assignedShow = new Show();

    //ensured that assignedShow is empty
    result = result && (assignedShow->m_root == nullptr);
    //show set equal to assignedShow
    show = *assignedShow;

    //checks to ensure both are nullptrs
    result = result && (show.m_root == nullptr);
    result = result && (assignedShow->m_root == nullptr);

    //assignedShow set equal to show to ensure both remain empty
    //two empty objects
    *assignedShow = show;

    //checks to ensure both are nullptrs
    result = result && (show.m_root == nullptr);
    result = result && (assignedShow->m_root == nullptr);

    delete assignedShow; //assignedShow deleted

    return result; //result returned
}

//findDrone functions

//normalFindDrone
//tests findDrone in normal cases
bool Tester::normalFindDrone(Show& show){
    bool result = true;
    
    //checked to ensure show is empty
    result = result && (show.m_root == nullptr);

    //show is filled with the drones from normalInsert
    result = result && normalInsert(show);

    //all drones are tested and found
    result = result && (show.findDrone(MINID));
    result = result && (show.findDrone(20000));
    result = result && (show.findDrone(25000));
    result = result && (show.findDrone(30000));
    result = result && (show.findDrone(35000));
    result = result && (show.findDrone(40000));
    result = result && (show.findDrone(45000));
    result = result && (show.findDrone(50000));
    result = result && (show.findDrone(55000));
    result = result && (show.findDrone(56000));
    result = result && (show.findDrone(58000));
    result = result && (show.findDrone(60000));
    result = result && (show.findDrone(75000));
    result = result && (show.findDrone(80000));
    result = result && (show.findDrone(MAXID));

    //newDrone inserted, and then tested to see if ID is found
    Drone myDrone(54000);
    show.insert(myDrone);
    result = result && (show.findDrone(54000));

    //a drone is then removed, and checked to ensure the ID is not found
    show.remove(50000);
    result = result && !(show.findDrone(50000));

    return result;
}

//errorFindDrone
//Checks for error cases of findDrone
bool Tester::errorFindDrone(Show& show){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //a drone is inserted, then removed, then attempted to be found
    Drone* myDrone = new Drone(MINID);
    show.insert(*myDrone);
    show.remove(MINID);
    result = result && !(show.findDrone(MINID));

    //fills show with random objects
    result = result && randomFill(show);

    //attempts to find negative IDs
    result = result && !(show.findDrone(-MINID));
    result = result && !(show.findDrone(-MAXID));
    result = result && !(show.findDrone(-15000));

    //attempts to find drones with IDs outside range
    result = result && !(show.findDrone(9999));
    result = result && !(show.findDrone(100000));

    //attempts to add an invalid drone, then try and find it
    delete myDrone;
    myDrone = new Drone(8000);
    show.insert(*myDrone);
    delete myDrone;

    result = result && !(show.findDrone(8000));

    return result;
}

//state functions

//normalSetState
//Tests setState to ensure it works normally
bool Tester::normalSetState(Show& show){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //a node is inserted, then the state is changed
    Drone* myDrone = new Drone(MINID, RED, LIGHTON);
    show.insert(*myDrone);
    result = result && (show.m_root->getState() == LIGHTON);
    delete myDrone;

    //state of the drone is changed
    result = result && (show.setState(MINID, LIGHTOFF));

    //the drone is then checked to ensure it is lightoff
    result = result && (show.m_root->getState() == LIGHTOFF);

    //three drones are inserted, and all have their state changed
    myDrone = new Drone(25000, RED, LIGHTON);
    show.insert(*myDrone);
    result = result && (show.setState(25000, LIGHTOFF));
    delete myDrone;
    myDrone = new Drone(50000, RED, LIGHTON);
    show.insert(*myDrone);
    result = result && (show.setState(50000, LIGHTOFF));
    delete myDrone;
    myDrone = new Drone(MAXID, RED, LIGHTOFF);
    show.insert(*myDrone);
    result = result && (show.setState(MAXID, LIGHTON));
    delete myDrone;

    //all are tested to ensure state is lightoff
    result = result && (show.m_root->getLeft()->getState() == LIGHTOFF);
    result = result && (show.m_root->getRight()->getState() == LIGHTOFF);
    result = result && (show.m_root->getRight()->getRight()->getState() == LIGHTON);

    //to further verify, removeLightOff called to ensure that MAXID is the only drone left within the structure
    show.removeLightOff();
    result = result && (show.m_root->getID() == MAXID);
    result = result && (show.m_root->getLeft() == nullptr);
    result = result && (show.m_root->getRight() == nullptr);

    return result;
}

//errorSetState
bool Tester::errorSetState(Show& show){
    bool result = true;

    //checks to ensure that the tree is empty
    result = result && (show.m_root == nullptr);

    //result then checks to ensure that false is returned and nothing is changed
    result = result && !(show.setState(MINID, LIGHTOFF));
    result = result && (show.m_root == nullptr);

    //two drones are then inserted, but then given the same state, so false should be returned
    //done so both LIGHTON and LIGHTOFF are tested
    Drone* myDrone = new Drone(MINID, RED, LIGHTOFF);
    show.insert(*myDrone);
    delete myDrone;
    myDrone = new Drone(MAXID, RED, LIGHTON);
    show.insert(*myDrone);
    delete myDrone;

    //results of both are tested
    result = result && !(show.setState(MINID, LIGHTOFF));
    result = result && !(show.setState(MAXID, LIGHTON));

    //states of both are checked to ensure stayed the same
    result = result && (show.m_root->getState() == LIGHTOFF);
    result = result && (show.m_root->getRight()->getState() == LIGHTON);

    //invalid states tested
    result = result && !(show.setState(MINID, static_cast<STATE>(8)));
    result = result && !(show.setState(MAXID, static_cast<STATE>(2)));
    result = result && !(show.setState(MAXID, static_cast<STATE>(-1)));

    //maxID drone is removed, then state is attempted to be changed
    show.remove(MAXID);
    result = result && !(show.setState(MAXID, LIGHTOFF));

    //checks to ensure m_root->right is still empty
    result = result && (show.m_root->getRight() == nullptr);

    return result;
}

//other functions

//testRemoveLightOff
//tests the removeLightOff function
bool Tester::testRemoveLightOff(Show& show){
    bool result = true;
    //checks to ensure show is empty
    Random typeGen(0,2); // there are three colors

    result = result && (show.m_root == nullptr);
    //12047 used since it makes a perfect tree, useful for calculating height
    for (int i = MINID + 1; i < 10127; i++){
        STATE light = ((i % 2 == 0) ? LIGHTON : LIGHTOFF); //drone light state dependent on if modulus 2 == 0
        Drone drone(i, static_cast<LIGHTCOLOR>(typeGen.getRandNum()), light);
        show.insert(drone);
    }
    //height checked
    //the height should be 6, since summation 2^k from 0 to 6 == 127
    result = result && (checkHeight(show.m_root) == 6);

    //based on for loop, half of the nodes are lights on, so size of tree should be cut in half
    show.removeLightOff();

    //for loop checks to ensure that all odd numbers are removed from the tree and all even remain
    for (int i = MINID + 1; i < 10127; i++){
        //if statement checks based on modulus. If true, checks to ensure in the tree still, if not, checks to ensure removed
        if (i % 2 == 0){
            result = result && show.findDrone(i);
        }
        else{
            result = result && !(show.findDrone(i));
        }
    }

    //height checked again, since half of the drones are removed, which will make the max height 9
    //the value is floored
    result = result && (checkHeight(show.m_root) == 5);

    //removeLightHelper called to help check to ensure all lights are at zero
    result = result && removeLightHelper(result, show.m_root);

    //test is done again, this time with fifteen nodes, and ones in middle are lightsoff
    for (int i = 0; i < 6; i++){
        Drone myDrone((i + 10000), RED, LIGHTON);
        show.insert(myDrone);
    }    
    for (int i = 6; i < 9; i++){
        Drone myDrone((i + 10000), RED, LIGHTOFF);
        show.insert(myDrone);
    }
    for (int i = 9; i < 15; i++){
        Drone myDrone((i + 10000), RED, LIGHTON);
        show.insert(myDrone);
    }

    //show removeLightOff called and helper called to test
    show.removeLightOff();
    result = result && removeLightHelper(result, show.m_root);

    //show cleared again, but this time children nodes are all LIGHTSOFF
    show.clear();
    for (int i = 0; i < 6; i++){
        Drone myDrone((i + 10000), RED, LIGHTOFF);
        show.insert(myDrone);
    }    
    for (int i = 6; i < 9; i++){
        Drone myDrone((i + 10000), RED, LIGHTON);
        show.insert(myDrone);
    }
    for (int i = 9; i < 15; i++){
        Drone myDrone((i + 10000), RED, LIGHTOFF);
        show.insert(myDrone);
    }   

    //in order to prove removeLightHelper works, m_root's drone is shifted so light is off
    result = result && show.setState(show.m_root->getID(), LIGHTOFF);
    result = result && !(removeLightHelper(result, show.m_root));

    //new drone added for further proof
    result = result && show.setState(show.m_root->getID(), LIGHTON);
    Drone myDrone(MAXID, RED, LIGHTOFF);
    show.insert(myDrone);
    result = result && !(removeLightHelper(result, show.m_root));

    //show then cleared
    show.clear();

    //fils a tree with LIGHTON drones, then removes them all to prove that it works for edge cases
    for (int i = MINID; i < 10300; i++){
        Drone myDrone(i, RED, LIGHTOFF);
        show.insert(myDrone);
    }

    show.removeLightOff();
    result = result && (show.m_root == nullptr);

    //final case is when the tree is all lights on, so nothing changes
    for (int i = MINID; i < 10300; i++){
        Drone myDrone(i);
        show.insert(myDrone);
    }
    //assignedShow holds onto show before lights off
    Show* assignedShow = new Show();
    *assignedShow = show;

    //remove light called
    show.removeLightOff();

    //assignedShow and show are tested to ensure they are still equal, thus no nodes removed
    result = result && assignmentHelper(result, show.m_root, assignedShow->m_root);

    delete assignedShow;

    return result;
}

//testCountDrones
//tests the function count drones
bool Tester::testCountDrones(Show& show){
    bool result = true;

    //checks to ensure tree is empty
    result = result && (show.m_root == nullptr);

    //checks to ensure countDrones is 0 when empty
    result = result && (show.countDrones(BLUE) == 0);
    result = result && (show.countDrones(RED) == 0);
    result = result && (show.countDrones(GREEN) == 0);

    //fifteen drones are inserted, all with the same color
    //done in for loop
    for (int i = MINID; i < 10015; i++){
        Drone drone(i, BLUE);
        show.insert(drone);
    }

    //result checks to ensure all drones are blue, and that no colors exist
    result = result && (show.countDrones(BLUE) == 15);
    result = result && (show.countDrones(RED) == 0);
    result = result && (show.countDrones(GREEN) == 0);

    //drone object cleared
    show.clear();

    //for loop redone, with a third of each drone being specific colors
    for (int i = MINID; i < 10015; i++){
        //if statement checks the modulus of i - 10000 with 3, since 15 is divisible by 3
        if ((i - 10000) % 3 == 0){
            Drone drone(i, RED);
            show.insert(drone);
        }
        else if ((i - 10000) % 3 == 1){
            Drone drone(i, BLUE);
            show.insert(drone);
        }
        else{
            Drone drone(i, GREEN);
            show.insert(drone);            
        }
    }

    //result then checks to ensure that 5 of each drone exist in the tree
    result = result && (show.countDrones(BLUE) == 5);
    result = result && (show.countDrones(RED) == 5);
    result = result && (show.countDrones(GREEN) == 5);


    //show cleared again
    show.clear();

    //filled with fifteen of RED and BLUE, but will be filled with one of green at the end
    for (int i = MINID; i < 10030; i++){
        //if statement checks if odd or even in order to determine which color gets added
        if (i % 2 == 0){
            Drone drone(i, RED);
            show.insert(drone);
        }
        else{
            Drone drone(i, BLUE);
            show.insert(drone);
        }
    }

    //then a green and blue drone are inserted
    Drone* drone = new Drone(20000, GREEN);
    show.insert(*drone);
    delete drone;
    drone = new Drone(30000, BLUE);
    show.insert(*drone);
    delete drone;

    //checks to ensure all colors are matched with the value
    //this is done to ensure when the number of each color is different, it still works properly
    result = result && (show.countDrones(BLUE) == 16);
    result = result && (show.countDrones(RED) == 15);
    result = result && (show.countDrones(GREEN) == 1);

    return result;
}

//randomFill
//copied from driver, fills show with 15 random elements
bool Tester::randomFill(Show& show){
    bool result = true;
    Random idGen(MINID,MAXID);
    Random typeGen(0,2); // there are three colors
    int ID = 0;
    for(int i = 0; i < NORMAL_CASE; i++){
        ID = idGen.getRandNum();
        Drone drone(ID,static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
        show.insert(drone);
        result = result && (show.findDrone(ID));
    }

    return result; 
}

//time complexity helper functions

//insertTimeMeasurement
//displays the time complexity of insertion
double Tester::insertTimeMeasurement(Show& show, int addDrones){
    double T = 0.0;//stores running times
    clock_t start, stop;//stores the clock ticks while running the program
    Random idGen(MINID,MAXID);
    int ID;

    start = clock();
    // the algorithm to be measured
    //inserts the drones into the show
    for (int i = 0; i < addDrones; i++){
        ID = idGen.getRandNum();
        Drone drone(ID);
        show.insert(drone);
    }
    stop = clock();
    T = stop - start;//number of clock ticks the algorithm took
    double measureTime = T/CLOCKS_PER_SEC;//time in seconds
    return measureTime;
}

//removeTimeMeasurement
//displays the time complexity of removal
double Tester::removeTimeMeasurement(Show& show, int numToBeRemoved){
    double T = 0.0;//stores running times
    clock_t start, stop;//stores the clock ticks while running the program

    start = clock();
    // the algorithm to be measured
    //removes the drones from the show
    for (int i = 0; i < numToBeRemoved; i++){
       show.remove(i + MINID);
    }
    stop = clock();
    T = stop - start;//number of clock ticks the algorithm took
    double measureTime = T/CLOCKS_PER_SEC;//time in seconds
    return measureTime;
}


//insertion and deletion helper functions

//checkHeight
//checks the height of each node, and if it doesn't match, returns -1 to indicate error
int Tester::checkHeight(Drone* curr){
    if (curr == nullptr){
        return -1;
    }
    else{
        //left height and right height are declared and initialized
        int leftHeight = checkHeight(curr->getLeft());
        int rightHeight = checkHeight(curr->getRight());
        
        //height is checked
        int height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);

        //if statement checks to ensure that the height is equal to the actual supposed height
        if (curr->getHeight() == height){
            return height;
        }
        else{
            return 1000; //returns 1000 to indicate an error has occured
        }
    }
}

//binarySearchTest
//Ensures that the tree is still a binary search tree
bool Tester::binarySearchTest(bool result, const Drone* curr){
    if (curr == nullptr){
        return result;
    }
    else{
        //
        result = result && binarySearchTest(result, curr->getLeft());
        result = result && binarySearchTest(result, curr->getRight());
        
        //left and right ID are declared and initialized
        //checks if nullptr, if so returns a value out of range so always tue
        int leftID = ((curr->getLeft() == nullptr) ? 0: curr->getLeft()->getID());
        int rightID = ((curr->getRight() == nullptr) ? 999999: curr->getRight()->getID());

        //if statement curr->getID() is greater than the left ID and less than the right ID
        if ((curr->getID() > leftID) && (curr->getID() < rightID)){
            return result && true;
        }
        else{
            return false; //else, false is returned
        }
    }
}

//changeHeight
//helper function used to change the height to prove checkHeight works
bool Tester::changeHeight(Show& show, int id, int newHeight){
    Drone* curr = show.m_root;
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
        //else, id is equal to curr->getID(), meaning that the height will be changed
        else{
            curr->setHeight(newHeight);
            return true;
        }
    }
    return false; //if reaches end of while loop, returns false
}

//changeID
//Done to change the ID to prove that binarySearchTest is a valid test
bool Tester::changeID(Show& show, int id, int newID){
    Drone* curr = show.m_root;
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
        //else, id is equal to curr->getID(), meaning that the ID will be changed
        else{
            curr->m_id = newID;
            return true;
        }
    }
    return false; //if reaches end of while loop, returns false
}


//direct helper functions

//removeLightHelper
//helps to test that every light within the tree has its lights on
bool Tester::removeLightHelper(bool result, const Drone* curr){
    if (curr == nullptr){
        return result && true;
    }
    else{
        //left height and right height are declared and initialized
        result = result && removeLightHelper(result, curr->getLeft());
        result = result && removeLightHelper(result, curr->getRight());
        
        //if statement checks to ensure ID, state, and type are all the same
        if (curr->getState() == LIGHTON){
            return result && true;
        }
        else{
            return result && false; //else, false is returned
        }
    }
}

//assignmentHelper
//Helps to ensure that both trees are equal to each other data wise, but not memory wise
bool Tester::assignmentHelper(bool result, const Drone* lhsPtr, const Drone* rhsPtr){
    if ((lhsPtr == nullptr) && (rhsPtr == nullptr)){
        return result && true;
    }
    else if ((lhsPtr != nullptr) && (rhsPtr != nullptr)){
        bool result = true;

        //left height and right height are declared and initialized
        result = result && assignmentHelper(result, lhsPtr->getLeft(), rhsPtr->getLeft());
        result = result && assignmentHelper(result, lhsPtr->getRight(), rhsPtr->getRight());
        
        //if statement checks to ensure ID, state, and type are all the same
        if ((rhsPtr->getID() == lhsPtr->getID()) && (lhsPtr->getState() == rhsPtr->getState()) && (lhsPtr->getType() == rhsPtr->getType())){
            //second if statement checks to ensure the ptrs do not equal each other
            if ((lhsPtr != rhsPtr) && (lhsPtr->getHeight() == rhsPtr->getHeight())){
                return result && true;
            }
            else{
                return result && false; //else, false is returned
            }
        }
        else{
            return result && false; //else, false is returned
        }
    }
    else{
        return result && false; //else, result and false is returned
    }  
}