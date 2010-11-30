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
  std::cout << h->GetBinContent(1) << " " << h->GetBinError(1) << std::endl;
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
  std::cout << h->GetBinContent(1) << " " << h->GetBinError(1) << std::endl;
  return h;
}

TObjArray* ROOT_UstSuw::Histograms(int record) const
{
  TObjArray *hists = new TObjArray;
  hists->Add(HistTotalResponse(record));
  hists->Add(HistTotalTrackLength(record));

  return hists;
}

TObjArray *ROOT_UstSuw::Histograms() const
{
  TObjArray *h = new TObjArray;
  for (unsigned int i=0; i<GetNRecords(); i++)
    h->Add(Histograms(i));

  return h;
}
