#include <iostream>
#include <stdlib.h> //for rand 
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

//TODO: Implement this function 
//random accuracy generator
double CrossValidation(){
    return rand() % 100;     
}

//checks if a feature already exists in the set of features(used in search function)
bool isEmpty(vector<int> &features, int k){

    if(find(features.begin(), features.end(), k) != features.end()){
        return false; 
    }

    return true; 
    
}

//search function to find the most accurate feature set to use
void FeatureSearch(vector<vector<double>> &data){
    vector<int> currentFeatures;
    int featureToAdd; 
    double bestAccuracy = 0; 
    double currAccuracy = 0; 
     

    //iterate through all the features(levels) 
    for(int i = 1; i <= data.at(0).size() - 1; ++i){
        cout << "On " << i << "th level of the search tree" << endl; 
        bestAccuracy = 0; 

        //the additional features
        for(int k = 1; k <= data.at(0).size() - 1; ++k){

            //make sure feature doesn't already exist in set
            if(isEmpty(currentFeatures, k)){

                cout << "--Considering adding the " << k << "th feature" << endl;
                currAccuracy = CrossValidation(); 

                //check if currAccuracy is new max
                if(currAccuracy > bestAccuracy){
                    bestAccuracy = currAccuracy; 
                    featureToAdd = k; 
                }
            }  
        } 

        //push the new feature to currFeatures
        currentFeatures.push_back(featureToAdd); 
        cout << "On level " << i << " I added feature " << featureToAdd << " to current set" << endl; 
    }

    return; 
}

int main() {
    srand(time(0)); 
	fstream file;
    //stores the small data set into 2d array
	vector<vector<double>> smallData;
	vector<double> currRow(11); // row to add into the 2d vector 

	// Read file
	file.open("small.txt"); 
	if (file.is_open()) {

		while (file.good()) {
			for (int i = 0; i < 11; ++i) {
				file >> currRow.at(i);  
			}
			smallData.push_back(currRow); 
		}
	}
	else cout << "Unable to open file" << endl;
	file.close();

    // for(auto j: smallData){
    //     for(auto k: j){
    //         cout << k << "  ";
    //     }
    //     cout << endl; 
    // }

    FeatureSearch(smallData); 



        
    return 0;
}