Voxelizer: programfiles programfiles/main.o
	g++ -o Voxelizer programfiles/main.o programfiles/Voxelizer.o programfiles/Voxel.o programfiles/ChemParse.o programfiles/Atom.o
programfiles/main.o: src/main.cpp programfiles/Voxelizer.o programfiles/ChemParse.o
	g++ -std=c++0x -o programfiles/main.o src/main.cpp -c -Wall -pedantic -g
programfiles/Voxelizer.o: src/Voxelizer.cpp src/Voxelizer.h programfiles/Voxel.o
	g++ -std=c++0x -o programfiles/Voxelizer.o src/Voxelizer.cpp -c -Wall -pedantic -g
programfiles/Voxel.o: src/Voxel.cpp src/Voxel.h
	g++ -std=c++0x -o programfiles/Voxel.o src/Voxel.cpp -c -Wall -pedantic -g
programfiles/ChemParse.o: src/ChemParse.cpp src/ChemParse.h programfiles/Atom.o
	g++ -o programfiles/ChemParse.o src/ChemParse.cpp -c -Wall -pedantic -g
programfiles/Atom.o: src/Atom.cpp src/Atom.h
	g++ -o programfiles/Atom.o src/Atom.cpp -c -Wall -pedantic -g


programfiles:
	mkdir programfiles
