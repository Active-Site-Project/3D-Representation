//  main.cpp
//  Electron Cloud Placement
//  Purpose: Takes in 3 parameters 1)the radius of the Atom 2) the number of electrons per atom 3)Atom name
//  Program randomly generates a user choosen number of points at random points within a radius
//  that the user inputs
//  Created by Bryce Kroencke on 3/12/18.
//  Copyright © 2018 Bryce Kroencke. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <string>

using namespace std;

const string writeFilePath = "ElectronClouds/";

ofstream OFile;

double randDouble(double rad); //Generates a random double that within the atoms radius

int main (int argc, char* argv[]) {
    double radius = atof(argv[1]); //sets radius equal to the first arguement
    int eCount = atof(argv[2]);    //sets number of electrons to be generated to the second argument
    string fileName = argv[3]; //sets the name of file in folder
    fileName += ".txt";
    
    double Xr, Yr, Zr;     //random (x,y,z) location for electron
    double Xlist [eCount]; //list containing all of the random X values
    double Ylist [eCount]; //list containing all of the random Y values
    double Zlist [eCount]; //list containing all of the random Z values
    
    OFile.open(writeFilePath + fileName);
    
    for(int i=0; i<eCount; i++)   //randomly generates a set num of pix around atom
    {
        Xr = randDouble(radius);  //generates random X value
        Xlist[i] = Xr;                  //adds random X value to array
        Yr = randDouble(radius);  //generates random Y value
        Ylist[i] = Yr;                  //adds random Y value to array
        Zr = randDouble(radius);  //generates random Z value
        Zlist[i] = Zr;                  //adds random Z value to array
        OFile <<Xlist[i]<<","<<Ylist[i]<<","<<Zlist[i]<<endl;  //Prints out each electrons location
    }
    
    OFile.close();
    return 0;
}


double randDouble(double rad)
{
    double temp;
    double low = -rad, high=rad;
    
    // calculate the random number & return it
    temp = (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (high - low) + low;
    return temp;
}
