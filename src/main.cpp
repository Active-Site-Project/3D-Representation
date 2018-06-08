#include "Voxelizer.h"

void createMoleculeSpace(const std::string path,const double voxelSize, const uint32_t numOfVoxels, const std::string outFile); //creates a new active site
void molWithMolecule(const std::string path, const std::string outFile); //add mol file to molecule space
void moleculeWithMolecule(const std::string path, const std::string outFile); //combines 2 molecule spaces
double getVoxelSize();
uint32_t getGridDimensions();
std::string getMolFilePath();
std::string getMoleculePath();
std::string getOutputFile();

//takes in a molFile path with filename and a voxelSize in angstrums, Voxelizes it and outputs to json...also can take a json file of an active site and a new molFile path to add to it
int main(int argc, char** argv)
{
  std::string path, outFile;
  double voxelSize = 0.0;
  uint32_t voxels = 0;
  char addingToExisting = 0, combiningSpaces = 0; //used to tell if creating new space, space with mol or sapce with space

  if(argc == 1) //user is running program without passing command line argument
  {
    int choice = 0;
    bool done = false;

    while(!done)
    {
      std::cout << "1. Create a new molecule space\n";
      std::cout << "2. Add a mol file to a molecule space.\n";
      std::cout << "3. Combine 2 molecule spaces. Larger one first.\n\n";
      std::cout << "How would you like to use the Voxelizer? Enter 1-3 as your selection.\n";

      std::cin >> choice;

      switch(choice)
      {
        case 1:
          path = getMolFilePath();  //get the molFile path with filename from user
          voxelSize = getVoxelSize(); //get the voxel size from user
          outFile = getOutputFile();
          voxels = getGridDimensions();
          done = true;
          break;

        case 2: //path = "molecule mol"
          path = getMoleculePath();
          path += " ";
          path += getMolFilePath();
          outFile = getOutputFile();
          addingToExisting = 'y';
          done = true;
          break;

        case 3: //path = "molecule molecule"
          path = getMoleculePath();
          path += " ";
          path += getMoleculePath();
          outFile = getOutputFile();
          combiningSpaces = 'y';
          done = true;
          break;
        default:
          break;
      }
    }
  }
  else if(argc == 5 || argc == 4) //user is running with command line arguments, need to get file extension and check if json or mol
  {
    path = argv[1]; //path to active site or molFile path
    outFile = argv[3];

    if(path.find(".json") != std::string::npos) {  //.json found..so argv[1] = active site path and argv[2] = molFile path
      if(std::string(argv[2]).find(".json") != std::string::npos) { //second file is molecule space, so combining 2 molecule spaces
        combiningSpaces = 'y'; //combining 2 molecule spaces
      }
      else {
        addingToExisting = 'y'; //adding to an existing molecule space;
      }

      path += " ";
      path += argv[2]; //path now holds "active-site-path molFile-path" or  "active-site-path active-site-path" with space in the middle for seperator
    }
    else if(path.find(".mol") != std::string::npos || path.find(".sdf") != std::string::npos) {  //.mol found
      voxelSize = std::atof(argv[2]);
      voxels = std::atoi(argv[4]); //will be 4th argument, molfile voxelSize outFile numOfVoxels... might need error checking later
    }
  }
  else
  {
    std::cout << "error in main - Too many or too few command line arguments given to voxelizer program.\n";
    return 0;
  }

  //path at this point either is the molFile path, or then active site path, a space, and then the molFile path
  if(addingToExisting == 'y' || addingToExisting == 'Y')
    molWithMolecule(path, outFile); //combine molfile to a molecule space
  else if(combiningSpaces == 'y' || combiningSpaces == 'Y')  //new active-sitengSpaces == 'y' || combiningSpaces == 'Y')
    moleculeWithMolecule(path, outFile); //combine 2 molecule spaces
  else
    createMoleculeSpace(path, voxelSize, voxels, outFile); //new active-site
  return 0;
}

//------------------------------------------------------------------------------
//functions

double getVoxelSize()
{
  double size;
  std::cout << "Specify the Voxel Size in angstrums.\n";
  std::cin >> size;
  return size;
}

