#ifndef _Mgdraw_h_
#define _Mgdraw_h_

#include "Base.h"

namespace ReadFluka {
	class Hit {
	public:
		int id;
		float energy;
		float x;
		float y;
		float z;
		float cosX;
		float cosY;
		float cosZ;
	};

	class Mgdraw : public Base {
		int fNPFLKA; // number of particles in stack
		int fNSTMAX; // maximum number of particles in stack so far
		float fTKESUM; // total kinetic energy of the primaries of a user written SOURCE

		int nevent;
		int ntrack;
		int mtrack;
		int jtrack;
		int icode;
		float etrack;
		float wtrack;
		
	public:
		Mgdraw(const char *fname);
		virtual ~Mgdraw();

		int ReadEvent(int type=0);
		//		int ReadTrack(int ntrack, int mtrack, int jtrack, float etrack, float wtrack);
		//		int ReadEnergy(int icode, int jtrack, float etrack, float wtrack);
		//		int ReadSource(int ncase, int npflka, int nstmax, float tkesum, float weipri);

		void Print(const char *option="") const;
	};
}

#endif
