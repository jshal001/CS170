#include <iostream>
#include <stdlib.h> //for rand 
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <cmath>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <chrono>
using namespace std;

bool isEmpty(vector<int>&, int); 
int GetIndex(vector<int>&, int); 

//function to create a new vector for an instance excluding the class
vector<double> CreateObject(vector<double> v){
    vector<double> temp; 

    for(unsigned i = 1; i < v.size(); ++i){
        temp.push_back(v.at(i)); 
    }

    return temp; 
}


//takes in two vectors and calculates the distance 
double FindDistance(vector<double> v1, vector<double> v2){
    double sum = 0; 

    for(unsigned i = 0; i < v1.size(); ++i){

        double temp = v1.at(i) - v2.at(i); 
        sum += pow(temp, 2); 
    }
    
    return sqrt(sum); 
}


//choice decides if you remove the new feature or add it 
//choice == 1 add
//choice == 2 remove 
//choice == anything else, neither 
double CrossValidation(vector<vector<double>> data, vector<int> currentSet, int newFeature, int choice){
    //first, make the change(whether that's delete or add)
    if(choice == 1 || choice == 2){
        //if forwardselection, push new feature
        if(choice == 1){
            currentSet.push_back(newFeature);
        }
        else{
            //remove the feature
            currentSet.erase(currentSet.begin() + GetIndex(currentSet, newFeature));
        }
    
        //now iterate through data and zero out all features not in current set
        for(unsigned i = 0; i < data.size(); ++i){
            for(unsigned j = 1; j < data.at(i).size(); ++j){
                //if it's not in the set
                if(isEmpty(currentSet, j)){
                    //set it to 0
                    data.at(i).at(j) = 0; 
                }
            }
        }
    }
    double numberCorrect = 0; 
    for(unsigned i = 0; i < data.size(); ++i){
        //grab the subset of the vector at index i excluding the class(first index)
        vector<double> objectToClassify = CreateObject(data.at(i)); 
        double labelObjectClassify = data.at(i).at(0);  

        double nearestDistance = INFINITY; 
        double nearestLocation = INFINITY; 
        double nearestLabel = 0; 
        
        for(unsigned k = 0; k < data.size(); ++k){
            if(k != i){

                //create subset of current neighbor to calculate distance
                vector<double> tempObject = CreateObject(data.at(k));
                double distance = FindDistance(objectToClassify, tempObject);
               
                
                if(distance < nearestDistance){
                    nearestDistance = distance; 
                    nearestLocation = k; 
                    nearestLabel = data.at(nearestLocation).at(0); 
                }
            }
        }
        if(labelObjectClassify == nearestLabel){
            numberCorrect+=1.0; 
        }
    }
    double size = data.size() * 1.0; 
    return numberCorrect/size; 
}

//checks if a feature already exists in the set of features(used in search function)
bool isEmpty(vector<int> &features, int k){

    if(find(features.begin(), features.end(), k) != features.end()){
        return false; 
    }

    return true; 
    
}

//helper function to find index of given value 
int GetIndex(vector<int>& features, int val){
    vector<int>::iterator it = find(features.begin(), features.end(), val); 

    
    return distance(features.begin(), it); 
    
}


//Forward Search
void ForwardSelection(vector<vector<double>> &data){
    vector<int> currentFeatures = {}; 
    int featureToAdd; 
    double bestAccuracy = 0; 
    double currAccuracy = 0; 
    
    vector<int> finalSet; 
    double finalAccuracy = 0; 

    //iterate through all the features(levels) 
    for(unsigned i = 1; i <= data.at(0).size() - 1; ++i){
        cout << "On " << i << "th level of the search tree" << endl; 
        bestAccuracy = 0; 
        featureToAdd = 0; 

        //the additional features
        for(unsigned k = 1; k <= data.at(0).size() - 1; ++k){

            //make sure feature doesn't already exist in set
            if(isEmpty(currentFeatures, k)){

                cout << "--Considering adding the " << k << "th feature" << endl;
                currAccuracy = CrossValidation(data, currentFeatures, k, 1); 

                //check if currAccuracy is new max
                if(currAccuracy > bestAccuracy){
                    bestAccuracy = currAccuracy; 
                    featureToAdd = k; 
                }
            }  
        } 

        //push the new feature to currFeatures
        currentFeatures.push_back(featureToAdd); 
        cout << "On level " << i << " I added feature " << featureToAdd << " to current set for accuracy: " << bestAccuracy * 100 << "%" << endl;

        if(bestAccuracy > finalAccuracy){
            finalAccuracy = bestAccuracy; 
            finalSet = currentFeatures; 
        } 
    }
    cout << "Finished search!! The best feature subset is { "; 
    for(auto i: finalSet){
        cout << i << " ";
    }
    cout << "}, which has an accuracy" << endl << "of " << finalAccuracy * 100 << "%" << endl; 

    return; 
}

