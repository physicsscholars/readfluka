#include "ROOT_UstSuw.h"
#include <iostream>

using namespace ReadFluka;

ROOT_UstSuw::ROOT_UstSuw(const char *fname) : UstSuw(fname)
{
}

TH1F* ROOT_UstSuw::HistTotalResponse(int record) const
{
  TH1F *h = new TH1F(Form("%s_tot", UstSuw::GetName(record).c_str()), 
		     Form("Total %s response in region %d;Energy [GeV];Total Response [p/cm^{2}/pr]", Code2Name(GetID(record)), GetRegion(record)),
		     1, GetEmin(record), GetEmax(record));
  h->SetBinContent(1, GetTotResp(record));
  h->SetBinError(1, GetTotRespErr(record)*GetTotResp(record));  // absolute error
  h->SetEntries(GetEntryNumber());
  h->Print("base");
  //std::cout << h->GetBinContent(1) << " " << h->GetBinError(1) << std::endl;
  return h;
}

TH1F* ROOT_UstSuw::HistTotalTrackLength(int record) const
{
  TH1F *h = new TH1F(Form("%s_totl", UstSuw::GetName(record).c_str()), 
		     Form("Total %s Track Length in region %d;Energy [GeV];Total Track Length [cm/pr]", Code2Name(GetID(record)), GetRegion(record)),
		     1, GetEmin(record), GetEmax(record));
  h->SetBinContent(1, GetTotResp(record)*GetVolume(record));
  h->SetBinError(1, GetTotRespErr(record)*GetTotResp(record)*GetVolume(record));  // absolute error
  h->SetEntries(GetEntryNumber());
  h->Print("base");
  //std::cout << h->GetBinContent(1) << " " << h->GetBinError(1) << std::endl;
  return h;
}

TObjArray* ROOT_UstSuw::Histograms(int record) const
{
  TObjArray *hists = new TObjArray;
  hists->Add(HistTotalResponse(record));
  hists->Add(HistTotalTrackLength(record));
  hists->Add(HistFlux(record));

  return hists;
}

TObjArray *ROOT_UstSuw::Histograms() const
{
  TObjArray *h = new TObjArray;
  for (unsigned int i=0; i<GetNRecords(); i++)
    h->Add(Histograms(i));

  return h;
}

TH1F *ROOT_UstSuw::HistFlux(int record) const
{
  /*
    Histogram different. flux as a function of energy
   */
  std::vector<float> vxbins = GetEnergyBoundaries(record);
  std::vector<float> vflux = GetFluxAll(record);
  std::vector<float> verror = GetFluxErrAll(record);
  const int nbinsx = vxbins.size()-1; // -1 since the number of bins ONE less than the number of boundaries
  float *xbins = new float[nbinsx+1];
  
  for (int i=0; i<nbinsx+1; i++) {
    xbins[i] = vxbins[i];
    //    std::cout << xbins[i] << std::endl;
  }

  TH1F *h = new TH1F(Form("%s", UstSuw::GetName(record).c_str()),
		     Form("Differential Flux (or Fluence?) of %s in region %d;Energy [GeV];Flux [Part/GeV/cm^{2}/pr]", 
			  Code2Name(GetID(record)), GetRegion(record)),
		     nbinsx, xbins);
  float value,error;
  for (int bin=0; bin<nbinsx; bin++) {
    value = vflux[bin];
    error = value*verror[bin];
    h->SetBinContent(bin+1, value);
    h->SetBinError(bin+1, error);
  }
  h->SetEntries(GetEntryNumber());

  verror.clear();
  vflux.clear();
  vxbins.clear();
  delete [] xbins; xbins = 0;

  return h;
}
