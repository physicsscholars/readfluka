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
  		     GetNbinsZ(),   zmin, zmax,
  		     GetNbinsA()+2, amin, amax+2);

  //  std::cout << h->GetName() << " " << h->GetNbinsX() << " " << h->GetXaxis()->GetXmin() << " " << h->GetXaxis()->GetXmax() << std::endl;
  // std::cout << h->GetName() << " " << h->GetNbinsY() << " " << h->GetYaxis()->GetXmin() << " " << h->GetYaxis()->GetXmax() << std::endl;

  float tmp;
  for (int i=0; i<GetIZRHGH(); i++)
    for (int j=0; j<GetIMRHGH(); j++) {
      tmp = GetRNDATA()[j][i];
      //      cout << "\t" << i+1 << " " << j+1 << " " << GetK() << endl;
      if (tmp>0)
	h->SetBinContent(i+1, GetA(i,j), tmp);
    }
 
 
  h->SetEntries(GetEntryNumber());
 
 return h;
}
