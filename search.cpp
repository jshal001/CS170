#include <iostream>

using namespace std; 

//the struct that will be used to represent each state/node
struct state{ 
    //this 2d array will be used to store the state
    vector<vector<int>> puzzle; 
    //cost g(i.e. the cost to arrive at the node)
    int g = 0; 
    
    //how far the state is 
    int h = 0; 
    
    //f(n) = h(n) + g(n)
    int f = 0; 
}




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
        cout << 
    }

    

    return 0; 
}