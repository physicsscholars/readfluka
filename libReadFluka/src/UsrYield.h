#ifndef UsrYield_h
#define UsrYield_h

/////////////////////////////////////////////////
//
//  UsrYield
//
//  A class to read FLUKA's USRYIELD outpur
//
/////////////////////////////////////////////////

#include "Base.h"

namespace ReadFluka {
	class UsrYield : public Base {
	private:
		float                        fWEIPRI;          // weight of primary particle
		int                          fIJUSYL;          // projectile identity
		int                          fJTUSYL;          // target identity
		float                        fPUSRYL;          // momentum of projectile to be used to define (possible) Lorentz transformations, Feynman X etc.
		float                        fSQSUYL;          // cms energy for Lorentz transformation
		float                        fUUSRYL;          // laboratory projectile cosX-direction
		float                        fVUSRYL;          // laboratory projectile cosY-direction
		float                        fWUSRYL;          // laboratory projectile cosZ-direction
		
		int                          fMY;              // id-number of the yield detector
		char                        *fTITUYL;          // detector name (SDUM in first card)
		
		int                          fITUSYL;          // type of binning = WHAT(1) of first card = ie + ia*100
		int                          fie;              // ie, first physical quantity to be scored
		int                          fia;              // ia, second physical quantity to be scored
		
		int                          fIXUSYL;          // cross section kind, ixa + ixm*100
		int                          fixa;             // the kind of yield or cross-section desired
		int                          fixm;             // the target material (if needed in order to calculate a cross-section, otherwise ixm = 0)
		
		
		int                          fIDUSYL;          // particle scored = WHAT(2) of first card
		int                          fNR1UYL;          // first region = WHAT(4) of first card
		int                          fNR2UYL;          // second region = WHAT(5) of first card
		float                        fUSNRYL;          // user normalization factor
		float                        fSGUSYL;          // adopted cross section (if any)
		bool                         fLLNUYL;          // no low-energy neutron scoring if false, yes if true
		float                        fEYLLOW;          // minimum energy (GeV) (WHAT(2) of 2nd card: may be re-defined if low-energy neutrons are scored
		float                        fEYLHGH;          // maximum energy (GeV) (WHAT(1) of 2nd card)
		int                          fNEYLBN;          // number of energy intervals (re-defined by the program if low-energy neutrons are scored)
		float                        fDEYLBN;          // energy bin width (GeV) if linear in energy or referring to a low-energy neutron energy group, otherwise ratio between upper and lower edge of energy intervals
		float                        fAYLLOW;          // minimum angle (steradian) (WHAT(5) of 2nd card)
		float                        fAYLHGH;          // maximum angle (steradian) (WHAT(4) of 2nd card)
		
		int                          fIGMUYL;          //  maximum low-energy neutron group to be scored
		float                       *fENGMAX;          //  upper energies (GeV) of the neutron groups
		
		float                       *fSCORED;          // scored results [sr^{-1}*GeV^{-1}]
		int                          fINTERV;          // number of intervals in fSCORED = fNEYLBN + fIGMUYL
		
		bool                         fIsReadHeader;
		void                           ReadHeader();     // read the header
		void                           Reset();
		
		void                           MakeHist();
	public:
		UsrYield(const char *fname);
		virtual ~UsrYield();
		
		bool                         Read();
		float                        GetMin1() const {return fEYLLOW;}
		float                        GetMax1() const {return fEYLHGH;}
		float                        GetMin2() const {return fAYLLOW;}
		float                        GetMax2() const {return fAYLHGH;}
		int                          GetNbins1() const {return fNEYLBN;}
		
		const float                 *GetScored() const {return fSCORED;}
		float                        GetScored(int bin) const {return bin < fINTERV ? fSCORED[bin] : -1;}
		
		unsigned int                         GetUpstreamRegion() const {return fNR1UYL > 0 ? fNR1UYL : 0;}
		unsigned int                         GetDownstreamRegion() const {return fNR2UYL > 0 ? fNR2UYL : 0;}
		float                        GetUserNorm() const {return fUSNRYL;}
		
		const char                  *GetTitle() const {return fTITUYL;}
		const char                  *GetXTitle() const;
		const char                  *GetYTitle() const;
		int                          GetTypeOfScoredParticles() const {return fIDUSYL;}
		const char                  *GetNameOfScoredParticles() const;
	};
}

#endif
