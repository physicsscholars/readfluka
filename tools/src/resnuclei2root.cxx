/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
 */

#include "ROOT_ResNuclei.h"
#include <iostream>
#include "TString.h"
#include "TFile.h"
#include "TH2.h"

using namespace std;

int usage()
{
  cout << "Usage: resnuclei2root /path/to/resnuclei_binary_output [out.root]" << endl;
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
  ReadFluka::ROOT_ResNuclei *resnuclei = new ReadFluka::ROOT_ResNuclei(fname_in.Data());

  TFile *file = TFile::Open(fname_out.Data(), "recreate");

  while (resnuclei->Read()) {
    resnuclei->Histogram()->Write();
    clog << resnuclei->GetBinName() << "  " << flush;
  }
  clog << endl;

  file->Close();

  SafeDelete(resnuclei);

  return 0;
}
