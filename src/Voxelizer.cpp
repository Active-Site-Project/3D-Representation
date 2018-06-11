#include "Voxelizer.h"

//------------------------------------------------------------------------------
//public

Voxelizer::Voxelizer()
{
	numOfVoxels = 0;
	voxelSize = 0.5;
	x_transform = y_transform = z_transform = 0.0;
	voxelized_flag = site_active = false;
	bindingEnergy = 0;
}

//note-to-self: do not set transforms and other stuff until system for avoiding setUpGrid is thoroughly though out
Voxelizer::Voxelizer(const Voxelizer &v)
{
	numOfVoxels = 0;
	voxelSize = v.voxelSize;
	x_transform = 0.0;
	y_transform = 0.0;
	z_transform = 0.0;
	molecule = v.molecule;
	voxelized_flag = site_active = false;
	bindingEnergy = 0;
}

//for now can only copy an active site from a molFile, not from an existing active site
Voxelizer::Voxelizer(const ChemParse &m, double v_size, uint32_t gridDimensions)
{
	voxelSize = v_size;
	numOfVoxels = gridDimensions;
	x_transform = y_transform = z_transform = 0.0;
	molecule = m;
	voxelized_flag = site_active = false;
	bindingEnergy = m.getBindingEnergy();
}

//set Voxel dimensions
void Voxelizer::setVoxelSize(double v_size)
{
  if(v_size <= 0)
	  throw "Voxel size must be greater than 0.";

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

//set ChemParse object associated
void Voxelizer::setMolecule(const ChemParse &m) { molecule = m; bindingEnergy = m.getBindingEnergy(); }

//Voxelizes a molFile into a new active site, or adds it to an existing active site
void Voxelizer::voxelize()
{
	if(voxelized_flag == true) //do nothing
	  return;

	if(voxelSize <= 0)
	  throw "Voxel size must be positve.";

	if(!numOfVoxels)
		throw "numOfVoxels must be set.";

	setGrid(molecule.getAtomList(), molecule.getAtomCount()); //set x,y,z transforms and numOfVoxels

  if(grid.size() == 0) //checks if we already have a grid
    resizeGrid(); //allocate cube with dimensions numOfVoxels

  populateGrid(molecule.getAtomList(), molecule.getAtomCount()); //this is where voxel proton, neutron and elctron counts will be incremented
	voxelized_flag = site_active = true;
}

uint32_t Voxelizer::getDimensions() { return numOfVoxels; }

double Voxelizer::getVoxelSize() { return voxelSize; }

double Voxelizer::getXTranform() { return x_transform; }

double Voxelizer::getYTransform() { return y_transform; }

double Voxelizer::getZTransform() { return z_transform; }

//sets numOfVoxels
void Voxelizer::setDimensions(uint32_t gridDimensions) { numOfVoxels = gridDimensions; }

//exports to a json style format
void Voxelizer::exportJSON(const std::string outFile)
{
	if(voxelized_flag == false && site_active == false)
	  throw "Must voxelize or have an active molecule first.";

	std::ofstream out((outFile + ".json").c_str()); //will overwrite old write file

  //output MoleculeGrid-V1.0 so that when reading we can tell that this is a MoleculeGrid-V1.0, json style, file type.
  out << "MoleculeGrid-V1.0\n";

  //output the dimensions of the VoxelGrid as well as the size of each voxel
	out << "VoxelGrid Dimensions: " << numOfVoxels << '\n';
	out << "Voxel size: " << voxelSize << '\n'; //blank space between dimensions and voxels themselves
	out << "Transforms: " << x_transform << " " << y_transform << " " << z_transform << "\n";
	out << "Binding Energy: " << bindingEnergy << "\n\n";

  //for each voxel we must write its properties
	  for(uint32_t i = 0; i < numOfVoxels; ++i)
		{
			for(uint32_t j = 0; j < numOfVoxels; ++j)
			{
				for(uint32_t k = 0; k < numOfVoxels; ++k)
				{
					if (grid[i][j][k].getElectrons())
					{
						out << "{\n\t"; //open new json object
						out << "\"xpos\": "  << i << ",\n\t";
						out << "\"ypos\": " << j << ",\n\t";
						out << "\"zpos\": " << k << ",\n\t";
						out << "\"protons\": " << grid[i][j][k].getProtons() << ",\n\t"; //protons data member
						out << "\"neutrons\": " << grid[i][j][k].getNeutrons() << ",\n\t"; //neutrons data member
						out << "\"electrons\": " << grid[i][j][k].getElectrons() << "\n"; //electrons data member
						out << "}"; //close json object

		        //if not the last voxel add a comment and newline character
						if(i != numOfVoxels - 1 || j != numOfVoxels - 1 || k != numOfVoxels - 1)
						  out << ",\n";
					}
				}
			}
		}

	out.close();
}

//takes in an acitve site, sets the transforms, number of voxels, and voxelsize... can only be called if cuurent Voxelizer object is not voxelized
void Voxelizer::readActiveSite(std::string activeSite)
{
  if(voxelized_flag != false) //if not voxelized, then grid.size() will be 0, later we can check to see if this new one fits with already voxelized, and that would make order not important
	  throw "Cannot add an active-site to an already voxelized molecule. \nRead Active site first, and then voxelize with the second smaller molecule.";

  std::ifstream in(activeSite.c_str());
	std::string s; //use to exctract data
	uint64_t tempNumVoxels;
	double tempVoxelSize, temp_x_tranform, temp_y_tranform, temp_z_tranform, tempBindingEnergy;

  if(in.good()) {
		in >> s; //read in fileType, MoleculeGrid V1.0, .mg in the future possibly
		if(s != "MoleculeGrid-V1.0")
			throw "Invalid Active Site.";

		//read in dimensions
		in >> s; in >> s; //get rid of "VoxelGrid Dimensions: "
		in >> tempNumVoxels;
		in >> s; in >> s; //get rid of "Voxel size: "
		in >> tempVoxelSize;
		in >> s; //get rid of "tranforms: "
		in >> temp_x_tranform >> temp_y_tranform >> temp_z_tranform;
		in >> s; in >> s;
		in >> tempBindingEnergy;

		if(site_active) //grid will be alive arleady with cube dimensions numOfVoxels and numOfVoxels and voxelSize set
		{
			if(tempNumVoxels != numOfVoxels) //throw error or do nothing and continue to add to the handled space
				throw "Molecule spaces must be the same size.";
			bindingEnergy = tempBindingEnergy;
		}
		else
		{
			numOfVoxels = tempNumVoxels;
			voxelSize = tempVoxelSize;
			bindingEnergy = 0; //if the active site is the only thing present, it doesn't make sense to have a non-zero binding energy
			resizeGrid(); //allocate cube with dimensions numOfVoxels
		}

		//populate grid with voxels in file
		uint64_t tempParticles;
		for(uint32_t i = 0; i < numOfVoxels; ++i) {
			for(uint32_t j = 0; j < numOfVoxels; ++j) {
				for(uint32_t k = 0; k < numOfVoxels; ++k) {
					in >> s; in >> s;//get rid of '{' , '\t' , and "protons: "
					in >> tempParticles; //get number of protons
					if(tempParticles > 0)
						grid[i][j][k].addProtons(tempParticles); //adds protons to grid if there are any

					in >> s; in >> s;//get rid of ',' , '\t' , and "neutrons: "
					in >> tempParticles; //get number of neutrons
					if(tempParticles > 0)
						grid[i][j][k].addNeutrons(tempParticles); //adds neutrons to grid if there are any

					in >> s; in >> s;//get rid of ',' , '\t' , and "electrons: "
					in >> tempParticles; //get number of electrons
					if(tempParticles > 0)
						grid[i][j][k].addElectrons(tempParticles); //adds electrons to grid if there are any

					in >> s;//get rid of "}," or only "}" if the last one
				}
			}
		}

	}
	else {
		throw "Could not open active site json file.";
	}

	in.close();
	site_active = true;
}









//------------------------------------------------------------------------------
//private

//working
void Voxelizer::setGrid(const Atom * const &a, uint32_t count)
{
	if(count == 0)
		throw "File contains 0 atoms.";

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

		std::string eCloudPath = eCloudDirectory + a[i].getElemName() + ".txt";	//Parent/ElectronClouds/ELEMENT_SYMBOL.txt
    //std::string eCloudPath = "ElectronClouds/C.txt";
		e_cloud.open(eCloudPath.c_str()); //open file containing relative electron points

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
		else
		{
			std::cout << eCloudPath << '\n';
		  throw "Could not access electron cloud.";
		}
		e_cloud.close();
	}

  //either there is or isnt a transform
	x_transform = (min_x < 0)? -min_x: 0; //transforms will always be 0 or positive values
	y_transform = (min_y < 0)? -min_y: 0; //representing the opposite of the most negative value
	z_transform = (min_z < 0)? -min_z: 0;

  /*if (!numOfVoxels)
  {
    //number of voxels for cube will be max distance in any x rounded up, then divided by voxel size
  	double maxDistance = max_x - min_x;
  	if (maxDistance < (max_y - min_y))  //note that max distance affecting voxelsize does not necessarily mean we actually need that many voxels
  		maxDistance = max_y - min_y;      //we are actually over allocating here for simplicity
  	if (maxDistance < (max_z - min_z))  //use distance formula later
  		maxDistance = max_z - min_z;

    numOfVoxels = uint32_t(maxDistance / voxelSize + 1); //add one to round up
  }*/
}

