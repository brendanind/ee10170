// EE10170 Coursework 2 C++ Code
// bi242@bath.ac.uk
// March 2022
//Logger Class File

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Logger : public ofstream {
public:
    Logger(){
        cout << "Made a logger object\n";
    }

    void write(int data){
        // Method to write data to the file
        cout << "I have written something! (I havent, this is just a test :)";
    }
};