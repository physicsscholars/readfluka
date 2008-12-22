#include "UsrYield.h"
#include <TFile.h>
#include <TObjArray.h>
#include <Riostream.h>
using namespace std;
 
int main(int argc, char **argv)
{
  UsrYield *data = new UsrYield(argv[1]);
  data->SetVerboseLevel(1);

  while (data->Read()) {
    cout << endl;
  }

  TFile f("out.root", "recreate");
  data->GetHistograms()->Write();
  f.Close();
 
  delete data;
  return 0;
}
