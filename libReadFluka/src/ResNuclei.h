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
  private:
    bool fIsReadHeader;
    int  fNRN;
    std::string fTIURSN; // scoring name
    unsigned short fITURSN; // type of binning: 1 = spallation products, 2 = low-energy neutrons products, 3=all products
    unsigned int fNRURSN; // region
    float fVURSNC; // volume [cm^3] of the detector
    int fIMRHGH; // maximum M=N-Z-NMZ_min of the scoring (minimum M: 1). Note: N-Z=M+NMZ_min,N=M+Z+NMZ_min
    int fIZRHGH; // maximum Z of the scoring (minimum Z: 1)
    int fK; // what is it? !!!
    std::vector< std::vector<float> > fRNDATA; // residual nuclei data - the scored values


    bool ReadHeader();
  public:
    ResNuclei(const char *fname);
    virtual ~ResNuclei();
    void Reset();
    bool Read();

    inline int GetNRN() const {return fNRN;}
    inline std::string GetTIURSN() const {return fTIURSN;}
    inline unsigned short GetITURSN() const {return fITURSN;}
    inline unsigned int   GetNRURSN() const {return fNRURSN;}
    inline float          GetVURSNC() const {return fVURSNC;}
    inline int            GetIMRHGH() const {return fIMRHGH;}
    inline int            GetIZRHGH() const {return fIZRHGH;}
    inline int            GetK() const {return fK;}

    //inline const float *GetRNDATA() const {return fRNDATA;}
    

    void Print() const;
  };
}

#endif
