
#include "ResNuclei.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::ResNuclei *resnuclei = new ReadFluka::ResNuclei(argv[1]);
  
  cout << "***** " << resnuclei->GetRunTitle() << " *****" << endl;
  cout << resnuclei->GetRunTime() << endl;
  cout << "Total number of particles followed " << resnuclei->GetEntryNumber();
  cout << ", for a total weight of " << resnuclei->GetWeight() << endl;
  
  while (resnuclei->Read()) {
    resnuclei->Print();
  }

  int Z = 12;
  int A = 22;
  cout << "Z=" << Z << "\tA=" << A << "\t" << resnuclei->GetRNDATA(Z, A) << endl;
  cout << "Zmax = " << resnuclei->GetZmax() << endl;
  cout << "Amax = " << resnuclei->GetAmax() << endl;

  delete resnuclei;
 
  return 0;
}
