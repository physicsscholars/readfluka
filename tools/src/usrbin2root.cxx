/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
 */

#include "ROOT_UsrBin.h"
#include <iostream>
#include "TString.h"
#include "TFile.h"
#include "TH3.h"

using namespace std;

int usage()
{
  cout << "Usage: usrbin2root /path/to/usrbin_binary_output [out.root]" << endl;
  return 1;
}


int main(int argc, const char **argv)
{
  if (argc == 1) return usage();

  TString fname_in(argv[1]);
  TString fname_out;
  if (argc == 3) fname_out = argv[2];  else fname_out = fname_in + ".root";

  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::ROOT_UsrBin *usrbin = new ReadFluka::ROOT_UsrBin(fname_in.Data());

  TFile *file = TFile::Open(fname_out.Data(), "recreate");

  while (usrbin->Read()) {
    usrbin->Histogram()->Write();
  }

  file->Close();

  SafeDelete(usrbin);
  
  return 0;
}
