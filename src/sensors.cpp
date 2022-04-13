// EE10170 Coursework 2 C++ Code
// bi242@bath.ac.uk
// March 2022
// Sensor classes file

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Sensor : public ifstream {
public:
    float floatvalue, floatvalue_converted, floatvalue_converted_scaled;
    string stringvalue;
    Sensor(){
        cout << "created a Sensor Object\n";

    }
    float scale(float input, float alpha_gain, float beta_offset){
        return alpha_gain*(input - beta_offset);
    }
};

class Sensor1 : public Sensor {
public:
    //Sensor:Sensor(); //Call the constructor of the base class
    float convert(float input){
        float two_thirds = (float)2 / (float)3;
        //cout << two_thirds << " multiplied by " << sqrt(input) << endl;
        return (float)two_thirds*(float)sqrt(input);
    }
};

class Sensor2 : public Sensor {
public:
    //Sensor:Sensor(); //Call the constructor of the base class
    float convert(float previous_value, float current_value){
        return current_value - previous_value;
    }
};