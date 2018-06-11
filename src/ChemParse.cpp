#include "ChemParse.h"

ChemParse::ChemParse() : // default constructor
directory("data/"), filename("data1.mol"), headerLength(0), atomCount(0), bondCount(0), bindingEnergy(0), id(0)
{
}

ChemParse::ChemParse(std::string dir) : // constructor that specifies directory only
directory(dir), filename("data1.mol"), headerLength(0), atomCount(0), bondCount(0), bindingEnergy(0), id(0)
{
}

ChemParse::ChemParse(std::string dir, std::string file) : // constructor that specifies director and filename
directory(dir), filename(file), headerLength(0), atomCount(0), bondCount(0), bindingEnergy(0), id(0)
{
}

void ChemParse::parseData() //parse data function definition
{
  std::string fileExtension;
  if(atomCount) //do nothing if parsed, or else old element list will be left floating in memory
      delete [] elementList;

  int fileNameLength = filename.length();

  if (directory[directory.length()-1] != '/') //appends / to directory name if there is not one already given (so filename can easily be appended)
    directory = directory + "/";

  fileExtension = filename.substr(fileNameLength-4, fileNameLength);
  if (fileExtension == ".mol") // there is probably a better way of doing this
      parseMol();
  else if  (fileExtension == ".sdf")
      parseSDF();
  else
      throw("parseData Error: File type not supported.");

}

void ChemParse::parseMol()
{
  setHeaderLength(3);
  std::ifstream molFile;
  std::string buffer, fileLocation;
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

void ChemParse::parseSDF()
{
    int skipCol = 3;
    std::ifstream SDFFile;
    std::string target, buffer, fileLocation;
    fileLocation = directory + filename; //name of mol file location on computer to open
    SDFFile.open(fileLocation.c_str());
    target = "BEGIN";
    getline(SDFFile, buffer, '\n');
    while (buffer.find(target) == std::string::npos)
      getline(SDFFile, buffer, '\n');

    for (int i = 0; i < skipCol; i++)
      SDFFile >> buffer;

    SDFFile >> atomCount; //gets the total number of atoms from sdf file
    SDFFile >> bondCount; //gets the total number of bonds from sdf file

    if (!atomCount)
      throw("parseSDF Error: atomCount not correctly set (maybe header length is incorrect).");
    elementList = new Atom[atomCount]; //dynamically create a list of atoms based on the number of atoms in listed in mol file
    bondNum = new int[bondCount][3]; //dynamically create 2D array where each from row represents a different bond ([rows][columns])
    while (buffer.find(target) == std::string::npos)
      getline(SDFFile, buffer, '\n');

    for (int i = 0; i < atomCount; i++)
    {
      for (int j = 0; j < skipCol; j++)
        SDFFile >> buffer;


      SDFFile >> elementList[i].elementName;
      SDFFile >> elementList[i].x;
      SDFFile >> elementList[i].y;
      SDFFile >> elementList[i].z;
      getline(SDFFile, buffer, '\n');
    }

    while (buffer.find(target) == std::string::npos)
      getline(SDFFile, buffer, '\n');

    for (int i = 0; i < bondCount; i++)
    {
      for (int j = 0; j < skipCol; j++)
        SDFFile >> buffer;

      SDFFile >> bondNum[i][2];
      SDFFile >> bondNum[i][0];
      bondNum[i][0]--;
      SDFFile >> bondNum[i][1];
      bondNum[i][1]--;

    }
    target = "ID";
    while (buffer.find(target) == std::string::npos)
      getline(SDFFile, buffer, '\n');

    SDFFile >> id;

    target = "Energy";
    while (buffer.find(target) == std::string::npos)
      getline(SDFFile, buffer, '\n');
    SDFFile >> bindingEnergy;
    SDFFile.close();
}

Atom* ChemParse::getAtomList() //returns list of atoms for use in other program
{
  if (!atomCount) //there have to be atoms in elementList or error is thrown
    throw("getAtomListError: No atoms currently available. Try parsing data."); //not really sure how to handle errors in C++
  return elementList;
}

int ChemParse::getAtomCount() const //returns number of atoms from mol file (if mole file has not been read, the count is zero)
{
  return atomCount;
}

int ChemParse::getBondCount() const //returns number of bond from mol file (if mole file has not been read, the count is zero)
{
  return bondCount;
}

void ChemParse::setDirectory(std::string dirName) //gives user option to set a new directory name
{
  directory = dirName;
}

void ChemParse::setFileName(std::string fileName) //gives user option to set different mol files
{
  filename = fileName;
}
void ChemParse::setHeaderLength(int hlen)
{
  headerLength = hlen;
}

std::string ChemParse::getDirectoryName() const //returns name of directory ChemParse object is set to
{
  return directory;
}

std::string ChemParse::getFileName() const //returns name of file ChemParse object is set to
{
  return filename;
}

double ChemParse::getBindingEnergy() const
{
  return bindingEnergy;
}

void ChemParse::displayMoleculeInfo()
{
  if (!atomCount) //there has to be atoms in elementList or error is thrown
    throw("displayMoleculeInfoError: No atoms to display. Try parsing file first");
  else
  {
    std::cout << '\n' << directory << filename << "\n\n";
    std::cout << "ID: " << id << "\n\n";
    std::cout << "Atom Count: " << atomCount << '\n';
    std::cout << "Bond Count: " << bondCount << "\n\n";
    for (int i = 0; i < atomCount; i++)
      std::cout << elementList[i].getX() << ' ' << elementList[i].getY() << ' ' << elementList[i].getZ() << ' ' << elementList[i].getElemName() << '\n';
    std::cout << "\nBinding Energy: " << bindingEnergy;
    std::cout << "\n\n";

  }
}
