#ifndef MOL_PARSE_H
#define MOL_PARSE_H

#include <iostream>
#include <string>
#include "Atom.h"

class MolParse
{
  public:
    MolParse(); //default constructor - do not recommend as original directory setup is probably different than what we need
    MolParse(std::string directory); //constructor that initializes a directory that can contain multiple mol files
    MolParse(std::string directory, std::string filename); //constructor that initializes a directory and a specific filename
    void parseData(); //function that parses mol file and retrieves coordinate and bonding information
    int getAtomCount() const; //returns number of atoms in mol file
    int getBondCount() const; //returns number of bonds in mol file
    Atom* getAtomList(); //returns list of atoms generated after parsing mol file (only use after parsing the data)
    void setDirectory(std::string); //set the directory to extract mol files
    void setFileName(std::string); //set the filename that parseData function will extract data from
    std::string getDirectoryName() const; //in case user needs to quickly look at the name of the directory
    std::string getFileName() const; //in case user needs to quickly look at mol file name
    void setHeaderLength(int); //set number of lines to skip at beginning of file (in my experience it has always been three but it could maybe vary)
    void displayMoleculeInfo(); //function that displays contents of molFile
  private:
    std::string directory; //directory from which data is extracted
    std::string filename; //name of mol file
    int headerLength; //determines how many lines to skip at beginning of file
    int atomCount; //number of atoms
    int bondCount; //number of bonds (have not done much with this)
    Atom *elementList; //list of atoms w/ info extracted from mol file
    int (*bondNum)[3]; //list of bond information (possibly need to arrange this in a more intuitive manner later on)
};

#endif