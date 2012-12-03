#include "UsxSuw.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsxSuw *usxsuw = new ReadFluka::UsxSuw(argv[1]);
  
  cout << "**** " << argv[1] << " ****" << endl;
  cout << usxsuw->GetRunTitle() << endl;
  //cout << usxsuw->GetRunTime() << endl;
  cout << "Total primaries run:\t" << usxsuw->GetEntryNumber() << endl;
  cout << "Total weight of the primaries run:\t" << usxsuw->GetWeight() << endl;

  usxsuw->Read();
  usxsuw->Print();

  delete usxsuw;
  
  return 0;
}
