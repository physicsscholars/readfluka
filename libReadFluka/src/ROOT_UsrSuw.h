/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
*/

#ifndef ROOT_UsrSuw_h
#define ROOT_UsrSuw_h

#include <TObject.h>
#include <TH2F.h>
#include <TH1F.h>
#include "UsrSuw.h"


namespace ReadFluka {

  class ROOT_UsrSuw : public UsrSuw {
  public:
    ROOT_UsrSuw(const char *fname);
    virtual ~ROOT_UsrSuw();

    TString GetBinTitle() const;
    TString GetBinTitleA() const;
    TString GetBinTitleZ() const;
    TH2F *Histogram() const;
    TH1F *HistogramA() const;
    TH1F *HistogramZ() const;
  };
}
#endif
