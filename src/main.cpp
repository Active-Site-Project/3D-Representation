#include "Voxelizer.h"

void createActiveSite(const std::string path,const double voxelSize); //creates a new active site
void addToAnActiveSite(const std::string path); //adds the specified molfile to the specified active site
char getUserChoice(); //returns user choice to add to existing active site

//takes in a molFile path with filename and a voxelSize in angstrums, Voxelizes it and outputs to json...also can take a json file of an active site and a new molFile path to add to it
int main(int argc, char** argv)
{
  std::string path;
  double voxelSize = 0.0;
  char addingToExisting = 0;

  if(argc == 1) //user is running program without passing command line argument
  {
    addingToExisting = getUserChoice(); //find out if there is a new or existing active site

    if(addingToExisting == 'y' || addingToExisting == 'Y') {    //existing active site
      std::string temp;
      std::cout << "Specify the path and file name to an active-site json file you would like to add to.\n";
      std::cin >> temp;

      path += (temp + " "); //"active-site " active site path followed by space seperator

      std::cout << "Specify the path and file name to a molFile that you would like to add to the active site. \nNote: Acitve site should be larger than added molecule.\n";
      std::cin >> temp;  //add on molFile path

      path += temp; //"active-site molFile"
    }
    else {  //new active site
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
    path = argv[1]; //path to active site or molFile path

    if(path.find(".json") != std::string::npos) {  //.json found..so argv[1] = active site path and argv[2] = molFile path
      addingToExisting = 'y'; //adding to an existing molfile
      path += " ";
      path += argv[2]; //path now holds "active-site-path molFile-path" with space in the middle for seperator
    }
    else if(path.find(".mol") != std::string::npos) {  //.mol found
      addingToExisting = 'n'; //adding to an existing molfile
      voxelSize = std::atof(argv[2]);
    }
  }
  else {
    std::cout << "error in main - Too many command line arguments given to voxelizer program.\n";
    return 0;
  }

  //path at this point either is the molFile path, or then active site path, a space, and then the molFile path
  if(addingToExisting == 'n' || addingToExisting == 'N')   //new active-site
    createActiveSite(path, voxelSize);
  else //adding to existing, and we are sure addingToExisting == either y || n or their respective capitals
    addToAnActiveSite(path);

  return 0;
}






char getUserChoice()
{
  char c;
  //find out if we are adding to an active-site or a new active-site
  do {
    std::cout << "Are you adding to an existing Active-Site? (y or n)\n";
    std::cin >> c;
  } while(c != 'y' && c != 'Y' &&  //continue looping while it doesnt equal any of the following
          c != 'n' && c != 'N');

  return c;
}

void createActiveSite(const std::string path, const double voxelSize)
{
  //by this point we have what was given by the user and now need to verify data integrity
  //get directory and filename form the user
  std::size_t posOfLastSlash = path.find_last_of('/');
  std::string directory, fileName;

  //check if a directory is specified
  if(posOfLastSlash != std::string::npos) {
    directory = path.substr(0, posOfLastSlash+1);
    fileName = path.substr(posOfLastSlash+1, path.size() - posOfLastSlash);
    //std::cout << "Direcotry: " << directory << " Filename: " << fileName << std::endl;
  }
  else {
    std::cout << "error in main - No specified directory for molfile" << '\n';
    return;
  }

  //at this point we should have some positive value for voxelSize and a directory for the molFile
  MolParse m(directory, fileName);
  try{ m.parseData(); } //this will throw error if cannot access molFile
  catch(const char *e) {
    std::cout << e << '\n';
    return;
  }

  //Now we have a molParse object with validated molFile and we have a validated voxel size, next we need a Voxelizer
  Voxelizer v(m, voxelSize);

  try{ v.voxelize(); } //here is where all the elements in the molFile and their respective electron clouds update the voxels, calls setUpGrid and populateGrid
  catch(const char *e) {
    std::cout << e << '\n';
    return;
  }

  v.exportJSON();
}

void addToAnActiveSite(const std::string path)
{

}
