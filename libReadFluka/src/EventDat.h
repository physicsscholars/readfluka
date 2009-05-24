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
using namespace std;

#include "Base.h"
#define NENDIST 12
#define NSEED 4

namespace ReadFluka {
  class EventDat : public Base {
    unsigned int                fNREGS;        // number of regions
    unsigned int                fNSCO;         // number of scoring distributions requested by SCORE
    int                *fSco;          // array of scoring distributions
    int                *fISCORE;       // Ith requested (generalized) particle distribution
    float               fWEIPRU;       // primary weight
    float               fENETOT;       // primary particle total energy (GeV)
    float              *fENDIST;       // 12 energy contributions to the total energy balance
    float              *fREGSCO;       // fNREGS values corresponding for scoring distributions ( not normalized )
    int                 fSeed[NSEED];     // These NSEED numbers are necessary to reproduce a given event: fSeeds[0,1]-counters, fSeeds[2,3]-seeds
    
    bool                fIsReadHeader;             // true if header was read, otherwise false
    void                  fReadHeader();
    void                  fReadSeeds();
    void                  fReadENDIST();
    void                  fReadScoredDistributions();
  public:
    EventDat(const char *fname);
    virtual              ~EventDat();
    bool                ReadEvent(); // read 1 event from the data file
    const unsigned int GetNregs() const {return fNREGS;}
    const unsigned int GetNsco()  const {return fNSCO;}
    const int           GetSco(unsigned int i) const {return fSco[i];}
    const float         GetENDIST(unsigned int i) const {return (i<NENDIST) ? fENDIST[i] : 0;}
    float              *GetENDIST() const {return fENDIST;}
    const float         GetValue(unsigned int region) const { return (region<=fNREGS) ? fREGSCO[region-1] : 0;}
    int                *GetSeed() { return fSeed;}
    const int           GetSeed(unsigned short i) const {return fSeed[i];}
  };
}

#endif
