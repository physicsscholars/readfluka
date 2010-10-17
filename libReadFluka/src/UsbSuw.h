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
  class UsbSuw : public Base {
  private:
    unsigned int fReadCounter;
    int                   fMCASE;  //
    int                   fNBATCH; //
    int fNCTOT;
    int fMCTOT;
    int fWCTOT;
    int fKLAST;
    int fKMAX;

    std::vector<std::string> fTITUSB; // title of distribution scored
    std::vector<int> fITUSBN; // type of binning
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

    std::vector<bool> fLNTZER; // logical flag for printing only non-zero cells
    std::vector<int>  fBKUSBN; // 1st Birk's law parameter for this binning (meaningful only for energy scoring)
    std::vector<int>  fB2USBN; // 2nd Birk's law parameter for this binning (meaningful only for energy scoring) 
    std::vector<float>  fTCUSBN; // time cut-off (seconds) for this binning

    int fIRECRD;
    
    std::vector<int> fJB;

    //    std::vector<bool> fLSNGBN; // logical flag for zero precision scoring of data
    std::vector<bool> fLTRKBN; // logical flag for flagging track-length binnings
    int  fNUSRBN; // number of user defined binnings used
    bool fLSTATI;

    std::vector<int> fKBUSBN; // initial location in blank common of the considered binning (for index 1, real*8 address for lsngbn = false, real*4 address for lsngbn = true)

    std::vector< std::vector<float> > fScored; // GMSTOR?
    std::vector< std::vector<float> > fGBSTOR;

  public:
    UsbSuw(const char *fname);
    virtual ~UsbSuw();
    void Reset();
    bool Read();

    inline const int GetMCASE() const {return fMCASE;}
    inline const int GetNBATCH() const {return fNBATCH;}
    inline const int GetNbins(int i) const {return fNXBIN[i]*fNYBIN[i]*fNZBIN[i];}

    void Print() const;
  };
}

#endif
