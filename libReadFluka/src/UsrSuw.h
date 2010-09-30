#ifndef UsrSuw_h
#define UsrSuw_h

//////////////////////////////////////////////////////
//
// UsrSuw - a class to read the $FLUTIL/usrsuw binary output
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
    std::vector< std::vector<float> > fRNERR; // residual nuclei relative errors [%]
    
    float fTotalResp;
    float fTotalRespErr;
    std::vector<float> fYieldA; // isotope yield as a function of mass number
    std::vector<float> fYieldAErr;
    std::vector<float> fYieldZ; // isotope yield as a function of atomic number
    std::vector<float> fYieldZErr;

  public:
    UsrSuw(const char *fname);
    virtual ~UsrSuw();
    void Reset();
    bool Read();

    std::string GetBinTitle() const;
    inline std::string GetBinTitleA() const {return "Isotope Yield as a function of Mass Number";}
    inline std::string GetBinTitleZ() const {return "Isotope Yield as a function of Atomic Number";}

    inline int GetA(int i, int j) const {return j+1+fK+2*(i+1);}

    std::vector< std::vector<float> > GetRNERR()  const {return fRNERR;}
    float GetRNERR(int Z, int A) const;
    
    inline float GetYieldA(int i) const {return fYieldA[i];}
    inline float GetYieldAErr(int i) const {return fYieldAErr[i];}

    inline float GetYieldZ(int i) const {return fYieldZ[i];}
    inline float GetYieldZErr(int i) const {return fYieldZErr[i];}

    void Print() const;
  };
}

#endif
