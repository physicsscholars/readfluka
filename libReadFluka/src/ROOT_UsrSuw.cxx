#include "ROOT_UsrSuw.h"
#include <iostream>

using namespace ReadFluka;

ROOT_UsrSuw::ROOT_UsrSuw(const char *fname) : UsrSuw(fname)
{
  fHistograms = new TObjArray;
  fHistogramsA = new TObjArray;
  fHistogramsZ = new TObjArray;
}

ROOT_UsrSuw::~ROOT_UsrSuw()
{
  fHistograms->Clear("C");  SafeDelete(fHistograms);
  fHistogramsA->Clear("C");  SafeDelete(fHistogramsA);
  fHistogramsZ->Clear("C");  SafeDelete(fHistogramsZ);
}

TString ROOT_UsrSuw::GetBinTitle(int i) const
{
  TString str = UsrSuw::GetBinTitle(i);
  str = str + ";" + GetXtitle() + ";" + GetYtitle() + ";" + GetZtitle();
  return str;
}

TString ROOT_UsrSuw::GetBinTitleA(int i) const
{
  TString str = UsrSuw::GetBinTitleA(i);
  str = str + ";" + GetYtitle() + ";Yield [nuclei/cm^{2}/primary]";
  return str;
}

TString ROOT_UsrSuw::GetBinTitleZ(int i) const
{
  TString str = UsrSuw::GetBinTitleZ(i);
  str = str + ";" + GetXtitle() + ";Yield [nuclei/cm^{2}/primary]";
  return str;
}

TH2F *ROOT_UsrSuw::Histogram(int iHist) const
{
  TH2F *h = new TH2F(GetBinName(iHist).data(), ROOT_UsrSuw::GetBinTitle(iHist).Data(), 
  		     GetNbinsZ(iHist)+1,   GetZmin(iHist), GetZmax(iHist)+1,
  		     GetNbinsA(iHist)+1,   GetAmin(iHist), GetAmax(iHist)+1);

  h->SetBit(TH2::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!
  h->SetDirectory(0);

  float val, err;
  for (int i=0; i<GetIZRHGH(i); i++)
    for (int j=0; j<GetIMRHGH(i); j++) {
      std::cout << iHist << std::endl;
      val = GetRNDATA()[iHist][j][i];
      err = GetRNERR()[iHist][j][i];
      // std::cerr << val << " " << err << std::endl;
      if (val>0) {
	h->SetBinContent(i+1, GetA(iHist, i,j), val);
	h->SetBinError(i+1, GetA(iHist,  i,j), err);
      }
    }

  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 
 return h;
}

TH1F *ROOT_UsrSuw::HistogramZ(int iHist) const
{
  /*
    Isotope Yield as a function of Atomic Number
   */

  TH1F *h = new TH1F(Form("%s_Z", GetBinName(iHist).data()), ROOT_UsrSuw::GetBinTitleZ(iHist).Data(), 
  		     GetNbinsZ(iHist)+1, GetZmin(iHist), GetZmax(iHist)+1);

  h->SetBit(TH1::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!

  float val, err;
  for (int i=GetZmin(iHist)-1; i<GetZmax(iHist); i++) {
    val = GetYieldZ(iHist,i);
    err = GetYieldZErr(iHist,i);
    if (val>0) {
      h->SetBinContent(i+1, val);
      h->SetBinError(i+1,   err*val); // the error is relative, but we need absolute
    }
  }
  
  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 
 return h;
}

TH1F *ROOT_UsrSuw::HistogramA(int iHist) const
{
  /*
    Isotope Yield as a function of Mass Number
   */

  TH1F *h = new TH1F(Form("%s_A", GetBinName(iHist).data()), ROOT_UsrSuw::GetBinTitleA(iHist).Data(), 
  		     GetNbinsA(iHist)+1, GetAmin(iHist), GetAmax(iHist)+1);

  h->SetBit(TH1::kIsAverage); // see NOTE1 in the source code of hadd.cxx - not yet implemented for hadd (29 Sep 2010)
  h->Sumw2(); // it must go before filling the histo!
  h->SetDirectory(0);

  float val, err;
  for (int i=GetAmin(iHist)-1; i<GetAmax(iHist); i++) {
    val = GetYieldA(iHist, i);
    err = GetYieldAErr(iHist, i);
    if (val>0) {
      h->SetBinContent(i+1, val);
      h->SetBinError(i+1,   err*val); // the error is relative, but we need absolute
    }
  }
  
  h->SetEntries(GetEntryNumber()); // must go after the filling, otherwise wrong entry number
 
 return h;
}

TObjArray *ROOT_UsrSuw::Histograms() const 
{
  fHistograms->Clear();
  TH2F *h;
  for (int i=0; i<GetN(); i++) { std::cout << i << std::endl;
    h = Histogram(i);
    //h->Print();
    fHistograms->Add(h);
  }
  return fHistograms;
}

TObjArray *ROOT_UsrSuw::HistogramsA() const 
{
  fHistogramsA->Clear();
  TH1F *h;
  for (int i=0; i<GetN(); i++) {
    h = HistogramA(i);
    fHistogramsA->Add(h);
  }
  return fHistogramsA;
}

TObjArray *ROOT_UsrSuw::HistogramsZ() const 
{
  fHistogramsZ->Clear();
  TH1F *h;
  for (int i=0; i<GetN(); i++) {
    h = HistogramZ(i);
    fHistogramsZ->Add(h);
  }
  return fHistogramsZ;
}
