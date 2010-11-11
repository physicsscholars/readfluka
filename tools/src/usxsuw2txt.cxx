#include "UsxSuw.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsxSuw *usxsuw = new ReadFluka::UsxSuw(argv[1]);
  
  cout << usxsuw->GetRunTitle() << endl;
  cout << usxsuw->GetRunTime() << endl;
  cout << "Total number of particles followed " << usxsuw->GetEntryNumber() << flush;
  cout << ", for a total weight of ";
  cout << usxsuw->GetWeight() << endl;

  int i=0;
  usxsuw->Read();
  //usxsuw->Print(i);
  i++;
  

  delete usxsuw;
  
  return 0;
}
