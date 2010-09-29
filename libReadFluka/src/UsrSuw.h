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
#include "Base.h"
#include <cmath>

namespace ReadFluka {
  class UsrSuw : public Base {
  private:
    bool fIsReadHeader;
    int  fNRN;
    std::string fTIURSN; // scoring name
    int fITURSN; // type of binning: 1 = spallation products, 2 = low-energy neutrons products, 3=all products
    unsigned int fNRURSN; // region
    float fVURSNC; // volume [cm^3] of the detector
    int fIMRHGH; // maximum M=N-Z-NMZ_min of the scoring (minimum M: 1). Note: N-Z=M+NMZ_min,N=M+Z+NMZ_min
    int fIZRHGH; // maximum Z of the scoring (minimum Z: 1)
    int fK; // NMZMNM in usrsuw.f
    std::vector< std::vector<float> > fRNDATA; // residual nuclei data - the scored values
    std::vector< std::vector<float> > fRNERR; // residual nuclei relative errors [%]
    
    float fTotalResp;
    float fTotalRespErr;
    std::vector<float> fYieldA; // isotope yield as a function of mass number
    std::vector<float> fYieldAErr;
    std::vector<float> fYieldZ; // isotope yield as a function of atomic number
    std::vector<float> fYieldZErr;

    bool ReadHeader();
  public:
    UsrSuw(const char *fname);
    virtual ~UsrSuw();
    void Reset();
    bool Read();

    inline int GetNRN() const {return fNRN;}
    inline std::string GetTIURSN() const {return fTIURSN;}
    inline std::string GetBinName() const {return fTIURSN;} // the same as GetTIURSN()
    std::string GetBinTitle() const;
    inline int GetITURSN() const {return fITURSN;}
    inline unsigned int   GetNRURSN() const {return fNRURSN;}
    inline float          GetVURSNC() const {return fVURSNC;}
    inline int            GetIMRHGH() const {return fIMRHGH;}
    inline int            GetIZRHGH() const {return fIZRHGH;}
    inline int            GetK() const {return fK;}

    inline int GetAmin() const {return 1;} 
    inline int GetAmax() const {return fIMRHGH + 2*fIZRHGH + fK;}
    inline int GetZmin() const {return 1;}
    inline int GetZmax() const {return fIZRHGH;} // the same as GetIZRHGH()
    inline int GetA(int i, int j) const {return j+1+fK+2*(i+1);}

    //inline const float *GetRNDATA() const {return fRNDATA;}
    std::vector< std::vector<float> > GetRNDATA() const {return fRNDATA;}
    std::vector< std::vector<float> > GetRNERR()  const {return fRNERR;}
    float GetRNDATA(unsigned int Z, unsigned int A) const;
    float GetRNERR(unsigned int Z, unsigned int A) const;
    
    const char *GetXtitle() const {return "Z";} 
    const char *GetYtitle() const {return "A";} 
    const char *GetZtitle() const {return "Residual nuclei per cm^{3} per primary";}

    inline unsigned int GetNbinsZ() const {return GetZmax()-GetZmin();}
    inline unsigned int GetNbinsA() const {return GetAmax()-GetAmin();}
    

    void Print() const;
  };
}

#endif
