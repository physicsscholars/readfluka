#ifndef UstSuw_h
#define UstSuw_h

/*
  UstSuw - a class to read the $FLUTIL/ustsuw binary output (USRTRACK)
 */

#include <vector>
#include <string>
#include "Base.h"

namespace ReadFluka {
  class UstSuw : public Base {
  private:
    int fMTC; // ???
    std::vector<std::string> fTITUTC; // track or coll name
    std::vector<int> fITUSTC; // type of binning: 1 - linear energy, track; 2 - linear energy, coll; -1 - logarithmic energy, track; -2 - logarithmic energy, coll
    std::vector<int>   fIDUSTC; // distribution to be scored
    std::vector<int>   fNRUSTC; // region
    std::vector<float> fVUSRTC; // volume [cm^3] of the dectctor
    std::vector<bool>  fLLNUTC; // low energy neutron scoring if true
    std::vector<float> fETCLOW; // minimum energy
    std::vector<float> fETCHGH; // maximum energy
    std::vector<int>   fNETCBN; // number of energy intervals
    std::vector<float> fDETCBN; // energy bin width

    /*    std::vector<float> fATCLOW; // Remove this???? 
    std::vector<float> fATCHGH;
    std::vector<int>   fNATCBN;
    std::vector<float> fDATCBN;*/

    std::vector<int> fIGMUTC; // maximum low energy group to be scored
    std::vector<float> fENGMAX;
    std::vector<float> fGMSTOR; // data

    std::vector<float> fTotResp;
    std::vector<float> fTotRespErr;

    std::vector< std::vector<float> > fEPGMAX;

    std::vector<float> fFlux;
    std::vector<float> fFluxErr;

    std::vector<float> fFluxLEN; // low energy neutron flux
    std::vector<float> fFluxLENErr;

    std::vector<float> fCumulFlux;
    std::vector<float> fCumulFluxErr;

    std::vector<float> fCumulFluxLEN; // low energy neutron flux
    std::vector<float> fCumulFluxLENErr;

    unsigned int fNRecords; // total number of records (USRTRACK cards)
    bool fIsReadHeader; // true if the header has already been read
    bool ReadHeader();

    void PrintEnergyBoundaries(int record) const;
    void PrintLowEnergyBoundaries(int record) const;
  public:
    UstSuw(const char *fname);
    virtual ~UstSuw();

    void Reset();
    bool Read();

    inline int GetIGMUTC(int i) const { return fIGMUTC[i]; }
    inline int GetMaxNeutronGroup(int i) const { return GetIGMUTC(i); }
    
    inline int GetNETCBN(int i) const { return fNETCBN[i]; }
    inline int GetNbinsE(int i) const { return GetNETCBN(i); }
    inline int GetNbinsTotal(int i) const {return GetNbinsE(i)+GetMaxNeutronGroup(i);}

    inline float GetVUSRTC(int i) const {return fVUSRTC[i];}
    inline float GetVolume(int i) const {return GetVUSRTC(i);}

    inline float GetTotResp(int i) const {return fTotResp[i];} // [p/cmq/pr]
    inline float GetTotRespErr(int i) const {return fTotRespErr[i];} // [%]
    
    inline float GetTrackLength(int i) const {return GetTotResp(i)*GetVolume(i);} // [cm/pr]

    inline float GetFlux(int i) const {return fFlux[i];}
    inline float GetFluxErr(int i) const {return fFluxErr[i];}

    inline float GetFluxLEN(int i) const {return fFluxLEN[i];}
    inline float GetFluxLENErr(int i) const {return fFluxLENErr[i];}

    inline float GetCumulFlux(int i) const {return fCumulFlux[i];}
    inline float GetCumulFluxErr(int i) const {return fCumulFluxErr[i];}

    inline float GetCumulFluxLEN(int i) const {return fCumulFluxLEN[i];}
    inline float GetCumulFluxLENErr(int i) const {return fCumulFluxLENErr[i];}

    inline bool GetLLNUTC(int i) const { return fLLNUTC[i]; }
    inline bool IsReadNeutrons(int i) const { return GetLLNUTC(i); }

    float GetLowestBoundary(int record) const;

    void Print(int i) const;
    void Print() const;
  };
}

#endif
