#ifndef ATOM_H
#define ATOM_H

#include <iostream>
#include <string>

class Atom
{
  public:
    Atom(); //default constructor
    Atom(double, double, double, std::string);
    friend class MolParse; //make MolParse a friend class to set private data members in parseData function
    double getX(); //returns x
    double getY(); //returns y
    double getZ(); //returns z
    std::string getElemName(); //returns abbreviation of element
  private:
    double x, y, z;
    std::string elementName;
};

#endif
