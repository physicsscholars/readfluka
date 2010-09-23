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
  
  resnuclei->Read();

  /*cout << "NRN: " << fNRN << endl;
  cout << "TIURSN: " << fTIURSN << endl;
  cout << "ITURSN: " << fITURSN << endl;
  cout << "NRURSN: " << fNRURSN << endl;
  cout << "VURSNC: " << fVURSNC << " cm3" << endl;
  cout << "IMRHGH: " << fIMRHGH << endl;
  cout << "IZRHGH: " << fIZRHGH << endl;
  cout << "K: " << fK << endl;*/

  if (abs(resnuclei->GetITURSN())<1) { // see page 211
    //    cout << "Res. nucl #" << fNRN << " " << fTIURSN << ", high energy products, region " << fITURSN << fNRURSN << fVURSNC << fIMRHGH + fK << fK+1 << endl;
    cout << "Res. nuclei n. " << resnuclei->GetNRN() << " \"" << resnuclei->GetTIURSN() 
	 << "\", 'high' energy products, region n. " << resnuclei->GetNRURSN() << endl;
    cout << "\tdetector volume: " << resnuclei->GetVURSNC() << " cm**3" << endl;
    cout << "\tMax. Z: " << resnuclei->GetIZRHGH();
    cout << ", Max. N-Z: " << resnuclei->GetIMRHGH() + resnuclei->GetK();
    cout << ", Min. N-Z: " << resnuclei->GetK()+1 << endl;
  } else if (abs(resnuclei->GetITURSN())<2) {
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
  
  cout << "\tZ\tA\tresidual nuclei" << endl;
  cout << "\t\t\tper cm**3 per primary" << endl;

  /*  float tmp;
  for (int i=0; i<resnuclei->GetIZRHGH(); i++)
    for (int j=0; j<resnuclei->GetIMRHGH(); j++) {
      tmp = resnuclei->GetRNDATA()[i][j];
      cout << "\t" << i+1 << " " << j+1 << " " << resnuclei->GetK() << endl;
      cout << "\t" << i+1 << "\t" << j+1+resnuclei->GetK()+2*(i+1) << "\t" << tmp << endl;
      }*/
  delete resnuclei;
  
  return 0;
}
