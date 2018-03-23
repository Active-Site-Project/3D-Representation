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
    double getX() const; //returns x
    double getY() const; //returns y
    double getZ() const; //returns z
    std::string getElemName() const; //returns abbreviation of element
  private:
    double x, y, z;
    std::string elementName;
};

#endif
