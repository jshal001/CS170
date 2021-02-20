#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

using namespace std; 

struct IndexPair{
    int x; 
    int y; 

    IndexPair(int x, int y): x(x), y(y){}
}; 
struct Node; 
class MinHeap; 
int MisplacedTiles(Node*); 
int ManhattanDistance(Node*); 

map<int, IndexPair*> indexMap; 

vector<vector<int>> goal; 



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

    //this will be used for the first node in the queue
    Node(vector<vector<int>> userPuzzle, int searchMethod){
        state = userPuzzle; 

        goal = state; 

        //create indexMap to be used by manhattan function
        //create goal state 
        int i, j; 
        for(i = 0; i < goal.size(); ++i){
            for(j = 0; j < goal.at(i).size(); ++j){
                //assign the correct value to each pair
                goal.at(i).at(j)= ((3*i) + j + 1);
                //populate the map
                indexMap.emplace(((3*i) + j + 1), new IndexPair(i,j)); 
            }
        }
        //Assign the zero at the end 
        goal.at(i - 1).at(j - 1) = 0;

        //Remove the extra 9 key from map
        indexMap.erase(9);
        
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
            cout << "this ------------" << endl; 
            h = ManhattanDistance(this); 
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

//used in a map to store indices of tiles for quick look up during manhattan heuristic calculation 



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

    //stores goal state of the puzzle passed in 
    vector<vector<int>> goalState; 

    //A map with tile values as keys and the [i,j] pair that should correspond to each tile
    //useful for quick lookup when calculating manhattan heuristic 

    public: 

    MinHeap(Node* rootNode): numNodes(1), maxNodes(1){
        //push the node to the heap
        minHeap.push(rootNode); 
    }

    const int GetTotalNodes(){
        return numNodes; 
    }

    const int GetMaxNodes(){
        return maxNodes; 
    }

    const vector<vector<int>>& getGoalState(){
        return goalState; 
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

    //destructor to deallocate all nodes in the heap
    ~MinHeap(){
        while(!minHeap.empty()){
            Node* currNode = minHeap.top();
            minHeap.pop(); 
            delete currNode; 
        }
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


//TODO: Function to generate Manhattan heuristic
int ManhattanDistance(Node* currNode){
    
    vector<vector<int>> tempState = currNode->state; 

    //used to calculate distance of each value from it's correct position 
    int deltaX = 0; 
    int deltaY = 0; 

    int hValue = 0;   
    //loop through every value at each index pair of current node's state, and compare to the index pair at the value 
    //in the indexMap derived from the goal state
    for(int i = 0; i < tempState.size(); ++i){
        for(int j = 0; j < tempState.at(i).size(); ++j){
            //if the value at this position is 0 do nothing
            if(tempState.at(i).at(j) == 0){
                //nothing
            }
            else{
                
                deltaX = abs(indexMap[tempState.at(i).at(j)]->x - i); 
                deltaY = abs(indexMap[tempState.at(i).at(j)]->y - j); 
               
                //add the sum of deltaX and delta
                hValue+= deltaX + deltaY; 
            }
        }
    }


    return hValue; 
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
    vector<vector<int>> matrix;

    
    
    matrix.push_back({2,1,3});
    matrix.push_back({5,4,6});
    matrix.push_back({7,0,8});

    MinHeap* myHeap = new MinHeap(new Node(matrix, 3));
    

     
    // myHeap->pushNode(node2); 
    // myHeap->pushNode(node3); 

    while(!myHeap->Empty()){
        myHeap->RemoveFront()->PrintNode(); 
    }

    // cout << "The goal state is: " << endl; 

    // for(auto i: goal){
    //     for(auto j: i){
    //         cout << j << " ";
    //     }
    //     cout << endl; 
    // }

    // cout << "the map is as follows" << endl; 
    
  
    cout << endl << myHeap->GetMaxNodes() << " " << myHeap->GetTotalNodes() << endl; 



    delete myHeap;



    return 0; 
}