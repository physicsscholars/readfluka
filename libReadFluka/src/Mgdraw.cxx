#include "Mgdraw.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace ReadFluka;

Mgdraw::Mgdraw(const char *fname) : Base(fname)
{
	// Constructor

	nevent = ntrack = mtrack = jtrack = icode = 0;
	etrack = wtrack = 0.0f;
}

Mgdraw::~Mgdraw()
{
	// Destructor
}

int Mgdraw::ReadEvent(int type)
{
	std::cout << "ReadEvent" << std::endl;
	int len = SizeStart();
	if (len != 20) {
		std::cerr << "Mgdraw::ReadEvent():\t format error" << std::endl;
		return -1;
	}

	int icase;

	for (int ievent=0; ievent<50000000; ievent++) {
		icase = ReadInt(); // ndum
		
		if (icase<0) {
			if ( (type == 0) || (type == 2) ) ReadSource();
		} else if (icase == 0) {
			if ( (type == 0) || (type == 1) ) ReadEnergy();
		} else if (icase > 0) {
			if (type == 0) ReadTrack();
		}
		if ((fNCASE>1) && (icase == 0)) {
			std::cerr << "next event" << std::endl;
			break;
		}
	}

	return icase;
}

int Mgdraw::ReadSource()
{
	// Read source particle

	std::cout << "*** source particle" << std::endl;

	fNCASE = ReadInt();    // mdum
	fNPFLKA = ReadInt();   // jdum
	fTKESUM = ReadFloat(); // edum
	fWEIPRI = ReadFloat(); // wdum

	//	std::cout << "icase: " << icase << "\t- type of this record" << std::endl;
	std::cout << "\tncase: " << fNCASE << "\t- number of primaries treated so far (including the current one)" << std::endl;
	std::cout << "\tnpflka: " << fNPFLKA << "\t- number of particles in the stack" << std::endl;
	//	std::cout << "nstmax: " << fNSTMAX << std::endl;
	std::cout <<"\ttkesum: " << fTKESUM << "\t- total kinetic energy of the primaries of a user written SOURCE" << std::endl;
	std::cout << "\tweipri: " << fWEIPRI << "\t- total weight of the primaries handled so far" << std::endl;

	SizeEnd();	
	SizeStart();
	
	std::vector<ReadFluka::Hit*> hits;
	
	for (int i=0; i<fNPFLKA; i++) {
		Hit *hit = new Hit;
		hit->id = ReadInt();
		hit->energy = ReadFloat();
		std::cout << "\t hit: " << hit->id << "\t" << hit->energy << "\t";

		hit->x = ReadFloat(); hit->y = ReadFloat(); hit->z = ReadFloat();
		std::cout << hit->x << " " << hit->y << " " << hit->z << "\t";

		if (false) ReadFloat(); else std::cout << "\t (strange number: " << ReadFloat() << ")\t";
		hit->cosX = ReadFloat(); hit->cosY = ReadFloat(); hit->cosZ = ReadFloat();
		std::cout << hit->cosX << " " << hit->cosY << " " << hit->cosZ << std::endl;
		
		hits.push_back(hit);
	}
	SizeEnd(); SizeStart();

	return fNCASE;
}

int Mgdraw::ReadEnergy()
{
	//
	std::cout << "*** point energy deposition" << std::endl;

	int   icode  = ReadInt();
	int   jtrack = ReadInt();
	float etrack = ReadFloat();
	float wtrack = ReadFloat();
	std::cout << "\t icode,jtrack,e,w: " << icode << " " << jtrack << " " << etrack << " " << wtrack << std::endl;
	
	SizeEnd(); SizeStart();

	float xsco = ReadFloat();
	float ysco = ReadFloat();
	float zsco = ReadFloat();
	float rull = ReadFloat();
	std::cout << "\t x,y,z,rull: " << xsco << " " << ysco << " " << zsco << " " << rull << std::endl;
	
	SizeEnd(); SizeStart();

	return icode;
}

int Mgdraw::ReadTrack()
{
	// Read track

	return 0;
}


void Mgdraw::Print(const char *option) const
{
	std::cout << "print" << std::endl;
}
