#include "EventDat.h"

EventDat::EventDat(const char* fname) : ReadFluka(fname)
{
  // constructor. fname is a name of a file with EVENTDAT output
  fENDIST = new Float_t[NENDIST];
  fNCASE=fNREGS=fNSCO=0;
  fWEIPRU=fENETOT=0.0;

  fSco = 0;

  fIsReadHeader = false; //kFALSE;
  fReadHeader();
  fREGSCO = new Float_t[fNREGS];
  for (UInt_t i=0; i<fNREGS; i++) fREGSCO[i] = -1;
  for (UInt_t i=0; i<4; i++) fSeed[i] = 0;
}

EventDat::~EventDat()
{
  if (gVerbose) cout << "EventDat read " << fNCASE << " events" << endl << endl;
  delete [] fSco; fSco = 0;
  delete [] fENDIST; fENDIST = 0;
  delete [] fREGSCO; fREGSCO = 0;
}


void EventDat::fReadHeader()
{
  // read a header: RUNTIT, RUNTIM, NREGS, NSCO, ISCORE[NSCO]
  
  if (fIsReadHeader == false) { // did not read the header yet?
    fIsReadHeader = true;
    // read run title:
    ReadRunTitle();

    // read run time:
    ReadRunTime();

    fNREGS = ReadInt(); // number of regions 
    if (gVerbose) cout << "number of regions: " << fNREGS << endl;
    fNSCO = ReadInt();  // number of scoring distributions
    if (gVerbose) cout << "number of scoring distributions: " << fNSCO << " ->\t";
    
    fSco = new Int_t[fNSCO];
    for (UInt_t i=0; i<fNSCO; i++) {
      fSco[i] = ReadInt();
      if (gVerbose) cout << fSco[i] << ' ';
    }
    if (gVerbose) cout << endl;
    ReadInt(2);
  }
}

void EventDat::fReadENDIST()
{
  // read 12 energy contributions to the total energy balance ( NOT normalized to the weight of primary)

  fin->read((Char_t *)fENDIST, sizeof(Float_t)*NENDIST); // 48=4*12
}

void EventDat::fReadScoredDistributions()
{
  // read scored distributions
  Float_t F[4];
  Int_t iisc;
  Int_t iscore[fNSCO];
  for (UInt_t isc=0; isc<fNSCO; isc++) {
    fin->read((Char_t *)&iisc, 4);
    fin->read((Char_t *)&iscore[isc], 4);
    
    if (gVerbose>2) cout << isc+1 << ": " << iisc << " " << iscore[isc] << endl;
    if (iisc != (Int_t)isc+1) 
      if (gVerbose>2) cerr << "Wrong sequence: " << iisc << " != " << isc+1 << endl;

    if (gVerbose>2) cout << "strange numbers: ";
    for (Int_t i=0; i<4; i++) {
      F[i] = ReadFloat();
      if (gVerbose>2) cout << F[i] << " ";
    }
    
    for (UInt_t region=0; region<fNREGS; region++) {
      fREGSCO[region] = ReadFloat();
    }
    
    //    if (gVerbose>2) cout << endl << "dummy records: " << endl;
    
    // read 3 dummy records
    ReadInt(3);
    
    Int_t checker = ReadInt();
    
    if (checker < 0) {
      //      cout << "float before seeds: ";
      for (Int_t i=0; i<3; i++) {
	F[0] = ReadFloat();
	//	cout << F[0] << " ";
      }
      //      cout << endl;
      fReadSeeds();
    } else {
      fin->seekg(-8, ios::cur);
    }
  }
}

void EventDat::fReadSeeds()
{
  if (gVerbose>=0) cout << endl << "seeds after this event:\t";
  Int_t iTmp;
  for (Int_t i=0; i<10; i++)  {
    iTmp = ReadInt();
    if (i==0) fSeed[0] = iTmp;
    if (i==1) fSeed[1] = iTmp;
    if (i==6) fSeed[2] = iTmp;
    if (i==7) fSeed[3] = iTmp;
    if (gVerbose>=0) cout << hex << iTmp << dec << " ";
  }
  cout << '\t';
  if (true) cout << ReadFloat() << " " << ReadFloat(); else ReadFloat(2);
  if (gVerbose>=0) cout << endl;
}

Bool_t EventDat::ReadEvent()
{
    // read 1 event from the data file
  Int_t tmp = ReadInt();
  if (!fin->good()) return false;
  fNCASE = tmp;
  if (gVerbose>1) clog << "reading event #: " << fNCASE << endl;
  fWEIPRU = ReadFloat();
  fENETOT = ReadFloat();
  
  ReadFloat(2); // do not understand the meaning of these numbers
  
  fReadENDIST();
  fReadScoredDistributions();
  
  return true;
}
