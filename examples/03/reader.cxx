#include "EventDat.h"
#include "ROOT_UsrBin.h"
#include <TFile.h>
#include <TTree.h>
#include <iostream>

using namespace std;

int usage()
{
  cerr << "Usage: reader /path/to/eventdat_binary_output /path/to/usrbin_binary_output [out.root]" << endl;
  return 1;
}

int main(int argc, const char **argv)
{
  if (argc<4) return usage();

  TString fname_eventdat(argv[1]);
  TString fname_usrbin(argv[2]);
  TString fname_out(argv[3]);

  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::EventDat *eventdat = new ReadFluka::EventDat(fname_eventdat.Data());
  ReadFluka::ROOT_UsrBin *usrbin = new ReadFluka::ROOT_UsrBin(fname_usrbin.Data());

  TString title = Form("%s\t%s", eventdat->GetRunTitle(), eventdat->GetRunTime()); cout << title << endl;
  const UInt_t Nregs = eventdat->GetNregs();
  //  UInt_t Nsco = eventdat->GetNsco();

  TFile *file = new TFile(fname_out.Data(), "recreate", title.Data());
  file->SetTitle(title.Data());

  Float_t *Ed = new Float_t[Nregs];  //  memset(Ed, 0, sizeof(Float_t)*Nregs);
  Int_t   *seed = new Int_t[NSEED];
  Float_t *endist = new Float_t[NENDIST];

  TTree *tree = new TTree("EVENTDAT", title.Data());
  tree->Branch("DATA", Ed,  Form("Ed[%d]/F", Nregs));
  tree->Branch("SEEDS", seed, Form("seed[%d]/I", NSEED));
  tree->Branch("ENDIST", endist, Form("endist[%d]/F", NENDIST));

  while (eventdat->ReadEvent() == kTRUE) {
    for (unsigned int reg=0; reg<Nregs; reg++) {
      Ed[reg] = eventdat->GetValue(reg+1);
    }

    for (unsigned short iseed=0; iseed<NSEED; iseed++)
      seed[iseed] = eventdat->GetSeed(iseed);

    for (unsigned short i=0; i<NENDIST; i++)
      endist[i] = eventdat->GetENDIST(i);

    tree->Fill();
  }

  // write the histograms:
  while (usrbin->Read()) {
    usrbin->Histogram()->Write();
  }


  tree->Write();
  file->Close();

  //  cout << "delete tree:" << endl;
  //  delete tree;
  delete [] endist;
  delete [] seed;
  delete [] Ed;
  SafeDelete(file);
  cout << endl;
  return 0;
}
