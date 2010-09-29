#include "UsrSuw.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsrSuw *usrsuw = new ReadFluka::UsrSuw(argv[1]);
  
  cout << "***** " << usrsuw->GetRunTitle() << " *****" << endl;
  cout << usrsuw->GetRunTime() << endl;
  cout << "Total number of particles followed " << usrsuw->GetEntryNumber();
  cout << ", for a total weight of " << usrsuw->GetWeight() << endl;
  
  while (usrsuw->Read()) {

        if (abs((float)usrsuw->GetITURSN())<1) { // see page 211
      cout << "Res. nuclei n. " << usrsuw->GetNRN() << " \"" << usrsuw->GetTIURSN() 
	   << "\", 'high' energy products, region n. " << usrsuw->GetNRURSN() << endl;
      cout << "\tdetector volume: " << usrsuw->GetVURSNC() << " cm**3" << endl;
      cout << "\tMax. Z: " << usrsuw->GetIZRHGH();
      cout << ", Max. N-Z: " << usrsuw->GetIMRHGH() + usrsuw->GetK();
      cout << ", Min. N-Z: " << usrsuw->GetK()+1 << endl;
    } else if (abs((float)usrsuw->GetITURSN())<2) {
      cout << "Res. nuclei n. " << usrsuw->GetNRN() << " \"" << usrsuw->GetTIURSN() 
	   << "\", 'low' energy products, region n. " << usrsuw->GetNRURSN() << endl;
      cout << "\tdetector volume: " << usrsuw->GetVURSNC() << " cm**3" << endl;
      cout << "\tMax. Z: " << usrsuw->GetIZRHGH();
      cout << ", Max. N-Z: " << usrsuw->GetIMRHGH() + usrsuw->GetK();
      cout << ", Min. N-Z: " << usrsuw->GetK()+1 << endl;
    } else {
      cout << "Res. nuclei n. " << usrsuw->GetNRN() << " \"" << usrsuw->GetTIURSN() 
	   << "\", all products, region n. " << usrsuw->GetNRURSN() << endl;
      cout << "\tdetector volume: " << usrsuw->GetVURSNC() << " cm**3" << endl;
      cout << "\tMax. Z: " << usrsuw->GetIZRHGH();
      cout << ", Max. N-Z: " << usrsuw->GetIMRHGH() + usrsuw->GetK();
      cout << ", Min. N-Z: " << usrsuw->GetK()+1 << endl;
    }  
    
    usrsuw->Print();
  }

  int Z = 12;
  int A = 22;
  // cout << "Z=" << Z << "\tA=" << A << "\t" << usrsuw->GetRNDATA(Z, A) << endl;
  //cout << "Zmax = " << usrsuw->GetZmax() << endl;
  //cout << "Amax = " << usrsuw->GetAmax() << endl;

  delete usrsuw;
 
  return 0;
}
