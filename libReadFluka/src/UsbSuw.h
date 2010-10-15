#ifndef UsbSuw_h
#define UsbSuw_h

//////////////////////////////////////////////////////
//
// UsbSuw - a class to read the $FLUTIL/usbsuw binary output (USRBIN)
// 
//////////////////////////////////////////////////////

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

    std::vector<int> fDXUSBN;
    std::vector<int> fDYUSBN;
    std::vector<int> fDZUSBN;

  public:
    UsbSuw(const char *fname);
    virtual ~UsbSuw();
    void Reset();
    bool Read();

    void Print() const;
  };
}

#endif
