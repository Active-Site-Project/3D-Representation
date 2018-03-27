#include "Voxelizer.h"

//------------------------------------------------------------------------------
//public

//good
Voxelizer::Voxelizer()
{
	numOfVoxels = 0;
	voxelSize = 0.5;
	x_transform = y_transform = z_transform = 0.0;
	voxelized_flag = false;
}

//good, do not set transforms and other stuff until system for avoiding setUpGrid is thoroughly though out
Voxelizer::Voxelizer(const Voxelizer &v)
{
	numOfVoxels = 0;
	voxelSize = v.voxelSize;
	x_transform = 0.0;
	y_transform = 0.0;
	z_transform = 0.0;
	molecule = v.molecule;
	voxelized_flag = false;
}

//good
Voxelizer::Voxelizer(const MolParse &m, uint32_t v_size)
{
	voxelSize = v_size;
	numOfVoxels = 0;
	x_transform = y_transform = z_transform = 0.0;
	molecule = m;
	voxelized_flag = false;
}

//in progress
void Voxelizer::setVoxelSize(double v_size)
{
  if(v_size <= 0)
	  throw std::string("Voxel size must be greater than 0.\n");

	//programmer must revoxelize after resizing, later will add ability to not need setUpGrid again
	//only will need to repopulate
	if (grid.size() != 0)
	{
		grid.clear();
    voxelSize = v_size;
		numOfVoxels = 0;
		x_transform = y_transform = z_transform = 0.0;
		voxelized_flag = false;
	}
	else
		voxelSize = v_size;
}

//done
void Voxelizer::setMolecule(const MolParse &m) { molecule = m; }

//in progress
void Voxelizer::voxelize()
{
	if(voxelized_flag == true) //do nothing
	  return;

  //will only execute if file contains 0 molecules or still user hasnt provided new file
	if(molecule.getAtomCount() == 0)
		throw std::string("File contains 0 atoms.\n");

	setGrid(molecule.getAtomList(), molecule.getAtomCount()); //set x,y,z transforms and numOfVoxels

  if(numOfVoxels != 0)
	{
    std::vector<Voxel> lv1(numOfVoxels);
		std::vector<std::vector<Voxel>> lv2(numOfVoxels,lv1);
	  grid.resize(numOfVoxels, lv2);
	}

  populateGrid(molecule.getAtomList(), molecule.getAtomCount()); //this is where voxel proton, neutron and elctron counts will be incremented
	voxelized_flag = true;
}

//done
uint32_t Voxelizer::getDimensions() { return numOfVoxels; }

//done
double Voxelizer::getVoxelSize() { return voxelSize; }

//done
double Voxelizer::getXTranform() { return x_transform; }

//working
double Voxelizer::getYTransform() { return y_transform; }

//working
double Voxelizer::getZTransform() { return z_transform; }

//------------------------------------------------------------------------------
//private

//working
void Voxelizer::setGrid(const Atom * const &a, uint32_t count)
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

  //either there is or isnt a transform
	x_transform = (min_x < 0)? -min_x: 0;
	y_transform = (min_y < 0)? -min_y: 0;
	z_transform = (min_z < 0)? -min_z: 0;

	//number of voxels for cube will be max distance in any x rounded up, then divided by voxel size
	double maxDistance = max_x - min_x;
	if (maxDistance < (max_y - min_y))
		maxDistance = max_y - min_y;
	if (maxDistance < (max_z - min_z))
		maxDistance = max_z - min_z;

	numOfVoxels = uint32_t(maxDistance / voxelSize + 1); //add one to round up
}

//in progress
void Voxelizer::populateGrid(const Atom * const &a, uint32_t count)
{
	std::ifstream e_cloud;

  for(uint32_t i = 0; i < count ; ++i)
	{
		//get indecis for voxel placement, may be wrong at the current moment... truncation is not a problem
		int index_x = (a[i].getX() + x_transform) / voxelSize;
		int index_y = (a[i].getY() + y_transform) / voxelSize;
		int index_z = (a[i].getZ() + z_transform) / voxelSize;

		grid[index_x][index_y][index_z].addProton(); //later will replace with different means
		grid[index_x][index_y][index_z].addNeutron(); //representing nucleus

    //open electron cloud for each atom
		std::string eCloudPath = "../ElectronClouds/" + a[i].getElemName() + ".txt";	//Parent/ElectronClouds/ELEMENT_SYMBOL.txt
		e_cloud.open(eCloudPath.c_str());

    if(e_cloud.good())
		{
			while(!e_cloud.eof())
			{
				double temp_x = 0.0, temp_y = 0.0, temp_z = 0.0; //hold electron points
				char c; //take out commas from csv format

				e_cloud >> temp_x;
				e_cloud >> c;
				e_cloud >> temp_y;  //take out points in relation to nucleus
				e_cloud >> c;
				e_cloud >> temp_z;

				temp_x += (a[i].getX() + x_transform);
				temp_y += (a[i].getY() + y_transform); //actual point of electron with transform
				temp_z += (a[i].getZ() + z_transform);

				//corresponding indecis or place against wall
				index_x = (temp_x < 0)? 0: temp_x / voxelSize;
				index_y = (temp_y < 0)? 0: temp_y / voxelSize;
				index_z = (temp_z < 0)? 0: temp_z / voxelSize;

				grid[index_x][index_y][index_z].addElectron();
			}
		}
	}
}