uint32_t getGridDimensions()
{
  uint32_t voxels;
  std::cout << "Specify the dimensions necessary to encapsulate this molecule space.\n";
  std::cin >> voxels;
  return voxels;
}

std::string getMolFilePath() //path to molFile
{
  std::string rtrn;
  std::cout << "Specify the path and file name to a molfile you would like to represent.\n";
  std::cin >> rtrn;
  return rtrn;
}

std::string getMoleculePath() //path to molecule space
{
  std::string rtrn;
  std::cout << "Specify the path and file name to a molecule space you would like to add t.\n";
  std::cin >> rtrn;
  return rtrn;
}

std::string getOutputFile()
{
  std::string rtrn;
  std::cout << "Specify the path and file name for the outputted molecule space.\n";
  std::cin >> rtrn;
  return rtrn;
}

void createMoleculeSpace(const std::string path, const double voxelSize, const uint32_t numOfVoxels, const std::string outFile)
{
  //by this point we have what was given by the user and now need to verify data integrity
  //get directory and filename form the user
  std::size_t posOfLastSlash = path.find_last_of('/');
  std::string directory, fileName;
  Voxelizer v;

  if(posOfLastSlash != std::string::npos) {  //check if a directory is specified, later might not need
    directory = path.substr(0, posOfLastSlash+1);
    fileName = path.substr(posOfLastSlash+1, path.size() - posOfLastSlash);
  }
  else {
    std::cout << "error in main - No specified directory for molfile" << '\n';
    return;
  }

  //at this point we should have some positive value for voxelSize and a directory for the molFile
  ChemParse m(directory, fileName);
  try{
    m.parseData();  //this will throw error if cannot access molFile
    v.setMolecule(m); //at this point we have valid molFile data if no errors thrown
    v.setVoxelSize(voxelSize); //will throw error if <= 0
    v.setDimensions(numOfVoxels); //get voxel dimensions from user, later might be constant for network
    v.voxelize();
  }
  catch(const char *e) {
    std::cout << e << '\n';
    return;
  }

  v.exportJSON(outFile);
}

void molWithMolecule(const std::string path, const std::string outFile) //path contains "active-site molFile"
{
  std::string activeSitePath, molFilePath, molFileDirectory, molFileName;
  Voxelizer v; //Voxelizer object to hold active site and new molecule
  std::size_t spacePosition = path.find(" "); //no need to check for string::npos because space character is guranteed

  //split path into activeSite and molFile
  activeSitePath = path.substr(0, spacePosition);
  molFilePath = path.substr(spacePosition+1, path.size() - spacePosition);

  //use to split path into a directory and filename
  std::size_t posOfLastSlash = molFilePath.find_last_of('/');

  if(posOfLastSlash != std::string::npos) {  //check if a directory is specified, later might not need
    molFileDirectory = molFilePath.substr(0, posOfLastSlash+1);
    molFileName = molFilePath.substr(posOfLastSlash+1, path.size() - posOfLastSlash);
  }
  else {
    std::cout << "error in main - No specified directory for molfile" << '\n';
    return;
  }

  ChemParse m(molFileDirectory, molFileName); //by now we have directories and will use a try catch in case user gave bad info

  try {
    v.readActiveSite(activeSitePath); //open the existing active site
    m.parseData();
    v.setMolecule(m);
    v.voxelize();
    v.exportJSON(outFile);
  }
  catch(const char *e){
    std::cout << e << '\n';
    return;
  }
}

void moleculeWithMolecule(const std::string  path, const std::string outFile) //path contains 2 molcule spaces
{
  std::string space1Path, space2Path;
  Voxelizer v; //Voxelizer object to hold active site and new molecule
  std::size_t spacePosition = path.find(" "); //no need to check for string::npos because space character is guranteed

  //split path into activeSite and molFile
  space1Path = path.substr(0, spacePosition);
  space2Path = path.substr(spacePosition+1, path.size() - spacePosition);

  std::cout << space1Path << "\t" << space2Path << '\n';

  try
  {
    v.readActiveSite(space1Path);
    v.readActiveSite(space2Path);
    v.exportJSON(outFile);
  }
  catch(const char *e)
  {
    std::cout << e << '\n';
    return;
  }
}
