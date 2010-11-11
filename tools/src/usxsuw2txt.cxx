#include "UsxSuw.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsxSuw *usxsuw = new ReadFluka::UsxSuw(argv[1]);
  
  cout << "***** " << usxsuw->GetRunTitle() << " *****" << endl;
  cout << usxsuw->GetRunTime() << endl;
  cout << "Total number of particles followed " << usxsuw->GetEntryNumber() << flush;
  cout << ", for a total weight of ";
  cout.precision(8);
  cout << scientific << usxsuw->GetWeight() << endl;
  
  while (usxsuw->Read()) {
    usxsuw->Print();
}

  delete usxsuw;
  
  return 0;
}
