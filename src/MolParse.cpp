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

  if(atomCount) //do nothing if parsed, or else old element list will be left floating in memory
      delete [] elementList;


  std::ifstream molFile;
  std::string buffer, fileLocation;
  int fileNameLength = filename.length();

  if (directory[directory.length()-1] != '/') //appends / to directory name if there is not one already given (so filename can easily be appended)
    directory = directory + "/";
  if(fileNameLength  < 5) //in this case, we either have only .mol or anthing other than a filename.mol which has a min of 5 chars. ex. a.mol
    throw ("Invalid File Type!");
  if (filename.substr(fileNameLength-4, fileNameLength) != ".mol") // there is probably a better way of doing this
    throw ("Invalid File Type!");
  fileLocation = directory + filename; //name of mol file location on computer to open
  molFile.open(fileLocation.c_str());
  for (int i = 0; i < headerLength; i++) //skip header of mol file (contains info we don't need)
    getline(molFile, buffer, '\n');
  molFile >> atomCount; //gets the total number of atoms from mol file

  if (atomCount >= 100000)
  {
    bondCount = atomCount % 1000;
    atomCount = atomCount / 1000;
  }
  else
  {
    molFile >> bondCount; //gets the total number of bonds from mol file
  }
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
    if (bondNum[i][0] >= 100000)
    {
      bondNum[i][1] = bondNum[i][0] % 1000 - 1;
      bondNum[i][0] = bondNum[i][0] / 1000 - 1;
    }
    else
    {
      bondNum[i][0]--; //subtract 1 so bond index is zero-indexed
      molFile >> bondNum[i][1];
      bondNum[i][1]--; //subtract 1 so bond index is zero-indexed
    }
    molFile >> bondNum[i][2];
    getline(molFile, buffer, '\n');
  }
  molFile.close();
}

Atom* MolParse::getAtomList() //returns list of atoms for use in other program
{
  if (!atomCount) //there have to be atoms in elementList or error is thrown
    throw("getAtomListError: No atoms currently available. Try parsing data."); //not really sure how to handle errors in C++
  return elementList;
}

int MolParse::getAtomCount() const //returns number of atoms from mol file (if mole file has not been read, the count is zero)
{
  return atomCount;
}

int MolParse::getBondCount() const //returns number of bond from mol file (if mole file has not been read, the count is zero)
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

std::string MolParse::getDirectoryName() const //returns name of directory MolParse object is set to
{
  return directory;
}

std::string MolParse::getFileName() const //returns name of file MolParse object is set to
{
  return filename;
}

void MolParse::displayMoleculeInfo()
{
  if (!atomCount) //there has to be atoms in elementList or error is thrown
    throw("displayMoleculeInfoError: No atoms to display. Try parsing file first");
  else
  {
    std::cout << directory << filename << "\n\n";
    std::cout << "Atom Count: " << atomCount << '\n';
    std::cout << "Bond Count: " << bondCount << "\n\n";
    for (int i = 0; i < atomCount; i++)
      std::cout << elementList[i].getX() << ' ' << elementList[i].getY() << ' ' << elementList[i].getZ() << ' ' << elementList[i].getElemName() << '\n';
    std::cout << "\n\n";
  }
}
