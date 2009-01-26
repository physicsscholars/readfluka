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
	int len = SizeStart();
	if (len != 20) {
		std::cerr << "Mgdraw::ReadEvent():\t format error" << std::endl;
		return -1;
	}

	int icase = ReadInt(); // ndum
	fNCASE = ReadInt();    // mdum
	fNPFLKA = ReadInt();   // jdum
	fTKESUM = ReadFloat(); // edum
	fWEIPRI = ReadFloat(); // wdum
	
	SizeEnd();

	std::cout << "icase: " << icase << "\t- type of this record" << std::endl;
	std::cout << "ncase: " << fNCASE << "\t- number of primaries treated so far (including the current one)" << std::endl;
	std::cout << "npflka: " << fNPFLKA << "\t- number of particles in the stack" << std::endl;
	//	std::cout << "nstmax: " << fNSTMAX << std::endl;
	std::cout << "tkesum: " << fTKESUM << "\t- total kinetic energy of the primaries of a user written SOURCE" << std::endl;
	std::cout << "weipri: " << fWEIPRI << "\t- total weight of the primaries handled so far" << std::endl;
	
	if (icase<0) {
		if ( (type == 0) || (type == 2) ) ReadSource(-icase, fNCASE, fNPFLKA, fTKESUM, fWEIPRI);
	} else if (icase == 0) {
		if ( (type == 0) || (type == 1) ) ReadEnergy(fNCASE, fNPFLKA, fTKESUM, fWEIPRI);
	} else if (icase > 0) {
		if (type == 0) ReadTrack(icase, fNCASE, fNPFLKA, fTKESUM, fWEIPRI);
	}
		



		/*	int icase = ReadInt(); 
	if (icase<0) { 
		std::cout << "source particle" << std::endl;
		fNCASE = ReadInt(); 
		fNPFLKA = ReadInt(); 
		fNSTMAX = ReadInt();
		fTKESUM = ReadFloat();
		fWEIPRI = ReadFloat();

		std::cout << "ncase: " << fNCASE << std::endl;
		std::cout << "npflka: " << fNPFLKA << std::endl;
		std::cout << "nstmax: " << fNSTMAX << std::endl;
		std::cout << "tkesum: " << fTKESUM << std::endl;
		std::cout << "weipri: " << fWEIPRI << std::endl;
		
		std::cout << "int: " << ReadInt() << std::endl;
		
		std::vector<ReadFluka::Hit*> hits;

		for (int i=0; i<fNPFLKA; i++) {
			Hit *hit = new Hit;
			hit->id = ReadInt();
			hit->energy = ReadFloat();
			hit->x = ReadFloat(); hit->y = ReadFloat(); hit->z = ReadFloat();
			if (false) ReadFloat(); else std::cout << "strange number: " << ReadFloat() << std::endl;
			hit->cosX = ReadFloat(); hit->cosY = ReadFloat(); hit->cosZ = ReadFloat();

			std::cout << "hit: " << hit->id << "\t" << hit->energy << "\t";
			std::cout << hit->x << " " << hit->y << " " << hit->z << "\t";
			std::cout << hit->cosX << " " << hit->cosY << " " << hit->cosZ << std::endl;

			hits.push_back(hit);
		}
	} else if (icase == 0) {
		std::cout << "point energy deposition" << std::endl;
		//		std::cout << "zero: " << ReadInt() << std::endl;

		int icode = ReadInt();
		int jtrack = ReadInt();
		float etrack = ReadFloat();
		float wtrack = ReadFloat();
		std::cout << icode << " " << jtrack << " " << etrack << " " << wtrack << std::endl;

		SizeEnd(); SizeStart();

		float xsco = ReadFloat();
		float ysco = ReadFloat();
		float zsco = ReadFloat();
		float rull = ReadFloat();
		std::cout << "\t" << xsco << " " << ysco << " " << zsco << " " << rull << std::endl;

	} else if (icase>0) {
		std::cout << "continuous energy deposition" << std::endl;
	} else {
		std::cout << "do not know this case: " << icase << std::endl;
	}

	//	std::cout << "strange number: " << ReadInt() << std::endl;
	SizeEnd();
		*/
	return icase;
}

int Mgdraw::ReadSource(int ncase, int npflka, int nstmax, float tkesum, float weipri)
{
	// Read source particle

	std::cout << "*** source particle" << std::endl;
	fNCASE = ncase;
	fNPFLKA = npflka;
	fNSTMAX = nstmax;
	fTKESUM = tkesum;
	fWEIPRI = weipri;
	
	/*	std::cout << "ncase: " << fNCASE << std::endl;
	std::cout << "npflka: " << fNPFLKA << std::endl;
	std::cout << "nstmax: " << fNSTMAX << std::endl;
	std::cout << "tkesum: " << fTKESUM << std::endl;
	std::cout << "weipri: " << fWEIPRI << std::endl;*/
	
	//	std::cout << "int: " << ReadInt() << std::endl;
	SizeStart();
	
	std::vector<ReadFluka::Hit*> hits;
	
	for (int i=0; i<fNPFLKA; i++) {
		Hit *hit = new Hit;
		hit->id = ReadInt();
		hit->energy = ReadFloat();
		hit->x = ReadFloat(); hit->y = ReadFloat(); hit->z = ReadFloat();
		if (false) ReadFloat(); else std::cout << "\t strange number: " << ReadFloat() << std::endl;
		hit->cosX = ReadFloat(); hit->cosY = ReadFloat(); hit->cosZ = ReadFloat();
		
		std::cout << "\t hit: " << hit->id << "\t" << hit->energy << "\t";
		std::cout << hit->x << " " << hit->y << " " << hit->z << "\t";
		std::cout << hit->cosX << " " << hit->cosY << " " << hit->cosZ << std::endl;
		
		hits.push_back(hit);
	}
	SizeEnd();

	return ncase;
}

int Mgdraw::ReadEnergy(int icode, int jtrack, float etrack, float wtrack)
{
	//
	std::cout << "*** point energy deposition" << std::endl;
	//		std::cout << "zero: " << ReadInt() << std::endl;
	
	std::cout << "\t icode,jtrack,etrack,wtrack: " << icode << " " << jtrack << " " << etrack << " " << wtrack << std::endl;
	
	SizeStart();
	
	float xsco = ReadFloat();
	float ysco = ReadFloat();
	float zsco = ReadFloat();
	float rull = ReadFloat();
	std::cout << "\t x,y,z,rull: " << xsco << " " << ysco << " " << zsco << " " << rull << std::endl;

	return icode;
}

int Mgdraw::ReadTrack(int ntrack, int mtrack, int jtrack, float etrack, float wtrack)
{
	// Read track

	return 0;
}


void Mgdraw::Print(const char *option) const
{
	std::cout << "print" << std::endl;
}
