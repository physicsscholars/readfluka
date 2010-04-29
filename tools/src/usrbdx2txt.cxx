#include "UsrBdx.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsrBdx *usrbdx = new ReadFluka::UsrBdx(argv[1]);
  
  cout << "***** " << usrbdx->GetRunTitle() << " *****" << endl;
  cout << usrbdx->GetRunTime() << endl;
  cout << "Total number of particles followed " << usrbdx->GetEntryNumber() << flush;
  cout << ", for a total weight of " << usrbdx->GetWeight() << endl;
  
  while (usrbdx->Read()) {
    cout << "1" << endl;
    cout << "Bdrx n. " << usrbdx->GetCardNumber() << " \"" << usrbdx->GetBinName() << "\"" <<  flush;
    cout << ", generalized particle n. " << usrbdx->GetID() << flush;
    cout << ", from region n. " << usrbdx->GetRegFrom() << " to region n. " << usrbdx->GetRegTo() << endl;
    cout << "\tdetector area: " << usrbdx->GetArea() <<  " cm**2" << endl;
    if (usrbdx->IsReadNeutrons()) {
      cout << "\tlow energy neutrons scored from group 1" << " to group " << usrbdx->GetMaxNeutronGroup() << endl;
    }
    if (usrbdx->IsOneWay() == true) 
      cout << "\tthis is a two ways estimator" << endl; 
    else
      cout << "\tthis is a one way only estimator" << endl;
    
    if (usrbdx->IsFluence() == true) 
      std::cout << "\tthis is a fluence like estimator" << std::endl;
    else
      std::cout << "\tthis is a current like estimator" << std::endl;
    
    if (!usrbdx->IsLogE()) {
      cout << "\tlinear energy binning from " << usrbdx->GetEmin() << " to " << usrbdx->GetEmax()  << " GeV,\t" << flush;
      cout << usrbdx->GetNbinsE() << " bins ("  << usrbdx->GetEWidth() <<  " GeV wide)" << endl;
    } else {
      cout << "\tlogar. energy binning from " << usrbdx->GetEmin() << " to " << usrbdx->GetEmax() << " GeV,\t" << flush;
      cout << usrbdx->GetNbinsE() << " bins (ratio: " << usrbdx->GetEWidth() << ")" << endl;
    }
    
    if (!usrbdx->IsLogA()) {
      cout << "\tlinear angular binning from "<<usrbdx->GetAmin()<<" to "<< usrbdx->GetAmax() << " sr,\t"<< flush;
      cout <<  usrbdx->GetNbinsA() << " bins (" << usrbdx->GetAwidth() << " sr wide)"  << endl;
    } else {
      cout << "\tlogar. angular binning from "<<usrbdx->GetAmin()<<" to "<< usrbdx->GetAmax() << " sr,\t"<< flush;
      cout <<  usrbdx->GetNbinsA() << " bins (ratio: " << usrbdx->GetAwidth() << ")"  << endl;
    }
    
    cout << "\tData follow in a matrix A(ie,ia), format (1(5x,1p,10(1x,e11.4)))" << endl;
    int n = usrbdx->GetNScored();
    n = usrbdx->GetNbinsE()*usrbdx->GetNbinsA();
    cout << endl << "\t";
    for (int i=1; i<=usrbdx->GetNbinsA(); i++) {
      for (int j=1; j<=usrbdx->GetNbinsE(); j++)
	cout << usrbdx->GetScored(j, i) << " ";
      cout << endl << "\t";
    }
    cout << endl;
    
    
    if (usrbdx->IsReadNeutrons()) {
      cout << "\tLow energy neutron data from group 1 to group " << usrbdx->GetMaxNeutronGroup() <<  " follow in a matrix A(ig,ia), format (1(5x,1p,10(1x,e11.4)))" << endl;
      
      
      cerr << "!!! check it again !!! WHY ZEROS? why do we need the neutron data?" << endl;
      
      
      //  n = usrbdx->GetMaxNeutronGroup();
      //  for (int i=0; i<n; i++) cout << usrbdx->GetENGMAX(i) << " "; cout << endl;
      n = usrbdx->GetNScored();
      //      for (int i= usrbdx->GetNbinsE()*usrbdx->GetNbinsA(); i<n; i++)  cout << usrbdx->GetScored(i) << " "; 
      cout << endl << endl;
    }
  }

  delete usrbdx;
  
  return 0;
}
