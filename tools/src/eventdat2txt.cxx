/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
 */

#include "EventDat.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_SCORED;
  ReadFluka::EventDat *eventdat = new ReadFluka::EventDat(argv[1]);

  //cout << scientific << setprecision(4);
  cout << "  **** Event-Data ****" << endl;
  cout << eventdat->GetRunTitle() << endl;
  cout << eventdat->GetRunTime() << endl;
  cout << "No. of regions. " << eventdat->GetNregs() << " No. of distr. scored " << eventdat->GetNsco() << endl;
  cout << endl;

  while (eventdat->ReadEvent() == true) {
    cout << "Event # " << eventdat->GetEntryNumber() << endl;
    // actually, instead of "." one should format the cout output since the dot is a part of the preceeding number
    cout << "Primary Weight " << eventdat->GetWEIPRU();
    cout << " Primary Energy " << eventdat->GetENETOT() << " GeV" << endl;

    cout << endl << "Contributions to the energy deposition" << endl << "(GeV not normalized to the weight):" << endl;
    cout << uppercase << setprecision(9);
    for (unsigned short i=0; i<NENDIST; i++)
      cout << eventdat->GetENDIST(i) << " ";
    cout << endl;

    for (unsigned short n=0; n<eventdat->GetNsco(); n++) {
      cout << endl << "Generalized scoring distribution # " << eventdat->GetSco(n) << endl << "from first to last region:" << endl;
      int max = 5;
      for (unsigned int i=0; i<eventdat->GetNregs(); i++) {
	cout << eventdat->GetValue(eventdat->GetSco(n), i) << " ";
	if (eventdat->GetValue(eventdat->GetSco(n), i) == 0.0f) max++;
	if ( (i+1)%max == 0) {
	  cout << endl;
	  //	max++;
	}
      }
      cout << endl;
    }

    cout << endl << "Seeds after event # " << eventdat->GetEntryNumber() << endl;
    cout << "*** ";
    for (unsigned short i=0; i<10; i++) {
      cout << hex;
      if (i==0) cout << eventdat->GetSeed(0);
      else if (i==1) cout << eventdat->GetSeed(1);
      else if (i==6) cout << eventdat->GetSeed(2);
      else if (i==7) cout << eventdat->GetSeed(3);
      else cout << 0;
      cout << dec << " ";
    }
    cout << " ***" << endl;
  }

  delete eventdat; eventdat = 0;
  return 0;
}
