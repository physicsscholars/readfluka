#ifndef ResNuclei_h
#define ResNuclei_h

//////////////////////////////////////////////////////
//
// ResNuclei - a class to read the RESNUCLEI binary output
//
//////////////////////////////////////////////////////

#include <fstream>
#include <vector>
#include <string>
#include "Base.h"
#include <cmath>

namespace ReadFluka {
  class ResNuclei : public Base {
  protected:
    bool fIsReadHeader;
    int  fNRN;
    std::string fTIURSN; // scoring name
    int fITURSN; // type of binning: 1 = spallation products, 2 = low-energy neutrons products, 3=all products
    unsigned int fNRURSN; // region
    float fVURSNC; // volume [cm^3] of the detector
    int fIMRHGH; // maximum M=N-Z-NMZ_min of the scoring (minimum M: 1). Note: N-Z=M+NMZ_min,N=M+Z+NMZ_min
    int fIZRHGH; // maximum Z of the scoring (minimum Z: 1)
    int fK; // what is it? !!!
    std::vector< std::vector<float> > fRNDATA; // residual nuclei data - the scored values
    //float fRNDATA[100][260];

    bool ReadHeader();
  public:
    ResNuclei(const char *fname);
    virtual ~ResNuclei();
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

    std::vector< std::vector<float> > GetRNDATA() const {return fRNDATA;}
    float GetRNDATA(int Z, int A) const;
    
    const char *GetXtitle() const {return "Z";} 
    const char *GetYtitle() const {return "A";} 
    const char *GetZtitle() const {return "Residual nuclei per cm^{3} per primary";}

    inline unsigned int GetNbinsZ() const {return GetZmax()-GetZmin();}
    inline unsigned int GetNbinsA() const {return GetAmax()-GetAmin();}
    

    void Print() const;
  };
}

#endif
