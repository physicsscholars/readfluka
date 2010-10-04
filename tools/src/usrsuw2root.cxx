/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
 */

#include "ROOT_UsrSuw.h"
#include <iostream>
#include "TString.h"
#include "TFile.h"
#include "TH2.h"

using namespace std;

int usage()
{
  cout << "Usage: usrsuw2root /path/to/usrsuw_binary_output [out.root]" << endl;
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
  ReadFluka::ROOT_UsrSuw *usrsuw = new ReadFluka::ROOT_UsrSuw(fname_in.Data());

  TFile *file = TFile::Open(fname_out.Data(), "recreate");

  while (usrsuw->Read()) {
    usrsuw->Histograms()->Write();
    usrsuw->HistogramsA()->Write();
     usrsuw->HistogramsZ()->Write();
  }
  clog << endl;

  file->Close();

  SafeDelete(usrsuw);

  return 0;
}
