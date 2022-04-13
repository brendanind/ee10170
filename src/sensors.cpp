// EE10170 Coursework 2 C++ Code
// bi242@bath.ac.uk
// March 2022
// Sensor classes file

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

//Include the base Sensor class that inherits from ifstream.
class Sensor : public ifstream {
public:
    float floatvalue, floatvalue_converted, floatvalue_converted_scaled;
    string stringvalue;
    Sensor(){
        cout << "created a Sensor Object\n";

    }
    //Scale function is the same for all 3 sensors so we can define it in the base class and all children can inherit the function:
    float scale(float input, float alpha_gain, float beta_offset){
        return alpha_gain*(input - beta_offset);
    }
};

//Specific sensor stream 1 class inherited from Sensor class
class Sensor1 : public Sensor {
public:
    //The conversion function specific to Sensor1 hence defined here not in the base class:
    float convert(float input){
        float two_thirds = (float)2 / (float)3;
        //cout << two_thirds << " multiplied by " << sqrt(input) << endl;
        return (float)two_thirds*(float)sqrt(input);
    }
};

//Sensor 2 stream inherited from the main Sensor class
class Sensor2 : public Sensor {
public:
    // Specific convert function for sensor stream 2 hence its declared here not in the base Sensor class:
    float convert(float previous_value, float current_value){
        return current_value - previous_value;
    }
};