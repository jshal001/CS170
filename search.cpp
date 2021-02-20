#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std; 

struct Node; 
class MinHeap; 
int MisplacedTiles(Node*); 
vector<vector<int>> ManhattanDistance(Node*); 


//the struct that will be used to represent each state/node
struct Node{ 
    //this 2d array will be used to store the state
    vector<vector<int>> state; 
    //cost g(i.e. the cost to arrive at the node)
    int g;
    
    //how far the state is 
    int h; 
    
    //f(n) = h(n) + g(n)
    int f;   

    Node(vector<vector<int>> userPuzzle, int searchMethod){
        state = userPuzzle; 
        
        //depending on the passed in search method, generate the heuristic 
        //Uniform Cost
        if(searchMethod == 1){
            h = 0; 
        }
        //Misplaced tile 
        else if(searchMethod == 2){
            //pass in pointer to this node
            h = MisplacedTiles(this);
        }
        //Manhattan Distance
        else{
            h = 0; 
        }

        //Since this constructor is for the root node, g will be 0
        g = 0;  

        f = g + h; 
    }

    void PrintNode(){

        cout << endl << "The best state to expand with a g(n) = " << this->g << " and h(n) = " << this->h << " is..." << endl; 

        for(auto j: this->state){
            for(auto i: j){
                cout << i << " ";
            }
            cout << endl; 
        }

        cout << "Expanding this node..." << endl; 

    }
}; 


//Struct to use as the deciding comparison between two nodes
//this operator overload idea is taken from : https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
struct CompareNodes{
    bool operator()(Node* const& puzzleA, Node* const& puzzleB){
        //return true if puzzleB's f is less than puzzleA's
        return (puzzleA->f > puzzleB->f);
    }
};


//structure to define the min heap used
class MinHeap{

    private: 
    //the priority queue data structure of Node pointer type
    priority_queue<Node*, vector<Node*>, CompareNodes> minHeap; 

    //keeps track of the visited nodes 
    vector<Node*> visitedNodes;

    //total number of nodes expanded in search
    int numNodes; 

    //max number of nodes in queue at one time
    int maxNodes; 

    public: 

    MinHeap(): numNodes(0), maxNodes(0){}

    int GetTotalNodes(){
        return numNodes; 
    }

    int GetMaxNodes(){
        return maxNodes; 
    }

    //function to push a new node to the heap
    void pushNode(Node* currNode){
        //first check if the node has already been visited, if so return
        for(auto i: visitedNodes){
            //check if the puzzles are the same as well as 
            //if the heuristic of the visited node is le that of the currnode 
            if(i->state == currNode->state && i->f <= currNode->f){
                return; 
            }
        }
        //if the node hasn't been visited, push it to the queue and visited nodes list
        minHeap.push(currNode); 
        visitedNodes.push_back(currNode); 

        //increment total number of nodes expanded
        ++numNodes; 

        //update the max size of queue
        if(minHeap.size() > maxNodes){
            maxNodes = minHeap.size(); 
        }
        
        return; 
    }




    //function to remove and return the front node
    Node* RemoveFront(){
        
        Node* temp = minHeap.top(); 

        minHeap.pop(); 

        return temp; 
    }

    //a check to see if the queue is empty
    bool Empty(){
        return minHeap.empty(); 
    }



};

//Function to expand a node and returns a vector of nodes to be queued
vector<Node*> Expand(Node* currNode){
    //will store the vector of expanded nodes
    vector<Node*> temp;

    //stores the state of the current node that must be expanded
    vector<vector<int>> currState = currNode->state; 

    //TODO: find the blank tile(i.e. value is 0)
}


//TODO: Function for the general purpose search algorithm






//TODO: Function to generate A* misplaced tiles heuristic
//count the number of misplaced tiles
//at any index [i,j] in the 2d vector, the value should = 3i + j + 1. If not, add 1 to the heurestic 
int MisplacedTiles(Node* currNode){

    int hValue = 0; 
    
    int i = 0; 
    int j = 0; 
    for(i = 0; i < currNode->state.size(); ++i){
        for(j = 0; j < currNode->state.at(i).size(); ++j){
            //check the value at each index unless it's the blank tile(i.e. 0)
            if(currNode->state.at(i).at(j)!= ((3*i) + j + 1) && currNode->state.at(i).at(j) != 0){
                ++hValue; 
            }
        }
    }

    return hValue; 
}


//TODO: Function to generate node Manhattan heuristic
vector<vector<int>> ManhattanDistance(Node* currNode){
    //construct the goal state
    vector<vector<int>> goalState = currNode->state;


    int i, j; 

    for(i = 0; i < goalState.size(); ++i){
        for(j = 0; j < goalState.at(i).size(); ++j){
            //assign the correct value to each pair
            goalState.at(i).at(j)= ((3*i) + j + 1);
        }
    }
    //Assign the zero at the end 
    goalState.at(i - 1).at(j - 1) = 0;


    return goalState; 
}




//Function to check if the current node is a goal state




//TODO: function to calculate f






//driver code
int main(){
    
    // int puzzleChoice = 1; 
    // int currNum = 0; 
    
    // cout << "Welcome to Bertie Woosters 8-puzzle solver." << endl; 
    // cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl; 
    // cin >> puzzleChoice;

    // if(puzzleChoice == 1){
    //     //default puzzle is chosen
    // }
    // else{
    //     //have the user enter their puzzle
    // }
    MinHeap* myHeap = new MinHeap(); 
    vector<vector<int>> matrix;

    
    
    matrix.push_back({1,3,2});
    matrix.push_back({4,6,5});
    matrix.push_back({7,8,0});

    Node* node1 = new Node(matrix, 2);
    

    myHeap->pushNode(node1); 
    // myHeap->pushNode(node2); 
    // myHeap->pushNode(node3); 

    // while(!myHeap->Empty()){
    //     myHeap->RemoveFront()->PrintNode(); 
    // }
    
    // cout << endl << myHeap->GetMaxNodes() << " " << myHeap->GetTotalNodes() << endl; 


    delete node1; 

    delete myHeap;



    return 0; 
}