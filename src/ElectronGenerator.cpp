#include <math.h>

double randDouble(double rad); //Generates a random double that within the atoms radius

int main (int argc, char* argv[]) {
    double radius = std::atof(argv[1]); //sets radius equal to the first arguement
    int eCount = std::atof(argv[2]);    //sets number of electrons to be generated to the second argument
    double Xr, Yr, Zr;     //random (x,y,z) location for electron
    double Xlist [eCount]; //list containing all of the random X values
    double Ylist [eCount]; //list containing all of the random Y values
    double Zlist [eCount]; //list containing all of the random Z values

    for(int i=0; i<eCount; i++)   //randomly generates a set num of pix around atom
    {
        Xr = randDouble(radius);  //generates random X value
        Xlist[i] = Xr;                  //adds random X value to array
        Yr = randDouble(radius);  //generates random Y value
        Ylist[i] = Yr;                  //adds random Y value to array
        Zr = randDouble(radius);  //generates random Z value
        Zlist[i] = Zr;                  //adds random Z value to array
        //cout <<Xlist[i]<<","<<Ylist[i]<<","<<Zlist[i]<<endl;  //Prints out each electrons location
    }
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
