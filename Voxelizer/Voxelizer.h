#ifndef VOXELIZER_H_NAP
#define VOXELIZER_H_NAP

#include "../MolParse/MolParse.h"
#include "Voxel.h"
#include <fstream>
#include <vector>

class Voxelizer
{
  private:
	  std::vector<std::vector<std::vector<Voxel>>> grid; //3 dimensional vector of voxels for the grid
	  MolParse molecule;
	  uint32_t numOfVoxels; //grid dimensions determined by numOfVoxels, i.e numOfVoxels length, width, height in grid since cube
	  double voxelSize;   //will be length width and height of each voxel
	  double x_transform, y_transform, z_transform; //transform entire molecule onto pos xy plane, 1st octet
    bool voxelized_flag;

	  void setGrid(const Atom * const &, uint32_t); //finds transform to move all points into octet 1 (x,y,z > 0), and cube dimensions in numOfVoxels
	  void populateGrid(const Atom * const &, uint32_t); //will go through and populate protons, neutrons and electrons

  public:
	  Voxelizer(); //empty grid, will need to provide molParse object to voxelize later with Voxelize function
	  Voxelizer(const Voxelizer &); //object will be voxelized if copying object is voxelized
	  Voxelizer(const MolParse &, uint32_t = 0.5); //automatically starts the grid with 0.5 voxelSize... RECOMMENDED

	  void setVoxelSize(double v_size); //set dimensions for cubic voxel in angstrums
    void setMolecule(const MolParse &); //set molparse object associated with voxel Grid

	  uint32_t getDimensions(); //returns dimensions of voxel Grid in number of voxels per dimemsion.. i.e numOfVoxels x numOfVoxels x numOfVoxels,.. max lenght width and height of grid
	  double getVoxelSize(); //returns length of one voxel side in angstrums,.. note: voxel is a cube in space, volume = voxelSize^3
	  double getXTranform(); //actual x of anything is (x - transform)
	  double getYTransform(); //actual point of anything is (y - transform)
	  double getZTransform(); //actual point of anything is (z - transform)

    void voxelize(); //allocate, populate grid with protons, neutrons and electrons... exporting to json is seperate because we may want to track other interactions within the voxels

    
};

#endif // !VOXELIZER_H_NAP
