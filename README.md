# Active-Site
Models an Active site of a molecule in 3 dimensional space.

Linux:
  How To Install:

    1) Choose some directory for the application to be stored. (e.g. c:/my/path or ~/my/path)

    2) If you have git installed then you can type git clone https://github.com/Active-Site-Project/3D-Representation.git from 
        inside the directory that you you would like the project to be installed in. If you do not have git installed, you can 
        download the repostitory zip file and extract it to the directory that you would like the project to be stored in.

    3) Navigate to 3D-Representation/ directory and type the command make. You can now either run Voxelizer.exe with command line
        arguments, or you can run Voxelizer.exe and it will prompt you for the molfile and your desired voxel size. (See 
        documentation on Voxelizer.exe below.)
        

  How to use Voxelizer.exe, no command line arguments:

    1) From the 3D-representation folder, run ./Voxelizer.exe.

    2) Once the program has started, you will be prompted to see if you are adding to an existing active site .json file,  
        or if you and creating a new active site from a molFile. (To which you will respond with either 'y' or 'n') Note: a 
        existing active site file will be tagged with MoleculeGrid-V1.0 as the first line.

    3) If you are creating a new active site, you will be prompted first for the .mol file that you would like to represent, and 
        then the cubic dimensions of each Voxel that you would like to use. If you are adding to an existing active site, you 
        will first be prompted for the .json file that represents the active site, and then the .mol File that you would like to 
        add to the active site. Note: When adding to an active site, the active site itself is generated from this program, so 
        you would be feeding one of the .json files that has been created by this program. Whether you are adding to an existing 
        active site or creating a new one, a .json named molecule.json will be created in 3D-Representation/ that will either 
        represent your new active site, or the combination of your previous active site and the molecule (.mol file) that you 
        provided.


  How to use Voxelizer.exe, with command line arguments:


Windows: //To be determined
