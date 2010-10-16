#ifndef UsbSuw_h
#define UsbSuw_h

/////////////////////////////////////////////////////////////////////////
//
// UsbSuw - a class to read the $FLUTIL/usbsuw binary output (USRBIN)
// 
// The corresponding FLUKA program is called 'usbrea'
//
/////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <vector>
#include <string>
#include "UsrBin.h"
#include <cmath>

namespace ReadFluka {
  class UsbSuw : protected UsrBin {
  private:
    std::vector<int> fITUSBN; // type of binning
    std::vector<std::string> fTITUSB; // title of distribution scored
    std::vector<int> fIDUSBN; // distribution to be scored
    
    std::vector<float> fXLOW; // minimum value of x
    std::vector<float> fXHIGH; // maximum value of x
    std::vector<float> fYLOW;
    std::vector<float> fYHIGH;
    std::vector<float> fZLOW;
    std::vector<float> fZHIGH;

    std::vector<int> fNXBIN;
    std::vector<int> fNYBIN;
    std::vector<int> fNZBIN;

    std::vector<float> fDXUSBN;
    std::vector<float> fDYUSBN;
    std::vector<float> fDZUSBN;

    std::vector< std::vector<float> > fScored; // GMSTOR?

  public:
    UsbSuw(const char *fname);
    virtual ~UsbSuw();
    void Reset();
    bool Read();

    inline const int GetNbins(int i) const {return fNXBIN[i]*fNYBIN[i]*fNZBIN[i];}

    void Print() const;
  };
}

#endif
