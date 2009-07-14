/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
 */

#include "EventDat.h"
#include <TFile.h>
#include <TTree.h>
#include <iostream>

using namespace std;

int usage()
{
  cout << "Usage: eventdat2root /path/to/eventdat_binary_output [out.root]" << endl;
  cout << "\tArray indices in FLUKA start from ONE (FORTRAN style), but here they start from ZERO (C++ style)." << endl;
  cout << "\tSo, the region number in the root file produced is fulka region minus ONE." << endl;
  cout << "\tImplemented for a single scored distribution only." << endl << endl;
  return 1;
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
  UInt_t Nsco = eventdat->GetNsco();
  cout << "number of scored distributions: " << Nsco << endl;

  TFile *file = new TFile(fname_out.Data(), "recreate", title.Data());
  file->SetTitle(argv[1]);

  Float_t *Ed = new Float_t[Nregs];  //  memset(Ed, 0, sizeof(Float_t)*Nregs);
  Int_t   *seed = new Int_t[NSEED];
  Float_t *endist = new Float_t[NENDIST];

  TTree *tree = new TTree("EVENTDAT", title.Data());
  tree->Branch("DATA", Ed,  Form("Ed[%d]/F", Nregs));
  tree->Branch("SEEDS", seed, Form("seed[%d]/I", NSEED));
  tree->Branch("ENDIST", endist, Form("endist[%d]/F", NENDIST));

  while (eventdat->ReadEvent() == kTRUE) {
    for (unsigned int reg=0; reg<Nregs; reg++) {
      Ed[reg] = eventdat->GetValue(208, reg+1);
      if (reg == 2) cout << "Ed[2]: " << Ed[reg] << endl;
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
  delete [] endist;
  delete [] seed;
  delete [] Ed;
  SafeDelete(file);

  return 0;
}
