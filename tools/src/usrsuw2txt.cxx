#include "UsrSuw.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsrSuw *usrsuw = new ReadFluka::UsrSuw(argv[1]);
  
  /*cout << "***** " << usrsuw->GetRunTitle() << " *****" << endl;
  cout << usrsuw->GetRunTime() << endl;
  cout << "Total number of particles followed " << usrsuw->GetEntryNumber();
  cout << ", for a total weight of " << usrsuw->GetWeight() << endl;
  */

  while (usrsuw->Read()) {
    //usrsuw->Print();
  }

  int Z = 12;
  int A = 22;
  // cout << "Z=" << Z << "\tA=" << A << "\t" << usrsuw->GetRNDATA(Z, A) << endl;
  //cout << "Zmax = " << usrsuw->GetZmax() << endl;
  //cout << "Amax = " << usrsuw->GetAmax() << endl;

  delete usrsuw;
 
  return 0;
}
