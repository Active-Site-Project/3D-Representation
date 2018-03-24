main: main.o Voxelizer.o Voxel.o MolParse.o Atom.o
	g++ -o main main.o Voxelizer.o Voxel.o MolParse.o Atom.o
main.o: main.cpp Voxelizer.h
	g++ -std=c++0x main.cpp -c -Wall -pedantic
Voxelizer.o: Voxelizer.cpp Voxelizer.h Voxel.o MolParse.o
	g++ -std=c++0x Voxelizer.cpp -c -Wall -pedantic
Voxel.o: Voxel.cpp Voxel.h
	g++ -std=c++0x Voxel.cpp -c -Wall -pedantic
MolParse.o: MolParse.cpp MolParse.h Atom.o
	g++ -c MolParse.cpp -Wall -pedantic
Atom.o: Atom.cpp Atom.h
	g++ -c Atom.cpp -Wall -pedantic
