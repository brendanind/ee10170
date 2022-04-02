//
// Created by brend on 02/04/2022.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

int main (){
    ifstream s1("_StreamData/sensor_1.csv"); // open the input file
    if (!s1.is_open()) { // check for successful opening
        cout << "Input file could not be opened! Terminating!" << endl;
        return 1;
    }
    ifstream s2("_StreamData/sensor_2.csv"); // open the input file
    if (!s2.is_open()) { // check for successful opening
        cout << "Input file could not be opened! Terminating!" << endl;
        return 1;
    }
    ifstream s3("_StreamData/sensor_3.csv"); // open the input file
    if (!s3.is_open()) { // check for successful opening
        cout << "Input file could not be opened! Terminating!" << endl;
        return 1;
    }

    ofstream o1("_StreamData/sensor_o.csv"); // open the output file
    if (!o1.is_open()) { // check for successful opening
        cout << "Output file could not be opened! Terminating!" << endl;
        return 1;
    }
// read as long as the stream is good - any problem, just quit.
// output is each number times two on a line by itself
    double dat1,dat2,tmp2,dat3;
    double c1,c2,sc1,sc2,sc3,f,ma,mb,fc;
    tmp2=-1;
    string skip;
    while(getline(s1, skip,',')){
        getline(s2,skip,',');
        getline(s3,skip,',');
        s1>>dat1;
        s2>>dat2;
        s3>>dat3;
        c1=(sqrt(dat1))*2/3;
        c2=dat2-tmp2;
        sc1=2.7*(c1-1);
        sc2=0.7*(c2+0.5);
        sc3=1*(dat3-0.2);
        f=3*(sc1-sc3)/sc2-3;
        if(f<-1){
            fc=-1;
        } else if(f>1){
            fc=1;
        } else {
            fc=f;
        };
        ma=fc;
        mb=-1*fc;

        o1<<f<<","<<fc<<endl;
        tmp2=dat2;
    }

    s1.close();
    s2.close();
    s3.close();
    o1.close();
    cout << "Done!" << endl;
    return 0;
}