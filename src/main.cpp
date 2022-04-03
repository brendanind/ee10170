// EE10170 Coursework Code
// bi242@bath.ac.uk
// March 2022
// DWTFUL (Do what the F you like) Licence

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


//SET THIS FLAG TO TRUE IF YOU WANT TO SEE THE DEBUGGING OUTPUTS - PROGRAM RUNS SLOWWWW
bool debugging = false;

using namespace std;

float scale(float input, float alpha_gain, float beta_offset){
    return alpha_gain*(input - beta_offset);
}

float sensor1conversion(float input){
    float two_thirds = (float)2 / (float)3;
    //cout << two_thirds << " multiplied by " << sqrt(input) << endl;
    return (float)two_thirds*(float)sqrt(input);
}

float sensor2conversion(float previous_value, float current_value){
    return current_value - previous_value;
}

float sensor_fusion(float sensor1, float sensor2, float sensor3){
    float top = (float)3*(float)(sensor1 - sensor3);
    float bottom = (float)sensor2;
    float fraction = float(top) / (float)bottom;
    //cout << "\n" << top << "/" << bottom << "=" << fraction << endl;
    float fused = fraction - 3;
    //cout << "*********Fusion process: (" << top << "/" << bottom << ")-3=" << fused << "\n";
    return fused;

}

float clip(float value){
    if (value > 1){
        return 1;
    }
    else if(value < -1){
        return -1;
    }
    else{
        return value;
    }
}

string delim(string str, string delim){
    // Returns the stuff after the delimiter:
    return str.substr(str.find(delim)+1);
}

string sensor1location = "../sensors/actual/sensor_1.csv";
string sensor2location = "../sensors/actual/sensor_2.csv";
string sensor3location = "../sensors/actual/sensor_3.csv";

string verbose_location = "../sensors/verbose.csv";
string motor_location = "../sensors/motors.csv";

string sensor1stringvalue;
string sensor2stringvalue;
string sensor3stringvalue;

float sensor1floatvalue;
float sensor2floatvalue;
float sensor3floatvalue;

float sensor1floatvalue_converted;
float sensor2floatvalue_converted;
// sensor3floatvalue_converted not needed as sensor 3 has no conversions

float sensor1floatvalue_converted_scaled;
float sensor2floatvalue_converted_scaled;
float sensor3floatvalue_converted_scaled; //not actually converted but keep with convention and call it converted_scaled

float fused;
float clipped_motor;
float inverse_clipped;
float prev_sensor2_temp;

//Use this value when there is no previous value to use yet when converting sensor 2:
float previousSensorValue = -1;

