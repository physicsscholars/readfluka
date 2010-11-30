#include "UstSuw.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UstSuw *ustsuw = new ReadFluka::UstSuw(argv[1]);
  
  cout << "**** " << argv[1] << " ****" << endl;
  //  cout << ustsuw->GetRunTitle() << endl;
  //cout << ustsuw->GetRunTime() << endl;
  cout << "Total primaries run:\t" << ustsuw->GetEntryNumber() << endl;
  cout << "Total weight of the primaries run:\t" << ustsuw->GetWeight() << endl;

  int i=0;
  ustsuw->Read();
  ustsuw->Print();
  i++;
  

  delete ustsuw;
  
  return 0;
}
