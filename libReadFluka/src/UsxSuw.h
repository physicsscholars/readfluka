#ifndef UsxSuw_h
#define UsxSuw_h

///////////////////////////////////////////////
//
// UsxSuw - a class to read the
// $FLUTIL/usxsuw estimator output                       !!! NOT YET FINISHED !!!
//
///////////////////////////////////////////////

#include <fstream>
#include <vector>
#include <string>
#include "Base.h"
#include <cmath>

namespace ReadFluka {
  class UsxSuw : public Base {
  private:
    //int        fNX;        // USRBDX card number
    float      fWCTOT; // total weight
    int        fNCTOT; // total number of incident particles
    int        fMCTOT;
    int        fMBATCH;

    std::vector<std::string> fTITUSX; // bdrx name
    std::vector<int> fITUSBX; // type of the binning (WHAT(1): -1, -2, 1, 2)
    std::vector<int> fIDUSBX; // distribution to be scored
    std::vector<int> fNR1USX; // 1st region
    std::vector<int> fNR2USX; // 2nd region
    std::vector<float> fAUSBDX; // area of the detector
    std::vector<bool> fLWUSBX; // one way if false, two ways if true
    std::vector<bool> fLFUSBX; // current if false, fluence if true
    std::vector<bool> fLLNUSX; // low energy neutron scoring if true

    std::vector<float> fEBXLOW; // min energy
    std::vector<float> fEBXHGH; // max energy

    std::vector<unsigned int> fNEBXBN; // number of energy intervals
    std::vector<float> fDEBXBN; // energy bin width

    std::vector<float> fABXLOW; // min angle
    std::vector<float> fABXHGH; // max angle
    std::vector<unsigned int> fNABXBN; // number of angular intervals
    std::vector<float> fDABXBN; // angular (steradian) bin width 

    std::vector<int> fIGMUSX; // maximum low energy neutron group to be scored (normally 260)
    std::vector< std::vector<float> > fENGMAX; // array of the low energy neutron data
    std::vector< std::vector<float> > fEPGMAX; // energy boundaries for each distribution

    std::vector<int> fNScored; // number of scored values (INTERV*fNABXBN)
    std::vector< std::vector<float> > fScored;  // array with scored data
    std::vector< std::vector<float> > fGDSTOR;

    std::vector<float> fTOTTOT; // total responce not normalized by GeV and sr. See also UsxSuw::GetTotalResponce(unsigned int) const

    bool       fIsReadHeader;
    void       ReadHeader();
    void       Reset();

  public:
    UsxSuw(const char *fname);
    virtual ~UsxSuw();

    bool Read();

    inline int GetEntryNumber() const { return fNCTOT; }
    inline float GetWeight() const { return fWCTOT; }

    inline float GetArea(int i) const { return fAUSBDX[i]; }
    //    inline int GetCardNumber() const { return fNX; }
    std::string GetBinName(int i) const {return fTITUSX[i]; }
    std::string GetBinTitle(int i) const;
    inline int GetID(int i) const { return fIDUSBX[i]; }
    inline int GetRegFrom(int i) const { return fNR1USX[i]; }
    inline int GetRegTo(int i) const { return fNR2USX[i]; }

    inline bool GetLLNUSX(int i) const { return fLLNUSX[i]; }
    inline bool IsReadNeutrons(int i) const { return GetLLNUSX(i); }

    inline int GetIGMUSX(int i) const { return fIGMUSX[i]; }
    inline int GetMaxNeutronGroup(int i) const { return GetIGMUSX(i); }

    //    inline float GetENGMAX(int i) const { return fENGMAX[i]; }
    inline bool IsOneWay(int i) const { return !fLWUSBX[i]; }

    inline bool GetLFUSBX(int i) const { return fLFUSBX[i]; }
    inline bool IsFluence(int i) const { return GetLFUSBX(i); }
    inline bool IsCurrent(int i) const { return !IsFluence(i); }

    inline int  GetType(int i) const { return fITUSBX[i]; }

    inline float GetEmin(int i) const { return fEBXLOW[i]; }
    inline float GetEmax(int i) const { return fEBXHGH[i]; }
    inline unsigned int GetNbinsE(int i) const { return fNEBXBN[i]; }
    inline float GetEWidth(int i) const { return fDEBXBN[i]; }

    inline float GetAmin(int i) const { return fABXLOW[i]; }
    inline float GetAmax(int i) const { return fABXHGH[i]; }

    inline unsigned int GetNABXBN(int i) const { return fNABXBN[i]; }
    inline unsigned int GetNbinsA(int i) const { return GetNABXBN(i); }
    inline float GetAwidth(int i) const { return fDABXBN[i]; }

    inline int GetNScored(int i) const { return fNScored[i]; }
    //    inline float *GetScored() const { return fScored; }
    float GetScored(unsigned int ie, unsigned int ia) const;

    inline bool IsLogE(int i) const {return !(GetType(i)>0); }
    inline bool IsLogA(int i) const { return !(int(std::abs(float(GetType(i))))<=1); }

    const char* GetXtitle(int i) const;
    const char* GetYtitle(int i) const;

    std::vector<float> GetALowEdge(unsigned int i) const;
    float GetAwidthRAD(unsigned int i, unsigned int bin) const;

    std::vector<float> GetELowEdge(unsigned int i) const;

    float GetTotalResponce(unsigned int i) const;

    // void XbinsE_len() const;

    void Print(int i) const;
  };
}


#endif
