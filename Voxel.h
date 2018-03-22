#pragma once
#ifndef VOXEL_H_NAP
#define VOXEL_H_NAP

#include <cstdint>

class Voxel
{
  private:
	  uint64_t protons, neutrons, electrons;

  public:
	  Voxel(); //constructor, start with 0 of each
	  Voxel(const Voxel&); //copy constructor
	  uint64_t getProtons(); //returns number of protons in voxel
	  uint64_t getNeutrons(); //returns number of neutrons
	  uint64_t getElectrons(); //returns number of electrons
	  void addProton(); //adds 1 proton
	  void addNeutron(); //adds 1 neutron
	  void addElectron(); //adds 1 electron
	  void addProtons(uint64_t); //add p protons
	  void addNeutrons(uint64_t); //add n neutrons
	  void addElectrons(uint64_t); //add e electrons
};

#endif