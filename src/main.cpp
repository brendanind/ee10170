// EE10170 Coursework Code
// bi242@bath.ac.uk
// March 2022
// DWTFUL (Do what the F you like) Licence

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

float scale(float input, float alpha_gain, float beta_offset){
    return alpha_gain*(input - beta_offset);
}

float sensor1conversion(float input){
    float two_thirds = 2 / 3;
    return (float)two_thirds*(float)sqrt(input);
}

float sensor2conversion(float previous_value, float current_value){
    return current_value - previous_value;
}

float sensor_fusion(float sensor1, float sensor2, float sensor3){
    return ((3*(sensor1-sensor3))/(sensor2))-3;
}

float clip(float value){
    if (value > 3){
        return 3;
    }
    else{
        return value;
    }
}

string delim(string str, string delim){
    // Returns the stuff after the delimiter:
    return str.substr(str.find(delim)+1);
}

string sensor1location = "../sensors/sensor_1.csv";
string sensor2location = "../sensors/sensor_2.csv";
string sensor3location = "../sensors/sensor_3.csv";

string output_stream_location = "../sensors/output.csv";

string sensor1stringvalue;
string sensor2stringvalue;
string sensor3stringvalue;

float sensor1floatvalue;
float sensor2floatvalue;
float sensor3floatvalue;

//Use this value when there is no previous value to use yet when converting sensor 2:
float previousSensorValue = -1;

int main() {
    //Initiate all the input stream sensor objects
    ifstream sensor1file;
    ifstream sensor2file;
    ifstream sensor3file;

    // Open the sensor files in the objects
    sensor1file.open(sensor1location);
    sensor2file.open(sensor2location);
    sensor3file.open(sensor3location);

    // Make sure that all the sensors are open
    if(!sensor1file.is_open()) throw runtime_error("Could not open Sensor 1");
    if(!sensor2file.is_open()) throw runtime_error("Could not open Sensor 2");
    if(!sensor3file.is_open()) throw runtime_error("Could not open Sensor 3");

    //While there's still a line left to read in the sensor1file:
    while (getline(sensor1file, sensor1stringvalue)){

        //Get the string lines of each of the sensors and save to sensorXvalue variable
        getline(sensor1file, sensor1stringvalue);
        getline(sensor2file, sensor2stringvalue);
        getline(sensor3file, sensor3stringvalue);

        //Delimit those strings, so they contain just the sensor values and not the line numbers:
        sensor1stringvalue = delim(sensor1stringvalue, ",");
        sensor2stringvalue = delim(sensor2stringvalue, ",");
        sensor3stringvalue = delim(sensor3stringvalue, ",");

        //Print the unedited string sensor values:
        cout << "Sensor1original: " << sensor1stringvalue << endl << "Sensor2original: " << sensor2stringvalue << endl << "Sensor3original: " << sensor3stringvalue << "\n\n";

        /*
        //convert the first sensor value using the function
        sensor1floatvalue = sensor1conversion(sensor1floatvalue);
        cout << "Sensor1converted: " << sensor1floatvalue << endl;

        //Convert the second sensor using the previous value and the current one.
        sensor2floatvalue = sensor2conversion(previousSensorValue, sensor2floatvalue);
        cout << "Sensor2converted: " << sensor2floatvalue << endl;

        cout << "\n\n\n";

        //Update the previous sensor value for the next time the loop runs
        previousSensorValue = sensor2floatvalue;

        //Scale all the sensors using their respective biases
        sensor1floatvalue = scale(sensor1floatvalue, 2.7, 1);
        sensor2floatvalue = scale(sensor2floatvalue, 0.7, -0.5);
        sensor3floatvalue = scale(sensor3floatvalue, 1, 0.2);
         */
    }
    return 0;
}