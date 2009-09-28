#ifndef Usxsuw_h
#define Usxsuw_h

///////////////////////////////////////////////
//
// Usxsuw - a class to read the binary output of usxsuw - the FLUKA tool to analyse and combine the
// USRBDX estimator output.
//
///////////////////////////////////////////////

#include <fstream>
#include <vector>
#include <string>
#include "Base.h"

namespace ReadFluka {
  class Usxsuw : public Base {
  private:
    int        fMCASE;
    int        fNBATCH;
    int        fMBATCH;
    int        fRECORD;
    int        fNCTOT;
    int        fMCTOT;
    double     fWCTOT;            // weight
    int        fNX;
    int        fMX;

    std::string fTITUSX; // bdrx name
    int         fIDUSBX; // distribution to be scored
    int         fNR1USX; // 1st region
    int         fNR2USX; // 2nd region
    float       fAUSBDX; // area of the detector
    bool        fLWUSBX; // one way if false, two ways if true
    bool        fLFUSBX; // current if false, fluence if true
    bool        fLLNUSX; // no low energy neutron scoring if false, yes if true

    float       fEBXLOW; // min energy
    float       fEBXHGH; // min energy

    int         fNEBXBN; // number of energy intervals
    float       fDEBXBN; // energy bin width

    float       fABXLOW; // min angle
    float       fABXHGH; // min angle
    int         fNABXBN; // number of angular intervals
    float       fDABXBN; // angular (steradian) bin width 

    int         fIRECRD; // record number;

    int         fIGMUSX; // maximum low energy neutron group to be scored
    //    const int   fNMXGRP(300);
    float      *fENGMAX;

    bool       fIsReadHeader;
    void       ReadHeader();
    void       Reset();
  public:
    Usxsuw(const char *fname);
    virtual ~Usxsuw();

    void Read();

    void Print() const;
  };
}


#endif
