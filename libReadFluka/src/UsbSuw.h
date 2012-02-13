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
    float fWCTOT; // total weight
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

    int fN; // total number of histograms (USRBIN records)

  public:
    UsbSuw(const char *fname);
    virtual ~UsbSuw();
    void Reset();
    bool Read();

    float GetScored(int i, int x, int y, int z) const;
    inline float GetScored(int ihist, int ibin) const {return fScored[ihist][ibin];}
    float  GetError(int i, int x, int y, int z) const;
    inline  float GetError(int ihist, int ibin) const {return fGBSTOR[ihist][ibin];}

    inline const int GetMCASE() const {return fMCASE;}
    inline const int GetNBATCH() const {return fNBATCH;}
    inline const int GetNbins(int i) const {return fNXBIN[i]*fNYBIN[i]*fNZBIN[i];}
    inline const int GetN() const {return fN;} // total number of histograms (USRBIN records)

    inline std::string GetTITUSB(int ihist) const { return fTITUSB[ihist]; }
    inline int GetIDUSBN(int ihist) const {return fIDUSBN[ihist]; }
    inline int GetNXBIN(int ihist) const {return fNXBIN[ihist]; }
    inline int GetNYBIN(int ihist) const {return fNYBIN[ihist]; }
    inline int GetNZBIN(int ihist) const {return fNZBIN[ihist]; }

    inline float GetXLOW(int ihist) const {return fXLOW[ihist]; } // retun Xlow of the histogram number 'ihist'
    // retun Xlow of the bin number 'bin' in the binning number 'ihist'
    inline float GetXLOW(int ihist, int bin) const {return fXLOW[ihist] + fDXUSBN[ihist]*bin;} 
    inline float GetYLOW(int ihist) const {return fYLOW[ihist]; }
    float GetYLOW(int ihist, int bin) const {return fYLOW[ihist] + fDYUSBN[ihist]*bin;} 
    inline float GetZLOW(int ihist) const {return fZLOW[ihist]; }
    float GetZLOW(int ihist, int bin) const {return fZLOW[ihist] + fDZUSBN[ihist]*bin;} 

    inline float GetXHIGH(int ihist) const {return fXHIGH[ihist]; }
    // retun XHIGH of the bin number 'bin' in the binning number 'ihist'
    float GetXHIGH(int ihist, int bin) const {return GetXLOW(ihist, bin) + fDXUSBN[ihist];}
    inline float GetYHIGH(int ihist) const {return fYHIGH[ihist]; }
    inline float GetYHIGH(int ihist, int bin) const {return GetYLOW(ihist, bin) + fDYUSBN[ihist];} 
    inline float GetZHIGH(int ihist) const {return fZHIGH[ihist]; }
    inline float GetZHIGH(int ihist, int bin) const {return GetZLOW(ihist, bin) + fDZUSBN[ihist];} 


    void Print() const;
    void Print1() const; // for YJL
  };
}

#endif
