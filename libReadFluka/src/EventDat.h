#ifndef EventDat_h
#define EventDat_h

///////////////////////////////////////////////////////
//                                                   //
//   EventDat                                       // 
//                                                   //
//   a class to read FLUKA's EVENTDAT output         //
//                                                   //
///////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "Base.h"
#define NENDIST 12
#define NSEED 4

namespace ReadFluka {
  class EventDat : public Base {
    unsigned int        fNREGS;        // number of regions
    unsigned int        fNSCO;         // number of scoring distributions requested by SCORE
    int                *fISCORE;       // [fNSCO] Ith requested (generalized) particle distribution
    float               fWEIPRU;       // primary weight
    float               fENETOT;       // primary particle total energy (GeV)
    float              *fENDIST;       // NENDIST energy contributions to the total energy balance (see the FLUKA manual)
    vector< vector<float> > fREGSCO;       // fNREGS values corresponding for scoring distributions ( not normalized )
    int                 fSeed[NSEED];     // These NSEED numbers are necessary to reproduce a given event: fSeeds[0,1]-counters, fSeeds[2,3]-seeds
    
    bool                fIsReadHeader;             // true if header was read, otherwise false
    void                fReadHeader();
    void                fReadSeeds();
    void                fReadENDIST();
    void                fReadScoredDistributions();
  public:
    EventDat(const char *fname);
    virtual              ~EventDat();
    bool                ReadEvent(); // read 1 event from the data file
    const unsigned int  GetNregs() const {return fNREGS;}
    const unsigned int  GetNsco()  const {return fNSCO;}
    const float         GetWEIPRU() const {return fWEIPRU; }
    const float         GetENETOT() const {return fENETOT; }
    const int           GetSco(unsigned int i) const {return (i<fNSCO) ? fISCORE[i] : 0; }
    const float         GetENDIST(unsigned int i) const {return (i<NENDIST) ? fENDIST[i] : 0;}
    float              *GetENDIST() const {return fENDIST;}
    const float         GetValue(int id, unsigned int region) const;
    int                *GetSeed() { return fSeed;}
    const int           GetSeed(unsigned short i) const {return fSeed[i];}
  };
}

#endif
