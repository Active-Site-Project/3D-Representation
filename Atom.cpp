#include <iostream>
#include "Atom.h"

//default constructor - arbitrarily initialize hydrogen centered at (0,0,0)
Atom::Atom() :
x(0), y(0), z(0), elementName("H")
{
}

//alternate constructor if we need to actually initialize atom with coordinates and type later on
Atom::Atom(double xPos, double yPos, double zPos, std::string elemAbbrev) :
x(xPos), y(yPos), z(zPos), elementName(elemAbbrev)
{
}

//returns x-coordinate
double Atom::getX() const
{
  return x;
}

//returns y-coordinate
double Atom::getY() const
{
  return y;
}

//returns z-coordinate
double Atom::getZ() const
{
  return z;
}

//returns name/abbreviation of element
std::string Atom::getElemName() const
{
  return elementName;
}
