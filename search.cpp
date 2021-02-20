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

    //the search function used
    int searchFunction; 

    //this will be used for the first node in the queue
    Node(vector<vector<int>> &userPuzzle, int searchMethod){
        state = userPuzzle; 

        goal = state; 

        searchFunction = searchMethod; 

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


    Node(Node* parentNode, vector<vector<int>> &childState){
        
        this->state = childState;

        this->searchFunction = parentNode->searchFunction; 
        //the depth increases by one
        this->g = parentNode->g + 1; 
        if(searchFunction == 0){
            this->h = 0; 
        }
        else if(searchFunction == 1){
            this->h = MisplacedTiles(this); 
        }
        else{
            this->h = ManhattanDistance(this); 
        }


        this->f = this->h + this->g; 

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

    //Queueing function 
    void QueueingFunction(vector<Node*> children){
        //queue each node with the helper pushNode function 
        for(int i = 0; i < children.size(); ++i){
            pushNode(children.at(i)); 
        }
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

//swap function returns a state after tiles have been swapped
vector<vector<int>> swapTiles(vector<vector<int>>& parentState, int blankI, int blankJ, int tileI, int tileJ){
    
    vector<vector<int>> tempVector = parentState;

    //swap..
    tempVector.at(blankI).at(blankJ) = tempVector.at(tileI).at(tileJ); 
    tempVector.at(tileI).at(tileJ) = 0; 

    return tempVector;  
}

//Function to expand a node and returns a vector of nodes to be queued
vector<Node*> Expand(Node* parentNode){
    //will store the vector of expanded nodes
    vector<Node*> expandNodes;
    vector<vector<int>> tempState; 

    //stores the state of the current node that must be expanded
    vector<vector<int>> parentState = parentNode->state; 

    //find the blank tile(i.e. value is 0)
    int i, j, x, y;   

    for(i = 0; i < parentState.size(); ++i){
        for(j = 0; j < parentState.at(i).size(); ++j){
            if(parentState.at(i).at(j) == 0){
                x = i; 
                y = j; 
            }
        }
    }

    //determine in which direction the blank can go and create nodes respectively
    //if 0 can be swapped up
    if(x > 0){
        //create a new state after the swap 
        tempState = swapTiles(parentState, x, y, x - 1, y); 
        Node* upSwap = new Node(parentNode, tempState); 
        //push the node to the expandNodes vector
        expandNodes.push_back(upSwap); 
    }
    //if 0 can be swapped down
    if(x < parentState.size() - 1){
        //create new state after swap
        tempState = swapTiles(parentState, x, y, x + 1, y); 
        Node* downSwap = new Node(parentNode, tempState);
        expandNodes.push_back(downSwap);
    }
    //if 0 can be swapped to the left
    if(y > 0){
        tempState = swapTiles(parentState, x, y, x, y - 1);
        Node* leftSwap = new Node(parentNode, tempState);
        expandNodes.push_back(leftSwap);
    }
    //if 0 can be swapped to the right
    if(y < parentState.at(0).size() - 1){
        tempState = swapTiles(parentState, x, y, x, y + 1); 
        Node* rightSwap = new Node(parentNode, tempState); 
        expandNodes.push_back(rightSwap); 
    }

    //return vector of expanded nodes
    return expandNodes; 
}



// Function to generate A* misplaced tiles heuristic
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


//Function to generate Manhattan heuristic
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
bool GoalTest(vector<vector<int>>& testState){
    return testState == goal; 
}


//Function for the general purpose search algorithm
//Passes in initial state and the search method desired
Node* GeneralSearch(vector<vector<int>>& initialState, int searchMethod){
    // cout << "0" << endl; 

    MinHeap* myHeap = new MinHeap(new Node(initialState, searchMethod));

    // cout << "1" << endl; 
    while(!myHeap->Empty()){
        // cout << "2" << endl; 
        Node* node = myHeap->RemoveFront(); 
        // cout << "3" << endl; 
        if(GoalTest(node->state)){
            // cout << "4" << endl; 
            return node; 
        }
        // cout << "5" << endl; 
        myHeap->QueueingFunction(Expand(node)); 
        // cout << "6" << endl; 
    }

    //if it exits the while loop then the queue is empty
    //return failure
    return nullptr; 

}



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

    
    
    matrix.push_back({7,1,2});
    matrix.push_back({4,8,5});
    matrix.push_back({6,3,0});

    GeneralSearch(matrix, 1)->PrintNode(); 


    // MinHeap* myHeap = new MinHeap(new Node(matrix, 3));
    // MinHeap* myHeap = new MinHeap(new Node(matrix, 3));

    

     
    // myHeap->pushNode(node2); 
    // myHeap->pushNode(node3); 



    // cout << "The goal state is: " << endl; 



    // cout << "the map is as follows" << endl; 
    
  
    // cout << endl << myHeap->GetMaxNodes() << " " << myHeap->GetTotalNodes() << endl; 

    // for(auto i: matrix){
    //     for(auto j: i){
    //         cout << j << " ";
    //     }
    //     cout << endl; 
    // }

    // myHeap->QueueingFunction(Expand(myHeap->RemoveFront())); 
    // myHeap->QueueingFunction(Expand(myHeap->RemoveFront())); 

    // while(!myHeap->Empty()){
    //     myHeap->RemoveFront()->PrintNode(); 
    // }






    return 0; 
}