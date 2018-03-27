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

	  void setGrid(Atom * const &, uint32_t); //finds transform to move all points into octet 1 (x,y,z > 0), and cube dimensions in numOfVoxels
	  void populateGrid(Atom * const &, uint32_t); //will go through and populate protons, neutrons and electrons

  public:
	  Voxelizer(); //empty grid, will need to provide molParse object to voxelize later with Voxelize function
	  Voxelizer(const Voxelizer &); //object will be voxelized if copying object is voxelized
	  Voxelizer(const MolParse &, uint32_t = 0.5); //automatically starts the grid with 0.5 voxelSize... RECOMMENDED

	  void setVoxelSize(double v_size);
    void setMolecule(const MolParse &);

	  uint32_t getDimensions();
	  double getVoxelSize();
	  double getXTranform();
	  double getYTransform();
	  double getZTransform();

    void voxelize();
};

#endif // !VOXELIZER_H_NAP
