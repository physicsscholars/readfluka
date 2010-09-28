#include "ROOT_ResNuclei.h"
#include <iostream>

using namespace ReadFluka;

ROOT_ResNuclei::ROOT_ResNuclei(const char *fname) : ResNuclei(fname)
{

}

TString ROOT_ResNuclei::GetBinTitle() const
{
  TString str = ResNuclei::GetBinTitle();
  str = str + ";" + GetXtitle() + ";" + GetYtitle() + ";" + GetZtitle();
  return str;
}

TH2F *ROOT_ResNuclei::Histogram() const
{
  Float_t zmin = GetZmin();
  Float_t zmax = GetZmax();

  Float_t amin = GetAmin();
  Float_t amax = GetAmax();


  TH2F *h = new TH2F(GetBinName().data(), ROOT_ResNuclei::GetBinTitle().Data(), 
  		     GetNbinsZ(),   zmin, zmax-zmin+1,
  		     GetNbinsZ()*2, amin, GetNbinsZ()*2-amin+1); // can't use GetNbinsA() since it might change from sample to sample -> by default set it to 2*GetNbinsZ()
  //  		     GetNbinsA()+2, amin, amax+2);

  h->SetBit(TH2::kIsAverage); // see NOTE1 in the source code of hadd.cxx
  h->Sumw2(); // it must go before filling the histo!

  //  std::cout << h->GetName() << " " << h->GetNbinsX() << " " << h->GetXaxis()->GetXmin() << " " << h->GetXaxis()->GetXmax() << std::endl;
  // std::cout << h->GetName() << " " << h->GetNbinsY() << " " << h->GetYaxis()->GetXmin() << " " << h->GetYaxis()->GetXmax() << std::endl;

  float tmp;
  for (int i=0; i<GetIZRHGH(); i++)
    for (int j=0; j<GetIMRHGH(); j++) {
      tmp = GetRNDATA()[j][i];
      //      cout << "\t" << i+1 << " " << j+1 << " " << GetK() << endl;
      if (tmp>0) {
 	//h->SetBinContent(i+1, GetA(i,j), tmp);
 	//h->SetBinError(i+1, GetA(i,j), 0.0);
	h->Fill(i+1, GetA(i,j), tmp);
      }
    }
  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 

 
 return h;
}
