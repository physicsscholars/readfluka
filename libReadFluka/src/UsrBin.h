#ifndef UsrBin_h
#define UsrBin_h

/////////////////////////////////////////////////////
//
// UsrBin
//
// Class to read FLUKA's USRBIN output
// The numbering of fScored array is like this:
// 
//          | y
//    ------------|
//    |     |     |
//    |  3  |  4  |
//    |     |     |
//    ------o------- x
//    |     |     |
//    |  1  |  2  |
//    |     |     |
//    |------------
//
////////////////////////////////////////////////////

#include <fstream>
#include <vector>
#include "Base.h"

namespace ReadFluka {
  class UsrBin : public Base {
  private:
    char       *fTITUSB;         // title of scored distribution
    int         fITUSBN;         // type of binning (must be decoded if > 10)
    
    //		float       fWEIPRI;         // weight of primary particle
    int         fIDUSBN;         // distribution to be scored
    
    float       fXLOW;           // minimum value of x
    float       fXHIGH;          // maximum value of x
    float       fYLOW;           // minimum value of y
    float       fYHIGH;          // maxumum value of y
    float       fZLOW;           // minimum value of z
    float       fZHIGH;          // maximum value of z
    
    int         fNXBIN;          // number of x intervals
    int         fNYBIN;          // number of y intervals
    int         fNZBIN;          // number of z intervals
    
    float       fDXUSBN;         // x bin width
    float       fDYUSBN;         // y bin width
    float       fDZUSBN;         // z bin width
    
    bool        fIsReadHeader;   // true if the header has already been read
    void        fReadHeader();   // reads the header
    
    bool        fReadBinning();   // read the common part of binnings
    bool        fReadCartesian(); // reads the cartesian binning
    bool        fReadRegions();   // reads the region binning
    
    bool        fCheckBounds(int x, int y=1, int z=1) const;
    bool        fFirstRead;
    
  protected:
    float      *fScored;          // array of scored values
    std::vector<float> fScoredVec;
  public:
    UsrBin(const char *fname);
    virtual ~UsrBin();
    
    const float GetXmin() const {return fXLOW;}
    const float GetXmin(int i, int j=1, int k=1) const;
    
    const float GetXmax() const {return fXHIGH;}
    const float GetXmax(int i, int j=1, int k=1) const;
    
    const float GetYmin() const {return fYLOW;}
    const float GetYmin(int i, int j, int k=1) const;
    
    const float GetYmax() const {return fYHIGH;}
    const float GetYmax(int i, int j, int k=1) const;
    
    const float GetZmin() const {return fZLOW;}
    const float GetZmin(int i, int j, int k) const;
    
    const float GetZmax() const {return fZHIGH;}
    const float GetZmax(int i, int j, int k) const;
    const float GetVolume(int i, int j, int k) const;
    
    inline const int GetNbinsX() const {return fNXBIN;}
    inline const int GetNbinsY() const {return fNYBIN;}
    inline const int GetNbinsZ() const {return fNZBIN;}
    inline const int GetNbins()  const {return fNXBIN*fNYBIN*fNZBIN; }
    
    const float GetdX() const {return fDXUSBN;}
    const float GetdY() const {return fDYUSBN;}
    const float GetdZ() const {return fDZUSBN;}
    
    const int GetDistType() const {return fIDUSBN;}

    const int GetBinType() const {return fITUSBN;}
    inline const int GetITUSBN() const {return fITUSBN;}

    const float *GetScored() const {return fScored;}
    float  GetScored(int x, int y=1, int z=1) const;
    std::vector<float> GetScoredVec() const {return fScoredVec;}
		
    const char *GetBinName() const {return fTITUSB;}
    inline const char *GetTITUSB() const {return fTITUSB;}

    bool      Read(); // read 1 user binning
    void Print() const;
  };
}

#endif
