/*
File: 451proj-part2.cpp
Project: CMSC 451 Course Project Part 2
Authors: Humza Faraz & Joshua Philip Santos
Date: 12/1/21
Emails: hfaraz1@umbc.edu & jsantos3@umbc.edu
Description:
*
DPDA simulator. Creates DPDA from text file then takes in user input and determines if input is validated. 
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <string>
#include <tuple>
using namespace std;

int main(int argc, char* argv[]){
    ifstream myfile;
    if (argc < 2){
        cout << "Missing data file!" << endl;
        exit(0);
    }
    else{
        myfile.open(argv[1]);
    }

    string line;

    string mapType;
    vector<char> inputAlpha;
    vector<char> stackAlpha;
    int numStates;
    vector<string> acceptStates;

    if (myfile.is_open()){
        //Get map type
        getline(myfile, mapType);
        if (mapType != "DPDA"){
            cout << "Invalid map type!" << endl;
            exit(0);
        }

        //Get input alphabet
        getline(myfile, line);
        for (int i = 0; i < line.length(); i++){
            if (line[i] != ' '){
                inputAlpha.push_back(line[i]);
            }
        }

        //Get stack alphabet
        getline(myfile, line);
        for (int i = 0; i < line.length(); i++){
            if (line[i] != ' '){
                stackAlpha.push_back(line[i]);
            }
        }

        //Get number of states
        getline(myfile, line);
        numStates = atoi(line.c_str());

        //Get accept states
        getline(myfile, line);
        string state = "";
        char language;
        for (int i = 0; i < line.length(); i++){
            //for later vaidation
            if (i == 0){
                language = line[0];
            }

            if (line[i] != ' '){
                state += line[i];
            }
            else{
                //validate
                if (state[0] != language){
                    cout << "Invalid text file!" << endl;
                    exit(0);
                }
                acceptStates.push_back(state);
                state = "";
            }
        }
        acceptStates.push_back(state);

        //Get hyphens
        getline(myfile, line);
        if (line != "---"){
            cout << "Invalid hyphens!" << endl;
            exit(0);
        }

        //Get rules
        vector < tuple<string, char, char, string, char> > rules;
        string beginState;
        int count = 0;
        while (getline(myfile, line)){
            string startState;
            char alphaInput;
            char alphaStack;
            string transition;
            char pushStack;

            startState += line[0];
            startState += line[1];

            if (count == 0){
                beginState = startState;
                count++;
            }

            alphaInput = line[3];

            alphaStack = line[5];

            transition += line[7];
            transition += line[8];

            pushStack = line[10];

            //validations for states
            if (startState[0] != language || transition[0] != language){
                cout << "Invalid state!" << endl;
                exit(0);
            }
            //validations for alphabet
            if (alphaInput != inputAlpha[0] && alphaInput != inputAlpha[1] && alphaInput != ' '){
                cout << "Invalid input alphabet!" << endl;
                exit(0);
            }
            //validations for stack input
            if (alphaStack != stackAlpha[0] && alphaStack != stackAlpha[1] && alphaStack != ' '){
                cout << "Invalid stack alphabet!" << endl;
                exit(0);
            }
            //validation for stack push
            if (pushStack != stackAlpha[0] && pushStack != stackAlpha[1] && pushStack != ' '){
                cout << "Invalid stack alphabet!" << endl;
                exit(0);
            }

            //cout << startState << " " << inputAlpha << " " << stackAlpha << " " << transition << " " << pushStack << endl;
            tuple <string, char, char, string, char> myTuple;
            myTuple = make_tuple(startState, alphaInput, alphaStack, transition, pushStack);
            rules.push_back(myTuple);
        }


        //Get user input
        while (true){
            string userInput;
            cout << "Please enter an input string" << endl;
            getline(cin, userInput);

            string output;
            vector<string> states;
            vector<char> stack;
            states.push_back(beginState);
            stack.push_back(' ');
            string currentState = beginState;

            bool isValid = true;
            for (int i = 0; i < userInput.length(); i++){
                //check validity
                for (int i = 0; i < (int)inputAlpha.size() - 1; i++){
                    if (userInput[i] != inputAlpha[i] && userInput[i] != inputAlpha[i+1] && userInput[i] != ' '){
                        cout << "Invalid input!" << endl;
                        isValid = false;
                        break;
                    }
                }
                if (isValid == false) {
                    break;
                }

                //Get path
                for (int i = 0;  i < userInput.length(); i++){
                    //look through rules to find new state
                    for (int j = 0; j < (int)rules.size(); j++){
                        if ( (userInput[i] == get<1>(rules[j])) && (stack.back() == get<2>(rules[j])) ){
                            currentState = get<3>(rules[j]);
                            states.push_back(currentState);

                            //push or pop to stack
                            if (get<4>(rules[j]) != ' '){
                                char newStack = get<4>(rules[j]);
                                if (stack.back() == newStack){
                                    stack.push_back(newStack);
                                }
                                else{
                                    stack.pop_back();                                   
                                    //add newStack if stack is now empty
                                    if (stack.empty()){
                                        stack.push_back(newStack);
                                    }
                                    
                                }
                            }

                        }
                    }
                }

                //Check if input is valid
                bool isFound = false;
                for (int i = 0; i < (int)acceptStates.size(); i++){
                    if (currentState == acceptStates[i]){
                        isFound = true;
                    }
                }

                //Inform user whether their input was valid or not
                if (isValid == true){
                    if (isFound == true){
                        cout << "Input is accepted. Path:" << endl;
                    }
                    else{
                        cout << "Input is rejected. Path:" << endl;
                    }
                    for (int i = 0; i < (int)states.size(); i++){
                        cout << states[i] << " ";
                    }
                    cout << endl;
                }

            }

        }
    }
    else{
        cout << "Invalid text file!" << endl;
        exit(0);
    }

    return 0;
}