//Backwards search
void BackwardElimination(vector<vector<double>> &data){
    vector<int> currentFeatures;
    //populate vector with all features
    for(unsigned i = 1; i < data.at(0).size(); ++i){
        currentFeatures.push_back(i); 
    }


    int featureToRemove; 
    double bestAccuracy = 0; 
    double currAccuracy = 0; 
    
    //the final answers
    vector<int> finalSet; 
    double finalAccuracy = 0; 
    

    //iterate through all the features(levels) 
    for(unsigned i = data.at(0).size() - 1; i >= 1; --i){
        cout << "On " << i << "th level of the search tree" << endl; 
        bestAccuracy = 0; 

        //the additional features
        for(unsigned k = 1; k <= data.at(0).size() - 1; ++k){

            //make sure feature exists in set
            if(!isEmpty(currentFeatures, k)){

                cout << "--Considering removing the " << k << "th feature" << endl;
                currAccuracy = CrossValidation(data, currentFeatures, k, 2); 

                //check if currAccuracy is new max
                if(currAccuracy > bestAccuracy){
                    bestAccuracy = currAccuracy; 
                    featureToRemove = k; 
                }
            }  
        } 

        //remove feature from currFeatures 
        currentFeatures.erase(currentFeatures.begin() + GetIndex(currentFeatures, featureToRemove));  
        cout << "On level " << i << " I removed feature " << featureToRemove << " from current set for accuracy: " << bestAccuracy * 100 << "%" << endl; 
        if(bestAccuracy > finalAccuracy){
            finalAccuracy = bestAccuracy; 
            finalSet = currentFeatures; 
        } 
    }
    cout << "Finished search!! The best feature subset is { "; 
    for(auto i: finalSet){
        cout << i << " ";
    }
    cout << "}, which has an accuracy" << endl << "of " << finalAccuracy * 100 << "%" << endl; 
    return; 
}





int main() {
    string fileName;
    int userInput;  

    cout << "Welcome to Bertie Woosters Feature Selection Algorithm." << endl; 
    cout << "Type in the name of the file to test: "; 
    getline(cin, fileName); 
    cout << endl << "Type the number of the algorithm you want to run." << endl; 
    cout << "    1) Forward Selection" << endl; 
    cout << "    2) Backward Elimination" << endl; 

    cin >> userInput; 

    fstream file;
    //stores the small data set into 2d array
	vector<vector<double>> Data;
    string row; 

	file.open(fileName); 
	if (file.is_open()) {

		while (getline(file, row)){
            vector<double> temp;
            double tempVal;  
            stringstream ss(row); 

            while(ss >> tempVal){
                temp.push_back(tempVal); 
            }

			Data.push_back(temp); 
		}
	}
	file.close();

    cout << "This data set has " << Data.at(0).size() - 1 << " features(not including the class attribute), with"
         << endl << Data.size() << " instances." << endl; 

    cout << "Running nearest neighbor with all " << Data.at(0).size() - 1 << " features, using \"leaving-one-out\" "
         << "evaluation, I get an" << endl; 

    cout << "accuracy of " << CrossValidation(Data, {}, 0, 0) * 100 << "%" << endl; 

    cout << "Beginning search." << endl; 


    auto start = chrono::steady_clock::now(); 
    if(userInput == 1){
        ForwardSelection(Data); 
    }
    else{
        BackwardElimination(Data); 
    }

    auto end = chrono::steady_clock::now(); 

    auto diff = end - start; 

    cout << chrono::duration <double, milli> (diff).count()/1000 << "s" << endl;

        
    return 0;
}