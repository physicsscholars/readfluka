
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

    if (abs((float)resnuclei->GetITURSN())<1) { // see page 211
      cout << "Res. nuclei n. " << resnuclei->GetNRN() << " \"" << resnuclei->GetTIURSN() 
	   << "\", 'high' energy products, region n. " << resnuclei->GetNRURSN() << endl;
      cout << "\tdetector volume: " << resnuclei->GetVURSNC() << " cm**3" << endl;
      cout << "\tMax. Z: " << resnuclei->GetIZRHGH();
      cout << ", Max. N-Z: " << resnuclei->GetIMRHGH() + resnuclei->GetK();
      cout << ", Min. N-Z: " << resnuclei->GetK()+1 << endl;
    } else if (abs((float)resnuclei->GetITURSN())<2) {
      cout << "Res. nuclei n. " << resnuclei->GetNRN() << " \"" << resnuclei->GetTIURSN() 
	   << "\", 'low' energy products, region n. " << resnuclei->GetNRURSN() << endl;
      cout << "\tdetector volume: " << resnuclei->GetVURSNC() << " cm**3" << endl;
      cout << "\tMax. Z: " << resnuclei->GetIZRHGH();
      cout << ", Max. N-Z: " << resnuclei->GetIMRHGH() + resnuclei->GetK();
      cout << ", Min. N-Z: " << resnuclei->GetK()+1 << endl;
    } else {
      cout << "Res. nuclei n. " << resnuclei->GetNRN() << " \"" << resnuclei->GetTIURSN() 
	   << "\", all products, region n. " << resnuclei->GetNRURSN() << endl;
      cout << "\tdetector volume: " << resnuclei->GetVURSNC() << " cm**3" << endl;
      cout << "\tMax. Z: " << resnuclei->GetIZRHGH();
      cout << ", Max. N-Z: " << resnuclei->GetIMRHGH() + resnuclei->GetK();
      cout << ", Min. N-Z: " << resnuclei->GetK()+1 << endl;
    }  
    
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
