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

    std::string GetBinTitle() const;
    inline std::string GetBinTitleA() const {return "Isotope Yield as a function of Mass Number";}
    inline std::string GetBinTitleZ() const {return "Isotope Yield as a function of Atomic Number";}

    inline int GetA(int i, int j) const {return j+1+fK+2*(i+1);}

    std::vector< std::vector< std::vector<float> > > GetRNDATA()  const {return fRNDATA;}
    float GetRNDATA(int i, int Z, int A) const;
    std::vector< std::vector< std::vector<float> > > GetRNERR()  const {return fRNERR;}
    float GetRNERR(int i, int Z, int A) const;
    
    inline float GetYieldA(int i, int j) const {return fYieldA[i][j];}
    inline float GetYieldAErr(int i, int j) const {return fYieldAErr[i][j];}

    inline float GetYieldZ(int i, int j) const {return fYieldZ[i][j];}
    inline float GetYieldZErr(int i, int j) const {return fYieldZErr[i][j];}

    void Print() const;
  };
}

#endif
