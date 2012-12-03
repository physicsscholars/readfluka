#include "ROOT_UsxSuw.h"
#include <iostream>

using namespace ReadFluka;

ROOT_UsxSuw::ROOT_UsxSuw(const char *fname) : UsxSuw(fname)
{

}

TString ROOT_UsxSuw::GetBinTitle() const
{
  TString str = "";//UsxSuw::GetBinTitle();
  /*  str.ReplaceAll("<->", "#leftrightarrow");
  str.ReplaceAll("->", "#rightarrow");
  str = str + ";" + GetXtitle() + ";" + GetYtitle();*/
  return str;
}

TH2F *ROOT_UsxSuw::Histogram() const
{
  /*  Float_t emin = GetEmin();
  Float_t emax = GetEmax();
  if (IsLogE()) {
    emin = log10(emin);
    emax = log10(emax);
  }

  Float_t amin = GetAmin();
  Float_t amax = GetAmax();
  if (IsLogA()) {
    amin = log10(amin);
    amax = log10(amax);
  }

  TH2F *h = new TH2F(GetBinName().data(), ROOT_UsxSuw::GetBinTitle().Data(), 
  		     GetNbinsE(), emin, emax,
  		     GetNbinsA(), amin, amax);
  
  for (unsigned int i=1; i<=GetNbinsE(); i++)
    for (unsigned int j=1; j<=GetNbinsA(); j++)
      h->SetBinContent(i, j, GetScored(i, j));
  
  h->SetEntries(GetEntryNumber());
  
  return h;*/
  return (TH2F*)0;
}
