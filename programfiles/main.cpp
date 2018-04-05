#include "../Voxelizer/Voxelizer.h"

//takes in a molFile path with filename and a voxelSize in angstrums, Voxelizes it and outputs to json
int main(int argc, char** argv)
{
  std::string molFile;
  double voxelSize;

  if(argc == 1) //user is running program without passing command line argument
  {
    //get the molFile path with filename from user
    std::cout << "Specify the path and file name to a molfile you would like to represent.\n";
    std::cin >> molFile;

    //get the voxel size from user
    std::cout << "Specify the Voxel Size in angstrums.\n";
    std::cin >> voxelSize;
  }
  else if(argc == 3) //user is running with command line arguments
  {
    molFile = argv[1];
    voxelSize = std::atof(argv[2]);
  }
  else
  {
    throw "main - Too many command line arguments given to voxelizer program.";
  }

  //by this point we have what was given by the user and now need to verify data integrity
  //get directory and filename form the user
  std::size_t posOfLastSlash = molFile.find_last_of('/');
  std::string directory, fileName;

  //check if a directory is specified
  if(posOfLastSlash != std::string::npos)
  {
    directory = molFile.substr(0, posOfLastSlash);
    fileName = molFile.substr(posOfLastSlash+1, molFile.size() - posOfLastSlash);
  }
  else
  {
    throw "main.cpp - No specified directory for molfile";
  }

  //check integrity of getVoxelSize
  if(voxelSize <= 0)
  {
    throw "main - Voxel size must be positve.";
  }
  //std::cout << "Direcotry: " << directory << " Filename: " << fileName << std::endl;
  /*
  Voxelizer v;

  // directories are based upon where the compiler is being called from
  MolParse m("MolParse/mol");
  m.parseData();
  m.displayMoleculeInfo();


  v.setMolecule(m);

  try{
    v.voxelize();
  }
  catch(const char *e){
    std::cout << e << '\n';
  }

  std::cout << "Voxel grid dimensions: " << v.getDimensions() << '\n';

  try{
    v.exportJSON();
  }
  catch(const char *e){
    std::cout << e << '\n';
  }

  std::string firstLine;
  std::string path = "ElectronClouds/C.txt";
  std::ifstream in;
  in.open(path.c_str());
  if(in.good())
  {
    in >> firstLine;
    std::cout << firstLine << std::endl;
  }

  return 0;*/
}
