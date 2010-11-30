#include "ROOT_UstSuw.h"
#include <iostream>
#include "TFile.h"
#include "TH1F.h"

using namespace std;

int usage()
{
  cout << "Usage: ustsuw2root /path/to/usrsuw_binary_output [out.root]" << endl;
  return 1;
}


int main(int argc, const char **argv)
{
  if (argc == 1) return usage();

  TString fname_in(argv[1]);
  TString fname_out;
  if (argc == 3) fname_out = argv[2];
  else if (argc == 2) fname_out = fname_in + ".root";
  else return usage();

  clog << fname_in << "  ->  " << fname_out << ":\t" << flush;

  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::ROOT_UstSuw *ustsuw = new ReadFluka::ROOT_UstSuw(fname_in.Data());

  TFile *file = TFile::Open(fname_out.Data(), "recreate");

  ustsuw->Read();
  //ustsuw->HistTotalResponse(0)->Write();
  //ustsuw->HistTotalTrackLength(0)->Write();
  ustsuw->Histograms()->Write();
  
  clog << endl;

  file->Close();

  SafeDelete(ustsuw);

  delete ustsuw;
  
  return 0;
}
