#include "Voxel.h"

Voxel::Voxel() { protons = neutrons = electrons = 0; }

Voxel::Voxel(const Voxel &v)
{
	protons = v.protons;
	electrons = v.electrons;
	neutrons = v.neutrons;
}

uint64_t Voxel::getProtons() { return protons; }

uint64_t Voxel::getNeutrons() { return neutrons; }

uint64_t Voxel::getElectrons() { return electrons; }

void Voxel::addProton() { ++protons; }

void Voxel::addNeutron() { ++neutrons; }

void Voxel::addElectron() { ++electrons; }

void Voxel::addProtons(uint64_t p) { protons += p; }

void Voxel::addNeutrons(uint64_t n) { neutrons += n; }

void Voxel::addElectrons(uint64_t e) { electrons += e; }
