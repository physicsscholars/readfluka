/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
*/

#ifndef ROOT_UsrSuw_h
#define ROOT_UsrSuw_h

#include <TObject.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TObjArray.h>
#include "UsrSuw.h"


namespace ReadFluka {

  class ROOT_UsrSuw : public UsrSuw {
  private:
    TObjArray *fHistograms;
    TObjArray *fHistogramsA;
    TObjArray *fHistogramsZ;
  public:
    ROOT_UsrSuw(const char *fname);
    virtual ~ROOT_UsrSuw();

    TString GetBinTitle(int i) const;
    TString GetBinTitleA(int i) const;
    TString GetBinTitleZ(int i) const;
    TH2F *Histogram(int i) const;
    TH1F *HistogramA(int i) const;
    TH1F *HistogramZ(int i) const;

    TObjArray *Histograms() const;
    TObjArray *HistogramsA() const;
    TObjArray *HistogramsZ() const;
  };
}
#endif
