#include "UsrBin.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, const char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsrBin *usrbin = new ReadFluka::UsrBin(argv[1]);
  
  cout << scientific << setprecision(4);
  
  cout << " *****  " << usrbin->GetRunTitle() << " *****" << endl;
  cout << '\t' << usrbin->GetRunTime() << endl;
  cout << "\tTotal number of particles followed\t" << usrbin->GetEntryNumber();
  cout << ", for a total weight of  ";
  cout << usrbin->GetWeight() << endl;
  
  while (usrbin->Read()) {
    if (usrbin->GetBinType() % 10 == 0) {
      cout << 1 << endl << "   Cartesian binning n. ";
      if (usrbin->GetBinNumber() == 128) cout << 2; // is it correct? why it's like that?
      else { 
	/*
	  we add ONE because we are comparing the binary output with the following USRBIN card,
	  which writes the text file and goes after the current one in the input file
	*/
	cout <<  usrbin->GetBinNumber()+1; 
      }
      cout << " \""  << usrbin->GetBinName() << "\" , ";
      cout << "generalized particle n. " << usrbin->GetDistType() << endl;
      
      cout << "\tX coordinate: from " << usrbin->GetXmin() << " to " << usrbin->GetXmax() << " cm, " << 
	usrbin->GetNbinsX() << " bins (" << usrbin->GetdX() << " cm wide)" <<endl;
      cout << "\tY coordinate: from " << usrbin->GetYmin() << " to " << usrbin->GetYmax() << " cm, " <<
	usrbin->GetNbinsY() << " bins (" << usrbin->GetdY() << " cm wide)" << endl;
      cout << "\tZ coordinate: from " << usrbin->GetZmin() << " to " << usrbin->GetZmax() << " cm, " <<
	usrbin->GetNbinsZ() << " bins (" << usrbin->GetdZ() << " cm wide)" << endl;
      
      cout<<"\tData follow in a matrix A(ix,iy,iz), format (1(5x,1p,10(1x,e11.4)))" << endl << endl;
      if (usrbin->GetBinType()>=10) cout<<"\taccurate deposition along the tracks requested" << endl;
      if (usrbin->GetDistType()<200) cout<<"\tthis is a track-length binning" << endl; // is it correct?
      
      for (int i=0; i<usrbin->GetNbins(); i++) {
	if (i==0) cout << '\t';
	cout << usrbin->GetScored()[i] << " ";
	if ((i+1)%10 == 0) cout << endl << '\t';
      }
    }
  }
  delete usrbin;
  
  return 0;
}
