#include "ROOT_UsrSuw.h"
#include <iostream>

using namespace ReadFluka;

ROOT_UsrSuw::ROOT_UsrSuw(const char *fname) : UsrSuw(fname)
{

}

ROOT_UsrSuw::~ROOT_UsrSuw()
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
  TString str = UsrSuw::GetBinTitleA();
  str = str + ";" + GetYtitle() + ";Yield [nuclei/cm^{2}/primary]";
  return str;
}

TString ROOT_UsrSuw::GetBinTitleZ() const
{
  TString str = UsrSuw::GetBinTitleZ();
  str = str + ";" + GetXtitle() + ";Yield [nuclei/cm^{2}/primary]";
  return str;
}

TH2F *ROOT_UsrSuw::Histogram() const
{
  TH2F *h = new TH2F(GetBinName().data(), ROOT_UsrSuw::GetBinTitle().Data(), 
  		     GetNbinsZ()+1,   GetZmin(), GetZmax()+1,
  		     GetNbinsA()+1,   GetAmin(), GetAmax()+1);

  h->SetBit(TH2::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!

  float val, err;
  for (int i=0; i<GetIZRHGH(); i++)
    for (int j=0; j<GetIMRHGH(); j++) {
      val = GetRNDATA()[0][j][i];
      err = GetRNERR()[0][j][i];
      if (val>0) {
	h->SetBinContent(i+1, GetA(i,j), val);
	h->SetBinError(i+1, GetA(i,j), err);
      }
    }
  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 
 return h;
}

TH1F *ROOT_UsrSuw::HistogramZ() const
{
  /*
    Isotope Yield as a function of Atomic Number
   */

  TH1F *h = new TH1F(Form("%s_Z", GetBinName().data()), ROOT_UsrSuw::GetBinTitleZ().Data(), 
  		     GetNbinsZ()+1, GetZmin(), GetZmax()+1);

  h->SetBit(TH1::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!

  float val, err;
  for (int i=GetZmin()-1; i<GetZmax(); i++) {
    val = GetYieldZ(i,0);
    err = GetYieldZErr(i,0);
    if (val>0) {
      h->SetBinContent(i+1, val);
      h->SetBinError(i+1,   err*val); // the error is relative, but we need absolute
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

  TH1F *h = new TH1F(Form("%s_A", GetBinName().data()), ROOT_UsrSuw::GetBinTitleA().Data(), 
  		     GetNbinsA()+1, GetAmin(), GetAmax()+1);

  h->SetBit(TH1::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!

  float val, err;
  for (int i=GetAmin()-1; i<GetAmax(); i++) {
    val = GetYieldA(i,0);
    err = GetYieldAErr(i,0);
    if (val>0) {
      h->SetBinContent(i+1, val);
      h->SetBinError(i+1,   err*val); // the error is relative, but we need absolute
    }
  }
  
  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 
 return h;
}
