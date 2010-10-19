#include "ROOT_UsbSuw.h"
#include <iostream>

using namespace ReadFluka;

ROOT_UsbSuw::ROOT_UsbSuw(const char *fname) : UsbSuw(fname)
{
  fHistograms = new TObjArray;
}

ROOT_UsbSuw::~ROOT_UsbSuw()
{
  fHistograms->Clear("C");  SafeDelete(fHistograms);
}


TString ROOT_UsbSuw::GetBinTitle(int i) const
{
  return "ROOT_UsbSuw::GetBinTitle()";
  //  TString str = UsbSuw:GetBinTitle(i);
  // str = str + ";" + GetXtitle() + ";" + GetYtitle() + ";" + GetZtitle();
  // return str;
}

TH3F *ROOT_UsbSuw::Histogram(int iHist) const
{
  TH3F *h = new TH3F(GetTITUSB(iHist).c_str(), Form("%s (%d);x [cm];y [cm];z [cm]", GetTITUSB(iHist).c_str(), GetIDUSBN(iHist)),
		     GetNXBIN(iHist), GetXLOW(iHist), GetXHIGH(iHist), GetNYBIN(iHist), GetYLOW(iHist), GetYHIGH(iHist), GetNZBIN(iHist), GetZLOW(iHist), GetZHIGH(iHist));
  h->SetBit(TH3::kIsAverage);
	
  float val = 0;

  for (int x=1; x<=GetNXBIN(iHist); x++)
    for (int y=1; y<=GetNYBIN(iHist); y++)
      for (int z=1; z<=GetNZBIN(iHist); z++) {
	val = GetScored(iHist, x, y, z);
        h->SetBinContent(x, y, z, val);
	h->SetBinError(x, y, z, GetError(iHist, x, y, z)*val);
      }

  h->SetEntries(GetWEIPRI()); // must go after the filling, otherwise wrong entry number
 
  return h;
}

TObjArray *ROOT_UsbSuw::Histograms() const 
{
  fHistograms->Clear();
  TH3F *h;
  std::cout << "ROOT_UsbSuw: Histogram: " << GetN() << " histograms found" << std::endl;
  for (int i=0; i<GetN(); i++) {
    h = Histogram(i);
    //h->Print();
    fHistograms->Add(h);
  }
  return fHistograms;
}

