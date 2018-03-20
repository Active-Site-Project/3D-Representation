#include <iostream>
#include <fstream>
#include "MolParse.h"

const int HLEN = 3; //constant for default header length, can also be set in program


MolParse::MolParse() : // default constructor
directory("mol/"), filename("data1.mol"), headerLength(HLEN), atomCount(0), bondCount(0)
{
}

MolParse::MolParse(std::string dir) : // constructor that specifies directory only
directory(dir), filename("data1.mol"), headerLength(HLEN), atomCount(0), bondCount(0)
{
}

MolParse::MolParse(std::string dir, std::string file) : // constructor that specifies director and filename
directory(dir), filename(file), headerLength(HLEN), atomCount(0), bondCount(0)
{
}

void MolParse::parseData() //parse data function definition
{
  std::ifstream molFile;
  std::string buffer, fileLocation;
  int fileNameLength = filename.length();

  if (directory[directory.length()] != '/') //appends / to directory name if there is not one already given (so filename can easily be appended)
    directory = directory + "/";
  if (filename.substr(fileNameLength-4, fileNameLength) != ".mol") // there is probably a better way of doing this
    throw ("Invalid File Type!");
  fileLocation = directory + filename; //name of mol file location on computer to open
  molFile.open(fileLocation.c_str());
  for (int i = 0; i < headerLength; i++) //skip header of mol file (contains info we don't need)
    getline(molFile, buffer, '\n');
  molFile >> atomCount; //gets the total number of atoms from mol file
  molFile >> bondCount; //gets the total number of bonds from mol file
  elementList = new Atom[atomCount]; //dynamically create a list of atoms based on the number of atoms in listed in mol file
  bondNum = new int[bondCount][3]; //dynamically create 2D array where each from row represents a different bond ([rows][columns])
  //the first column is the index of one element, second column is index of second element, and third column is bond type (single, double, etc)
  getline(molFile, buffer, '\n');
  for (int i = 0; i < atomCount; i++) //initialize coordinates and element abbreviation of each atom in element list to corresponding atom in mol file
  {
    molFile >> elementList[i].x;
    molFile >> elementList[i].y;
    molFile >> elementList[i].z;
    molFile >> elementList[i].elementName;
    getline(molFile, buffer, '\n'); //skip the rest of the data and move to next line (this makes sense if you go look at a mol file)
  }
  for (int i = 0; i < bondCount; i++) //initialize bond information from mol file
  {
    molFile >> bondNum[i][0];
    bondNum[i][0]--; //subtract 1 so bond index is zero-indexed
    molFile >> bondNum[i][1];
    bondNum[i][1]; //subtract 1 so bond index is zero-indexed
    molFile >> bondNum[i][2];
    getline(molFile, buffer, '\n');
  }
  molFile.close();
}

Atom* MolParse::getAtomList() //returns list of atoms for use in other programs
{
  if (atomCount == 0) //there has to be atoms in elementList or error is thrown
    parseData(); //function that parses mol file and retrieves coordinate and bonding information
    //throw("no atoms currently available"); //not really sure how to handle errors in C++
  return elementList;
}

int MolParse::getAtomCount() //returns number of atoms from mol file (if mole file has not been read, the count is zero)
{
  return atomCount;
}

int MolParse::getBondCount() //returns number of bond from mol file (if mole file has not been read, the count is zero)
{
  return bondCount;
}

void MolParse::setDirectory(std::string dirName) //gives user option to set a new directory name
{
  directory = dirName;
}

void MolParse::setFileName(std::string fileName) //gives user option to set different mol files
{
  filename = fileName;
}

std::string MolParse::getDirectoryName() //returns name of directory MolParse object is set to
{
  return directory;
}

std::string MolParse::getFileName() //returns name of file MolParse object is set to
{
  return filename;
}
