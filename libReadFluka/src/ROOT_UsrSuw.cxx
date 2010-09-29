#include "ROOT_UsrSuw.h"
#include <iostream>

using namespace ReadFluka;

ROOT_UsrSuw::ROOT_UsrSuw(const char *fname) : UsrSuw(fname)
{

}

TString ROOT_UsrSuw::GetBinTitle() const
{
  TString str = UsrSuw::GetBinTitle();
  str = str + ";" + GetXtitle() + ";" + GetYtitle() + ";" + GetZtitle();
  return str;
}

TString ROOT_UsrSuw::GetBinTitleA() const
{
  TString str = UsrSuw::GetBinTitle();
  str = str + ";" + GetXtitle() + ";" + GetZtitle();
  return str;
}

TString ROOT_UsrSuw::GetBinTitleZ() const
{
  TString str = UsrSuw::GetBinTitle();
  str = str + ";" + GetYtitle() + ";" + GetZtitle();
  return str;
}

TH2F *ROOT_UsrSuw::Histogram() const
{
  Int_t zmin = GetZmin();
  Int_t zmax = GetZmax();

  Int_t amin = GetAmin();
  Int_t amax = GetAmax();


  TH2F *h = new TH2F(GetBinName().data(), ROOT_UsrSuw::GetBinTitle().Data(), 
  		     GetNbinsZ(),   zmin, zmax-zmin+1,
  		     amax-amin+1, amin, amax);

  h->SetBit(TH2::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!

  float val, err;
  for (int i=0; i<GetIZRHGH(); i++)
    for (int j=0; j<GetIMRHGH(); j++) {
      val = GetRNDATA()[j][i];
      err = GetRNERR()[j][i];
      if (val>0) {
	h->SetBinContent(i+1, GetA(i,j), val);
	h->SetBinError(i+1, GetA(i,j), err);
      }
    }
  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 
 return h;
}

TH1F *ROOT_UsrSuw::HistogramA() const
{
  /*
    Isotope Yield as a function of Mass Number
   */

  Int_t amin = GetAmin();
  Int_t amax = GetAmax();


  TH1F *h = new TH1F(Form("%s_a", GetBinName().data()), ROOT_UsrSuw::GetBinTitleA().Data(), 
  		     amax-amin+1, amin, amax);

  h->SetBit(TH1::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!

  float val, err;
  for (int i=GetAmin(); i<GetAmax(); i++) {
    val = GetYieldA(i);
    err = GetYieldAErr(i);
    if (val>0) {
      h->SetBinContent(i+1, val);
      h->SetBinError(i+1,   err*val); // the error is relative, but we need absolute
    }
  }
  
  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 
 return h;
}
