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

#include "ReadFluka.h"
#define NENDIST 12

class EventDat : public ReadFluka {
  UInt_t                fNREGS;        // number of regions
  UInt_t                fNSCO;         // number of scoring distributions requested by SCORE
  Int_t                *fSco;          // array of scoring distributions
  Int_t                *fISCORE;       // Ith requested (generalized) particle distribution
  Float_t               fWEIPRU;       // primary weight
  Float_t               fENETOT;       // primary particle total energy (GeV)
  Float_t              *fENDIST;       // 12 energy contributions to the total energy balance
  Float_t              *fREGSCO;       // fNREGS values corresponding for scoring distributions ( not normalized )
  Int_t                 fSeed[4];     // These 4 numbers are necessary to reproduce a given event: fSeeds[0,1]-counters, fSeeds[2,3]-seeds

  Bool_t                fIsReadHeader;             // true if header was read, otherwise false
  void                  fReadHeader();
  void                  fReadSeeds();
  void                  fReadENDIST();
  void                  fReadScoredDistributions();
 public:
  EventDat(const char *fname);
  virtual              ~EventDat();
  Bool_t                ReadEvent(); // read 1 event from the data file
  const UInt_t          GetNregs() const {return fNREGS;}
  const UInt_t          GetNsco()  const {return fNSCO;}
  const Int_t           GetSco(UInt_t i) const {return fSco[i];}
  const Float_t         GetENDIST(UInt_t i) const {return (i<NENDIST) ? fENDIST[i] : 0;}
  Float_t              *GetENDIST() const {return fENDIST;}
  const Float_t         GetValue(UInt_t region) const { return (region<=fNREGS) ? fREGSCO[region-1] : 0;}
  Int_t                *GetSeed() { return fSeed;}
};

#endif