//working
void Voxelizer::populateGrid(const Atom * const &a, uint32_t count)
{
	std::ifstream e_cloud;

  for(uint32_t i = 0; i < count ; ++i)
	{
		//get indecis for voxel placement, may be wrong at the current moment... truncation is not a problem
		uint32_t index_x = (a[i].getX() + x_transform) / voxelSize;
		uint32_t index_y = (a[i].getY() + y_transform) / voxelSize;
		uint32_t index_z = (a[i].getZ() + z_transform) / voxelSize;

    if (index_x >= numOfVoxels || index_y >= numOfVoxels || index_z >= numOfVoxels)
      throw("populateGridError: Atom index falls outside of grid space specified by the user.");
		grid[index_x][index_y][index_z].addProton(); //later will replace with different means
		grid[index_x][index_y][index_z].addNeutron(); //representing nucleus

    //open electron cloud for each atom
		std::string eCloudPath = eCloudDirectory + a[i].getElemName() + ".txt";	//Parent/ElectronClouds/ELEMENT_SYMBOL.txt
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

				//corresponding indicies or place against wall
				index_x = temp_x / voxelSize;
				index_y = temp_y / voxelSize;
				index_z = temp_z / voxelSize;

        if (index_x >= numOfVoxels || index_y >= numOfVoxels || index_z >= numOfVoxels)
          throw("populateGridError: Electron index falls outside of grid space specified by the user.");
				grid[index_x][index_y][index_z].addElectron();
			}
		}
		else
		{
			std::cout << eCloudPath << '\n';
		  throw "Could not access electron cloud.";
		}
		e_cloud.close();
	}
}

void Voxelizer::resizeGrid()
{
	std::vector<Voxel> lv1(numOfVoxels);
	std::vector<std::vector<Voxel>> lv2(numOfVoxels,lv1);
	grid.resize(numOfVoxels, lv2);
}
