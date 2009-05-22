#include "EventDat.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::EventDat *eventdat = new ReadFluka::EventDat(argv[1]);
  cout << eventdat->GetRunTitle() << "\t" << eventdat->GetRunTime() << endl;
  cout << "number of regions: " << eventdat->GetNregs() << endl;

  TFile *file = new TFile("a.root", "recreate");
  file->SetTitle(eventdat->GetRunTitle());
  TNtuple *nt = new TNtuple("nt", "nt", "reg:Ed");

  while (eventdat->ReadEvent() == kTRUE) {
    //    nt->Fill(
  }
  return 0;
}
