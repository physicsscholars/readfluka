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
	fSize_start = ReadInt();

	int icase = ReadInt(); 
	if (icase<0) { std::cout << "source particle" << std::endl;
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
		
		ReadInt();
		
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
		}
	} else if (icase == 0) {
		std::cout << "point energy deposition" << std::endl;
		//		std::cout << "zero: " << ReadInt() << std::endl;

		int icode = ReadInt();
		int jtrack = ReadInt();
		float etrack = ReadFloat();
		float wtrack = ReadFloat();
		std::cout << icode << " " << jtrack << " " << etrack << " " << wtrack << std::endl;

		fSize_end = ReadInt(); CheckSize();		fSize_start = ReadInt();

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
	fSize_end = ReadInt();
	CheckSize();

	return icase;
}

void Mgdraw::Print(const char *option) const
{
	std::cout << "print" << std::endl;
}
