#include <iostream>
#include <vector>

using namespace std; 

//the struct that will be used to represent each state/node
struct Node{ 
    //this 2d array will be used to store the state
    vector<vector<int>> puzzle; 
    //cost g(i.e. the cost to arrive at the node)
    int g = 0; 
    
    //how far the state is 
    int h = 0; 
    
    //f(n) = h(n) + g(n)
    int f = 0;   

    Node(vector<vector<int>> userPuzzle, int h, int g){
        this->puzzle = userPuzzle; 
        this->h = h; 
        this->g = g; 

        this->f = g + h; 
    }

    void PrintNode(){

        cout << "The best state to expand with a g(n) = " << this->g << " and h(n) = " << this->h << " is..." << endl; 

        for(auto j: this->puzzle){
            for(auto i: j){
                cout << i << " ";
            }
            cout << endl; 
        }

        cout << "Expanding this node..." << endl; 

    }
}; 

//TODO: Function for the general purpose search algorithm



//TODO: Function to expand the tree 



//TODO: The queuing function for A* misplaced tiles heuristic 



//TODO: The queuing function for A* Manhattan 



//TODO: The queuing function for uniform cost 





//TODO: a function to check if the current node is a goal state





//TODO: function to calculate f






//driver code
int main(){
    
    int puzzleChoice = 1; 
    int currNum = 0; 
    
    cout << "Welcome to Bertie Woosters 8-puzzle solver." << endl; 
    cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl; 
    cin >> puzzleChoice;

    if(puzzleChoice == 1){
        //default puzzle is chosen
    }
    else{
        //have the user enter their puzzle
    }

    

    return 0; 
}