#include "UsbSuw.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsbSuw *usbsuw = new ReadFluka::UsbSuw(argv[1]);
  
  /*cout << "***** " << usbsuw->GetRunTitle() << " *****" << endl;
  cout << usbsuw->GetRunTime() << endl;
  cout << "Total number of particles followed " << usbsuw->GetEntryNumber();
  cout << ", for a total weight of " << usbsuw->GetWeight() << endl;
  */

  usbsuw->Read();
  //while (usbsuw->Read()) {
    usbsuw->Print();
    //}

  delete usbsuw;
 
  return 0;
}
