#ifndef UsrBdx_h
#define UsrBdx_h

///////////////////////////////////////////////
//
// UsrBdx - a class to read the
// USRBDX estimator output.
//
///////////////////////////////////////////////

#include <fstream>
#include <vector>
#include <string>
#include "Base.h"
#include <cmath>

namespace ReadFluka {
  class UsrBdx : public Base {
  private:
    int        fMX;        // USRBDX card number

    std::string fTITUSX; // bdrx name
    int         fITUSBX; // type of the binning (WHAT(1): -1, -2, 1, 2)
    int         fIDUSBX; // distribution to be scored
    int         fNR1USX; // 1st region
    int         fNR2USX; // 2nd region
    float       fAUSBDX; // area of the detector
    bool        fLWUSBX; // one way if false, two ways if true
    bool        fLFUSBX; // current if false, fluence if true
    bool        fLLNUSX; // no low energy neutron scoring if false, yes if true

    float       fEBXLOW; // min energy
    float       fEBXHGH; // max energy

    unsigned int         fNEBXBN; // number of energy intervals
    float       fDEBXBN; // energy bin width

    float       fABXLOW; // min angle
    float       fABXHGH; // min angle
    unsigned int         fNABXBN; // number of angular intervals
    float       fDABXBN; // angular (steradian) bin width 

    int         fIGMUSX; // maximum low energy neutron group to be scored
    float      *fENGMAX; // array of the low energy neutron data

    int         fNScored; // number of scored values (INTERV*fNABXBN)
    float      *fScored;  // array with scored data

    bool       fIsReadHeader;
    void       ReadHeader();
    void       Reset();
  public:
    UsrBdx(const char *fname);
    virtual ~UsrBdx();

    bool Read();

    inline float GetArea() const { return fAUSBDX; }
    inline int GetCardNumber() const { return fMX; }
    std::string GetBinName() const {return fTITUSX; }
    std::string GetBinTitle() const;
    inline int GetID() const { return fIDUSBX; }
    inline int GetRegFrom() const { return fNR1USX; }
    inline int GetRegTo() const { return fNR2USX; }
    inline bool IsReadNeutrons() const { return fLLNUSX; }
    inline int GetMaxNeutronGroup() const { return fIGMUSX; }
    inline float GetENGMAX(int i) const { return fENGMAX[i]; }
    inline bool IsOneWay() const { return !fLWUSBX; }

    inline bool GetLFUSBX() const { return fLFUSBX; }
    inline bool IsFluence() const { return GetLFUSBX(); }
    inline bool IsCurrent() const { return !IsFluence(); }

    inline int  GetType() const { return fITUSBX; }

    inline float GetEmin() const { return fEBXLOW; }
    inline float GetEmax() const { return fEBXHGH; }
    inline unsigned int GetNbinsE() const { return fNEBXBN; }
    inline float GetEWidth() const { return fDEBXBN; }

    inline float GetAmin() const { return fABXLOW; }
    inline float GetAmax() const { return fABXHGH; }
    inline unsigned int GetNbinsA() const { return fNABXBN; }
    inline float GetAwidth() const { return fDABXBN; }

    inline int GetNScored() const { return fNScored; }
    inline float *GetScored() const { return fScored; }
    float GetScored(unsigned int ie, unsigned int ia) const;

    inline bool IsLogE() const {return !(GetType()>0); }
    inline bool IsLogA() const { return !(int(std::abs(float(GetType())))<=1); }

    const char* GetXtitle() const;
    const char* GetYtitle() const;

    void Print() const;
  };
}


#endif
