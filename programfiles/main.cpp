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
    std::cout << "error in main - Too many command line arguments given to voxelizer program.\n";
    return 0;
  }

  //by this point we have what was given by the user and now need to verify data integrity
  //get directory and filename form the user
  std::size_t posOfLastSlash = molFile.find_last_of('/');
  std::string directory, fileName;

  //check if a directory is specified
  if(posOfLastSlash != std::string::npos)
  {
    directory = molFile.substr(0, posOfLastSlash+1);
    fileName = molFile.substr(posOfLastSlash+1, molFile.size() - posOfLastSlash);
    //std::cout << "Direcotry: " << directory << " Filename: " << fileName << std::endl;
  }
  else
  {
    std::cout << "error in main - No specified directory for molfile" << '\n';
    return 0;
  }

  //check integrity of getVoxelSize
  if(voxelSize <= 0)
  {
    std::cout << "error in main - Voxel size must be positve.\n";
    return 0; //later will be error codes associated with error
  }

  //at this point we should have some positive value for voxelSize and a directory for the molFile
  MolParse m(directory, fileName);
  try{
    m.parseData(); //this will throw error if cannot access molFile
  }
  catch(const char *e){
    std::cout << e << '\n';
    return 0;
  }

  //Now we have a molParse object with validated molFile and we have a validated voxel size, next we need a Voxelizer
  Voxelizer v(m, voxelSize);

  try{
    v.voxelize(); //here is where all the elements in the molFile and their respective electron clouds update the voxels, calls setUpGrid and populateGrid
  }
  catch(const char *e){
    std::cout << e << '\n';
    return 0;
  }

  v.exportJSON();

  return 0;
}
