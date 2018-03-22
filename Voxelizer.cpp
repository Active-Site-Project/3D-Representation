#include "Voxelizer.h"

//------------------------------------------------------------------------------
//public

Voxelizer::Voxelizer()
{
	numOfVoxels = 0;
	voxelSize = 0.5;
	x_transform = y_transform = z_transform = 0.0;
}

Voxelizer::Voxelizer(const Voxelizer &v)
{
	numOfVoxels = v.numOfVoxels;
	voxelSize = v.voxelSize;
	x_transform = v.x_transform;
	y_transform = v.y_transform;
	z_transform = v.z_transform;
	molecule = v.molecule;
}

Voxelizer::Voxelizer(const MolParse &m, uint32_t)
{
	voxelSize = 0.5;
	numOfVoxels = 0;
	x_transform = y_transform = z_transform = 0.0;
	molecule = m;
	voxelize(molecule);
}

void Voxelizer::setVoxelSize(double v_size) 
{ 
	//must repopulate grid based on resize
	if (grid.size() != 0)
	{

	}
	else
		voxelSize = v_size; 
}

void Voxelizer::voxelize(const MolParse &m)
{
	molecule = m;
	setTransform(molecule.getAtomList(), molecule.getAtomCount()); //set x,y,z transforms
	setGridDimensions(molecule.getAtomList(), molecule.getAtomCount()); //set numOfVoxels and allocate vector grid
	populateGrid(molecule.getAtomList(), molecule.getAtomCount()); //this is where voxel proton, neutron and elctron counts will be incremented
}

uint32_t Voxelizer::getDimensions() { return numOfVoxels; }

double Voxelizer::getVoxelSize() { return voxelSize; }

double Voxelizer::getXTranform() { return x_transform; }

double Voxelizer::getYTransform() { return y_transform; }

double Voxelizer::getZTransform() { return z_transform; }

//------------------------------------------------------------------------------
//private

void Voxelizer::setTransform(const Atom * const &a, uint32_t count)
{
	double min_x, min_y, min_z; //keep track of smallest x and y in graph
	min_x = min_y = min_z = 0.0;

	for (uint32_t i = 0; i < count; ++i)
	{

	}
}

void Voxelizer::setGridDimensions(const Atom * const &a, uint32_t)
{

}

void Voxelizer::populateGrid(const Atom * const &a, uint32_t)
{

}