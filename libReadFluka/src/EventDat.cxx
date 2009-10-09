#include "EventDat.h"
#include <cstdlib>

using namespace ReadFluka;

EventDat::EventDat(const char* fname) : Base(fname)
{
  /*
    Constructor: fname is a name of a file with EVENTDAT output
  */

  fENDIST = new float[NENDIST];
  fNCASE=fNREGS=fNSCO=0;
  fWEIPRU=fENETOT=0.0;

  fISCORE = 0;

  fIsReadHeader = false;
  fReadHeader();
  //  fREGSCO = new float[fNREGS];
  //  for (unsigned int i=0; i<fNREGS; i++) fREGSCO[i] = -1;
  for (unsigned int i=0; i<4; i++) fSeed[i] = 0;
}

EventDat::~EventDat()
{
  if (gVerbose) clog << "EventDat read " << fNCASE << " events" << endl << endl;
  delete [] fISCORE; fISCORE = 0;
  delete [] fENDIST; fENDIST = 0;
  //  delete [] fREGSCO; fREGSCO = 0;
  fREGSCO.clear();
}


void EventDat::fReadHeader()
{
  /*
    Read the header: RUNTIT, RUNTIM, NREGS, NSCO, ISCORE[NSCO]
  */
  
  if (fIsReadHeader == false) { // did not read the header yet?
    fIsReadHeader = true;
    // read run title:
    ReadRunTitle();

    // read run time:
    ReadRunTime();

    fNREGS = ReadInt(); // number of regions 
    if (gVerbose>kPRINT_MISC) clog << "number of regions: " << fNREGS << endl;

    fNSCO = ReadInt();  // number of scoring distributions
    if (gVerbose>kPRINT_MISC) clog << "number of scoring distributions: " << fNSCO << " ->\t";
    
    fISCORE = new int[fNSCO];
    if (gVerbose>kPRINT_MISC) clog << "scored distributions:\t";
    for (unsigned int i=0; i<fNSCO; i++) {
      fISCORE[i] = ReadInt();
      if (gVerbose>kPRINT_MISC) clog << fISCORE[i] << ' ';
    }
    if (gVerbose>kPRINT_MISC) clog << endl;
    //    ReadInt(); // should be SizeEnd, but for which SizeStart?
    SizeEnd();
  }
}

void EventDat::fReadENDIST()
{
  /*
    Read 12 energy contributions to the total energy balance ( NOT normalized to the weight of primary)
  */

  fin->read((char *)fENDIST, sizeof(float)*NENDIST); // 48=4*12
}

void EventDat::fReadScoredDistributions()
{
  /* 
     read scored distributions
  */

  int iisc;
  int iscore[fNSCO];
  vector <float> val; // scored values
  for (unsigned int isc=0; isc<fNSCO; isc++) {
    //  vector <float> val;
    //ReadInt(iisc);
    fin->read((char *)&iisc, 4);
    fin->read((char *)&iscore[isc], 4);
    //    clog << isc << "\treading distribution " << iscore[isc] << endl;
    
    if (iisc != (int)(isc+1)) {
      cerr << "EventDat::fReadScoredDistributins:\tWrong sequence: " << iisc << " != " << isc+1 << endl;
      exit(WRONG_FORMAT);
    }

    CheckFormat(); 

    if (gVerbose>kPRINT_MISC) clog << "Reading the data for each " << fNREGS << " regions ..." << flush;

    val.clear();
    for (unsigned int region=0; region<fNREGS; region++) val.push_back(ReadFloat());
    fREGSCO.push_back(val);
    if (gVerbose>kPRINT_MISC) clog << " done" << endl;

    CheckFormat();
    
    // one dummy record (for historical reasons)
    //    ReadInt();

    int ndum = ReadInt();
    float dum1 = ReadFloat();
    float dum2 = ReadFloat();
    
    if (dum1 < 0) {
      fReadSeeds();
    } else {
      //      clog << "****************** seek" << endl;
      fin->seekg(-12, ios::cur);
    }
  }
}

void EventDat::fReadSeeds()
{
  CheckFormat(); 
  //  if (gVerbose>kPRINT_MISC) clog << endl << "seeds after this event:\t";
  int iTmp;
  for (int i=0; i<10; i++)  {
    iTmp = ReadInt();
    if (i==0) fSeed[0] = iTmp;
    if (i==1) fSeed[1] = iTmp;
    if (i==6) fSeed[2] = iTmp;
    if (i==7) fSeed[3] = iTmp;
    //    if (gVerbose>=kPRINT_MISC) clog << hex << iTmp << dec << " ";
  }
  //  if (gVerbose>kPRINT_MISC) clog << endl;
  //clog << "******** reading seeds done *** " << endl;
}

bool EventDat::ReadEvent()
{
    // read 1 event from the data file
  if (fNCASE==0) SizeStart();
  fREGSCO.clear();
  
  fNCASE = ReadInt();
  if (!fin->good()) return false;
  if (gVerbose>kPRINT_MISC) clog << "reading event #: " << fNCASE << endl;
  fWEIPRU = ReadFloat();
  fENETOT = ReadFloat();
  
  CheckFormat();
  fReadENDIST();

  CheckFormat();
  fReadScoredDistributions();
  //  ReadInt(4);
  CheckFormat(); 
  
  return true;
}

const float EventDat::GetValue(int id, unsigned int region) const
{
  int n;
  for (unsigned short i=0; i<GetNsco(); i++) {
    if (GetSco(i)==id) n = i;
  }
  //  clog << id << " "  << n << endl;
  return (region<fNREGS) ? fREGSCO[n][region] : 0;
}