int main() {

   if (not debugging){
        /*
        If we are debugging then we want cout otherwise we set it to a fail state, so it outputs nothing and makes
        The program quicker
        */
        std::cout.setstate(std::ios_base::failbit);
    }
    else {
        cout.clear();
    }


    //Initiate all the input stream sensor objects
    ifstream sensor1file;
    ifstream sensor2file;
    ifstream sensor3file;

    // Open the sensor files in the objects
    sensor1file.open(sensor1location);
    sensor2file.open(sensor2location);
    sensor3file.open(sensor3location);
    ofstream verbose(verbose_location);
    ofstream motors(motor_location);

    // Make sure that all the sensors are open
    if(!sensor1file.is_open()) throw runtime_error("Could not open Sensor 1");
    if(!sensor2file.is_open()) throw runtime_error("Could not open Sensor 2");
    if(!sensor3file.is_open()) throw runtime_error("Could not open Sensor 3");
    if(!verbose.is_open()) throw runtime_error("Could not open Verbose file");
    if(!motors.is_open()) throw runtime_error("Could not open Motors file");


    //Set up the output files:
    verbose << "a," << "b," << "c," << "d," << "e," << "f"<< "\n";
    motors << "a," << "b" << "\n";

    //While there's still a line left to read in the sensor1file:
    while (getline(sensor1file, sensor1stringvalue)){

        //Get the other two string lines of each of the other two sensors and save to sensorXvalue variable
        getline(sensor2file, sensor2stringvalue);
        getline(sensor3file, sensor3stringvalue);

        //Delimit those strings, so they contain just the sensor values and not the line numbers:
        sensor1stringvalue = delim(sensor1stringvalue, ",");
        sensor2stringvalue = delim(sensor2stringvalue, ",");
        sensor3stringvalue = delim(sensor3stringvalue, ",");

        //We are done delim-ing using string, so we convert to a float, so we can perform math on it.
        sensor1floatvalue = stof(sensor1stringvalue);
        sensor2floatvalue = stof(sensor2stringvalue);
        sensor3floatvalue = stof(sensor3stringvalue);

        //Print the unedited string sensor values:
        cout << "Sensor1original: " << sensor1stringvalue << endl << "Sensor2original: " << sensor2stringvalue << endl << "Sensor3original: " << sensor3stringvalue << "\n";

        //convert the first sensor value using the function
        sensor1floatvalue_converted = sensor1conversion(sensor1floatvalue);
        cout << "Sensor1converted: (2/3)*sqrt(" << sensor1floatvalue <<")= " << sensor1floatvalue_converted << endl;
        //Output the result to the verbose (POINT A)
        verbose << sensor1floatvalue_converted << ",";

        //Convert the second sensor using the previous value and the current one.
        sensor2floatvalue_converted = sensor2conversion(previousSensorValue, sensor2floatvalue);
        cout << "Sensor2converted: " << sensor2floatvalue << "-" << previousSensorValue << "= " << sensor2floatvalue_converted << endl;
        //Output the result to the verbose for (POINT B)
        verbose << sensor2floatvalue_converted << ",";

        //Update the previous sensor value as the raw sensor 2 value for the next time the loop runs
        previousSensorValue = sensor2floatvalue;

        //Scale all the sensors using their respective biases
        sensor1floatvalue_converted_scaled = scale(sensor1floatvalue_converted, 2.7, 1);
        sensor2floatvalue_converted_scaled = scale(sensor2floatvalue_converted, 0.7, -0.5);
        sensor3floatvalue_converted_scaled = scale(sensor3floatvalue, 1, 0.2);
        //^^Remember that sensor3floatvalue_converted doesn't exist, so we use raw value here.

        //Output the scaled values to the verbose file
        verbose << sensor1floatvalue_converted_scaled << ",";      // (POINT C)
        verbose << sensor2floatvalue_converted_scaled << ",";      // (POINT D)
        verbose << sensor3floatvalue_converted_scaled << ",";      // (POINT E)

        cout << "Sensor1scaled: 2.7(" << sensor1floatvalue_converted << "-1)= " << sensor1floatvalue_converted_scaled << endl << "Sensor2scaled: 0.7(" << sensor2floatvalue_converted<< "--0.5)= " << sensor2floatvalue_converted_scaled << endl << "Sensor3scaled: 1(" << sensor3floatvalue << "-0.2)= " << sensor3floatvalue_converted_scaled << "\n";

        //Sensor fusion time !
        fused = sensor_fusion(sensor1floatvalue_converted_scaled, sensor2floatvalue_converted_scaled, sensor3floatvalue_converted_scaled);
        cout << "Fused: ((3(" << sensor1floatvalue_converted_scaled << "-" << sensor3floatvalue_converted_scaled << "))/" << sensor2floatvalue_converted_scaled << ")-3= " << fused << endl;
        //Send to verbose: (POINT F)
        verbose << fused << endl;

        //Output the clipped motor value to the motor.csv file
        clipped_motor = clip(fused);
        inverse_clipped = clipped_motor * -1;
        motors << clipped_motor <<","<< inverse_clipped << endl;
        cout << "Original was " << fused << " and Clipped: " << clipped_motor << " " << "Inverse (" << clipped_motor << "*-1=):  " << inverse_clipped << endl;

        cout << "\n\n\n";
    }

    sensor1file.close();
    sensor2file.close();
    sensor3file.close();
    verbose.close();
    motors.close();

    cout.clear();
    cout << "All done! Closing!";
    return 0;
}