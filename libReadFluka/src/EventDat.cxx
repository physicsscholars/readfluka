#include "EventDat.h"

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
  if (gVerbose) cout << "EventDat read " << fNCASE << " events" << endl << endl;
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
    if (gVerbose) cout << "number of regions: " << fNREGS << endl;
    fNSCO = ReadInt();  // number of scoring distributions
    if (gVerbose) cout << "number of scoring distributions: " << fNSCO << " ->\t";
    
    fISCORE = new int[fNSCO];
    for (unsigned int i=0; i<fNSCO; i++) {
      fISCORE[i] = ReadInt();
      if (gVerbose) cout << fISCORE[i] << ' ';
    }
    if (gVerbose) cout << endl;
    ReadInt(2);
  }
}

void EventDat::fReadENDIST()
{
  /*
    read 12 energy contributions to the total energy balance ( NOT normalized to the weight of primary)
  */

  fin->read((char *)fENDIST, sizeof(float)*NENDIST); // 48=4*12
}

void EventDat::fReadScoredDistributions()
{
  // read scored distributions
  float F[4];
  int iisc;
  int iscore[fNSCO];
  //  vector <float> val; // scored values
  for (unsigned int isc=0; isc<fNSCO; isc++) {
    vector <float> val;
        fin->read((char *)&iisc, 4);
        fin->read((char *)&iscore[isc], 4);
	//ReadInt(iisc);
	//ReadInt(iscore[isc]);

    
    if (gVerbose>kPRINT_MISC) cout << "isc: " << isc+1 << ": " << iisc << " " << iscore[isc] << endl;
    if (iisc != (int)isc+1) 
      if (gVerbose>kPRINT_MISC) cerr << "Wrong sequence: " << iisc << " != " << isc+1 << endl;

    if (gVerbose>kPRINT_MISC) cout << "strange numbers: ";
    for (int i=0; i<4; i++) {
      F[i] = ReadFloat();
      if (gVerbose>kPRINT_MISC) cout << F[i] << " ";
    }
    
    for (unsigned int region=0; region<fNREGS; region++) {
      val.push_back(ReadFloat());
    }

    fREGSCO.push_back(val);
    //    val.clear();
    
    //    if (gVerbose>2) cout << endl << "dummy records: " << endl;
    
    // read 3 dummy records
    ReadInt(3);
    
    int checker = ReadInt();
    
    if (checker < 0) {
      //      cout << "float before seeds: ";
      for (int i=0; i<3; i++) {
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
  if (gVerbose>kPRINT_MISC) cout << endl << " seeds after this event:\t";
  int iTmp;
  for (int i=0; i<10; i++)  {
    iTmp = ReadInt();
    if (i==0) fSeed[0] = iTmp;
    if (i==1) fSeed[1] = iTmp;
    if (i==6) fSeed[2] = iTmp;
    if (i==7) fSeed[3] = iTmp;
    if (gVerbose>kPRINT_MISC) cout << hex << iTmp << dec << " ";
  }
  cout << '\t';
  if (gVerbose>kPRINT_MISC) cout << ReadFloat() << " " << ReadFloat(); else ReadFloat(2);
  if (gVerbose>kPRINT_NOTHING) cout << endl;
}

bool EventDat::ReadEvent()
{
    // read 1 event from the data file
  int tmp = ReadInt();
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

const float EventDat::GetValue(int id, unsigned int region) const
{
  int n;
  for (unsigned short i=0; i<GetNsco(); i++) {
    if (GetSco(i)==id) n = i;
  }
  //  cout << id << " "  << n << endl;
  return (region<fNREGS) ? fREGSCO[n][region] : 0;
}
