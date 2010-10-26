/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
 */

#include "EventDat.h"
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <sstream>

using namespace std;

int usage()
{
  cout << "Usage: eventdat2root /path/to/eventdat_binary_output [out.root]" << endl;
  cout << "\tArray indices in FLUKA start from ONE (FORTRAN style), but here they start from ZERO (C++ style)." << endl;
  cout << "\tSo, the region number in the root file produced is fulka region minus ONE." << endl;
  cout << "\tImplemented for a single scored distribution only." << endl << endl;
  return 1;
}

string code2name(ReadFluka::EventDat *ed)
{
  ostringstream str;
  int id;
  int nregs = ed->GetNregs();
  for (int bin=0; bin<ed->GetNsco(); bin++) {
    id = ed->GetSco(bin);
    str << ed->Code2Name(id) << "[" << nregs << "]/F";
    if (bin+1 != ed->GetNsco()) str << ":";
  }
  return str.str();
}

int main(int argc, const char **argv)
{
  if (argc < 2) return usage();
  
  TString fname_in(argv[1]);
  TString fname_out;
  if (argc == 3) 
    fname_out = argv[2];
  else 
    fname_out = fname_in + ".root";
  
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_MISC;
  ReadFluka::EventDat *eventdat = new ReadFluka::EventDat(fname_in.Data());
  
  TString title = Form("%s\t%s", eventdat->GetRunTitle(), eventdat->GetRunTime()); cout << title << endl;
  const UInt_t Nregs = eventdat->GetNregs();
  cout << "number of regions: " << Nregs << "\t";
  const UInt_t Nsco = eventdat->GetNsco();
  cout << "number of scored distributions: " << Nsco << endl;
  /*if (Nsco!=1) {
    cerr << "currently only one distribution at a time supported" << endl;
    return NOT_IMPLEMENTED;
    }*/
  
  TFile *file = new TFile(fname_out.Data(), "recreate", title.Data());
  if (file->IsZombie()) {
    cerr << "Can't create " << fname_out.Data() << endl;
    return CANT_OPEN_FILE;
  }
  file->SetTitle(argv[1]);

  Float_t *Ed = new Float_t[Nsco*Nregs];  //  memset(Ed, 0, sizeof(Float_t)*Nregs);
  Int_t   *seed = new Int_t[NSEED];
  Float_t *endist = new Float_t[NENDIST];

  TTree *tree = new TTree("EVENTDAT", title.Data());
  //  tree->Branch("DATA", Ed,  Form("%s[%d]/F", eventdat->Code2Name(208), Nregs));
  tree->Branch("DATA", Ed, code2name(eventdat).data());
  tree->Branch("SEEDS", seed, Form("seed[%d]/I", NSEED));
  tree->Branch("ENDIST", endist, Form("endist[%d]/F", NENDIST));


  while (eventdat->ReadEvent() == kTRUE) {
    for (unsigned int bin=0; bin<Nsco; bin++) {
      for (unsigned int reg=0; reg<Nregs; reg++) {
	Ed[bin+reg*Nsco] = eventdat->GetValue(eventdat->GetSco(bin), reg+1);
	///if (reg == 2) cout << "Ed[2]: " << Ed[reg] << endl;
      }
    }

    for (unsigned short iseed=0; iseed<NSEED; iseed++)
      seed[iseed] = eventdat->GetSeed(iseed);

    for (unsigned short i=0; i<NENDIST; i++)
      endist[i] = eventdat->GetENDIST(i);

    tree->Fill();
  }
  file->Write();
  file->Close();

  //  cout << "delete tree:" << endl;
  //  delete tree;
  delete [] endist; endist = 0;
  delete [] seed; seed = 0;
  delete [] Ed; Ed = 0;
  SafeDelete(file);
  SafeDelete(eventdat);

  return 0;
}
