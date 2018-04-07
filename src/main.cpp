#include "Voxelizer.h"

void createActiveSite(const std::string path,const double voxelSize);
void addToActiveSite(const std::string path);

//takes in a molFile path with filename and a voxelSize in angstrums, Voxelizes it and outputs to json...also can take a json file of an active site and a new molFile path to add to it
int main(int argc, char** argv)
{
  std::string path;
  double voxelSize = 0.0;
  char addingToExisting = 0;

  if(argc == 1) //user is running program without passing command line argument
  {
    //find out if we are adding to an active-site or a new active-site
    do
    {
      std::cout << "Are you adding to an existing Active-Site? (y or n)\n";
      std::cin >> addingToExisting;
    } while(addingToExisting != 'y' && addingToExisting != 'Y' &&  //continue looping while it doesnt equal any of the following
            addingToExisting != 'n' && addingToExisting != 'N');

    //existing active site
    if(addingToExisting == 'y' || addingToExisting == 'Y')
    {
      std::cout << "Specify the path and file name to an active-site json file you would like to add to.\n";
      std::cin >> path;
    }
    else //new active site
    {
      //get the molFile path with filename from user
      std::cout << "Specify the path and file name to a molfile you would like to represent.\n";
      std::cin >> path;

      //get the voxel size from user
      std::cout << "Specify the Voxel Size in angstrums.\n";
      std::cin >> voxelSize;
    }
  }
  else if(argc == 3) //user is running with command line arguments, need to get file extension and check if json or mol
  {
    path = argv[1];

    if(path.find(".json") != std::string::npos) //.json found..so argv[1] = active site path and argv[2] = molFile path
    {
      addingToExisting = 'y'; //adding to an existing molfile
      path += " "; //path now holds "molFile-path active-site-path" with space in the middle for seperator
      path += argv[2];
    }
    else if(path.find(".mol") != std::string::npos) //.mol found
    {
      addingToExisting = 'n'; //adding to an existing molfile
      voxelSize = std::atof(argv[2]);
    }
  }
  else
  {
    std::cout << "error in main - Too many command line arguments given to voxelizer program.\n";
    return 0;
  }

  //new active-site
  if(addingToExisting == 'n' || addingToExisting == 'N')
  {
    createActiveSite(path, voxelSize);
  }
  else //adding to existing, and we are sure addingToExisting == either y || n or their respective capitals
  {
    addToActiveSite(path);
  }

  return 0;
}

void createActiveSite(const std::string path, const double voxelSize)
{
  //by this point we have what was given by the user and now need to verify data integrity
  //get directory and filename form the user
  std::size_t posOfLastSlash = path.find_last_of('/');
  std::string directory, fileName;

  //check if a directory is specified
  if(posOfLastSlash != std::string::npos)
  {
    directory = path.substr(0, posOfLastSlash+1);
    fileName = path.substr(posOfLastSlash+1, path.size() - posOfLastSlash);
    //std::cout << "Direcotry: " << directory << " Filename: " << fileName << std::endl;
  }
  else
  {
    std::cout << "error in main - No specified directory for molfile" << '\n';
    return;
  }

  //check integrity of getVoxelSize
  if(voxelSize <= 0)
  {
    std::cout << "error in main - Voxel size must be positve.\n";
    return; //later will be error codes associated with error
  }

  //at this point we should have some positive value for voxelSize and a directory for the molFile
  MolParse m(directory, fileName);
  try{
    m.parseData(); //this will throw error if cannot access molFile
  }
  catch(const char *e){
    std::cout << e << '\n';
    return;
  }

  //Now we have a molParse object with validated molFile and we have a validated voxel size, next we need a Voxelizer
  Voxelizer v(m, voxelSize);

  try{
    v.voxelize(); //here is where all the elements in the molFile and their respective electron clouds update the voxels, calls setUpGrid and populateGrid
  }
  catch(const char *e){
    std::cout << e << '\n';
    return;
  }

  v.exportJSON();
}

void addToActiveSite(const std::string path)
{

}
