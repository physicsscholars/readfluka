#ifndef UsrSuw_h
#define UsrSuw_h

//////////////////////////////////////////////////////
//
// UsrSuw - a class to read the $FLUTIL/usrsuw binary output (RESNUCLEI)
// 
//////////////////////////////////////////////////////

#include <fstream>
#include <vector>
#include <string>
#include "ResNuclei.h"
#include <cmath>

namespace ReadFluka {
  class UsrSuw : protected ResNuclei {
  private:
    std::vector<int> fNRN;
    std::vector<std::string> fTIURSN; // scoring names
    std::vector<int> fITURSN; // type of binning: 1 = spallation products, 2 = low-energy neutrons products, 3=all products
    std::vector<unsigned int> fNRURSN; // regions
    std::vector<float> fVURSNC; // volume of the detector [cm^3]
    std::vector<int> fIMRHGH; // maximum M=N-Z-NMZ_min of the scoring (minimum M: 1). Note: N-Z=M+NMZ_min,N=M+Z+NMZ_min
    std::vector<int> fIZRHGH; // maximum Z of the scoring (minimum Z: 1)
    std::vector<int> fK;
    unsigned short fN; // number of RESNUCLEI cards read
    std::vector<float> fTotalResp;
    std::vector<float> fTotalRespErr;

    std::vector< std::vector<float> > fYieldA; // isotope yield as a function of mass number
    std::vector< std::vector<float> > fYieldAErr;

    std::vector< std::vector<float> > fYieldZ; // isotope yield as a function of atomic number
    std::vector< std::vector<float> > fYieldZErr;

    std::vector< std::vector< std::vector<float> > > fRNDATA; // residual nuclei production rate
    std::vector< std::vector< std::vector<float> > > fRNERR; // residual nuclei relative errors [%]

  public:
    UsrSuw(const char *fname);
    virtual ~UsrSuw();
    void Reset();
    bool Read();
    
    inline unsigned short GetN() const {return fN;}
    
    inline std::string GetTIURSN(int i) const {return fTIURSN[i];}
    inline std::string GetBinName(int i) const {return GetTIURSN(i);}
    std::string GetBinTitle(int i) const;
    inline std::string GetBinTitleA(int i) const {return "Isotope Yield as a function of Mass Number in the region";}
    inline std::string GetBinTitleZ(int i) const {return "Isotope Yield as a function of Atomic Number";}

    inline int GetA(int k, int i, int j) const {return j+1+fK[k]+2*(i+1);}

    std::vector< std::vector< std::vector<float> > > GetRNDATA()  const {return fRNDATA;}
    float GetRNDATA(int i, int Z, int A) const;
    std::vector< std::vector< std::vector<float> > > GetRNERR()  const {return fRNERR;}
    float GetRNERR(int i, int Z, int A) const;
    
    inline float GetYieldA(int i, int j) const {return fYieldA[i][j];}
    inline float GetYieldAErr(int i, int j) const {return fYieldAErr[i][j];}

    inline float GetYieldZ(int i, int j) const {return fYieldZ[i][j];}
    inline float GetYieldZErr(int i, int j) const {return fYieldZErr[i][j];}

    inline int GetIZRHGH(int i) const {return fIZRHGH[i];}
    inline int GetIMRHGH(int i) const {return fIMRHGH[i];}

    inline int GetAmin(int i) const {return 1;} 
    inline int GetZmin(int i) const {return 1;} 
    inline int GetAmax(int i) const {return fIMRHGH[i] + 2*fIZRHGH[i] + fK[i];}
    inline int GetZmax(int i) const {return fIZRHGH[i];}

    inline unsigned int GetNbinsZ(int i) const {return GetZmax(i)-GetZmin(i);}
    inline unsigned int GetNbinsA(int i) const {return GetAmax(i)-GetAmin(i);}

    void Print() const;
  };
}

#endif
