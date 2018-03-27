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

Voxelizer::Voxelizer(const MolParse &m, uint32_t v_size)
{
	voxelSize = v_size;
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
	setGrid(molecule.getAtomList(), molecule.getAtomCount()); //set x,y,z transforms and numOfVoxels
	populateGrid(molecule.getAtomList(), molecule.getAtomCount()); //this is where voxel proton, neutron and elctron counts will be incremented
}

uint32_t Voxelizer::getDimensions() { return numOfVoxels; }

double Voxelizer::getVoxelSize() { return voxelSize; }

double Voxelizer::getXTranform() { return x_transform; }

double Voxelizer::getYTransform() { return y_transform; }

double Voxelizer::getZTransform() { return z_transform; }

//------------------------------------------------------------------------------
//private

void Voxelizer::setGrid(Atom * const &a, uint32_t count)
{
	std::ifstream e_cloud; //file handle to get points of electrons per atom
	double min_x, min_y, min_z, max_x, max_y, max_z; //keep track of smallest x and y in graph
	min_x = min_y = min_z = max_x = max_y = max_z = 0.0;

	for (uint32_t i = 0; i < count; ++i)
	{
		//get min x,y,z from atom
		if (a[i].getX() < min_x)
			min_x = a[i].getX();

		if (a[i].getY() < min_y)
			min_y = a[i].getY();

		if (a[i].getZ() < min_z)
			min_z = a[i].getZ();

		//get max x,y,z from atom
		if (a[i].getX() > max_x)
			max_x = a[i].getX();

		if (a[i].getY() > max_y)
			max_y = a[i].getY();

		if (a[i].getZ() > max_z)
			max_z = a[i].getZ();

		std::string eCloudPath = "../ElectronClouds/" + a[i].getElemName() + ".txt";	//Parent/ElectronClouds/ELEMENT_SYMBOL.txt
		e_cloud.open(eCloudPath); //open file containing relative electron points

		//get min  and max x,y,z from electron cloud points
		if (e_cloud.good())
		{
			while (!e_cloud.eof())
			{
				double temp_x, temp_y, temp_z; //hold x,y,z of current electron... electron position is relative to atom
				char c; //take out commas

				e_cloud >> temp_x;
				e_cloud >> c;
				e_cloud >> temp_y;	//take x,y,z from csv format
				e_cloud >> c;
				e_cloud >> temp_z;

				temp_x += a[i].getX(); //actual x coordinate of electron in relation to atom
				if (temp_x < min_x)
					min_x = temp_x;
				if (temp_x > max_x)
					max_x = temp_x;

				temp_y += a[i].getY(); //actual y coordinate of electron in relation to atom
				if (temp_y < min_y)
					min_y = temp_y;
				if (temp_y > max_y)
					max_y = temp_y;

				temp_z += a[i].getZ(); //actual z coordinate of electron in relation to atom
				if (temp_z < min_z)
					min_z = temp_z;
				if (temp_z > max_z)
					max_z = temp_z;
			}
		}
	}

	x_transform = -min_x;
	y_transform = -min_y;
	z_transform = -min_z;

	//number of voxels for cube will be max distance in any x rounded up, then divided by voxel size
	double maxDistance = max_x - min_x;
	if (maxDistance < (max_y - min_y))
		maxDistance = max_y - min_y;
	if (maxDistance < (max_z - min_z))
		maxDistance = max_z - min_z;

	numOfVoxels = uint32_t(maxDistance / voxelSize + 1); //add one to round up
}

void Voxelizer::populateGrid(Atom * const &a, uint32_t count)
{

}
