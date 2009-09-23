#include "ROOT_UsrBin.h"
#include <iostream>

using namespace ReadFluka;

// since I'm not going to save this object in a .root file I do not need the class implementation
//ClassImp(ROOT_UsrBin);

ROOT_UsrBin::ROOT_UsrBin(const char *fname) : UsrBin(fname)
{

}

TH2F *ROOT_UsrBin::Histogram2(unsigned int z) const
{
  TH2F *h = new TH2F(GetBinName(), Form("%s(%d) {%.2f<z_{%d}<%.2f cm};x [cm];y [cm]", GetBinName(), GetDistType(), GetZmin(1, 1, z), z, GetZmax(1, 1, z)), GetNbinsX(), GetXmin(), GetXmax(), GetNbinsY(), GetYmin(), GetYmax());
  
  for (int x=1; x<=GetNbinsX(); x++)
    for (int y=1; y<=GetNbinsY(); y++) {
      h->SetBinContent(x, y, GetScored(x, y, z));
      //      cout << h->GetBinContent(x,y) << endl;
    }
  
  return h;
}

TH3F *ROOT_UsrBin::Histogram() const
{
  //  std::cout << "\t\"" << GetBinName() << "\"" << std::flush;
  TH3F *h = new TH3F(GetBinName(), Form("%s (%d);x [cm];y [cm];z [cm]", GetBinName(), GetDistType()), GetNbinsX(), GetXmin(), GetXmax(), GetNbinsY(), GetYmin(), GetYmax(), GetNbinsZ(), GetZmin(), GetZmax());
  
  for (int x=1; x<=GetNbinsX(); x++)
    for (int y=1; y<=GetNbinsY(); y++)
      for (int z=1; z<=GetNbinsZ(); z++)
	h->SetBinContent(x, y, z, GetScored(x, y, z));
  
  return h;
}
