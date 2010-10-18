/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
*/

#ifndef ROOT_UsbSuw_h
#define ROOT_UsbSuw_h

#include <TObject.h>
#include <TH3F.h>
#include <TObjArray.h>
#include "UsbSuw.h"


namespace ReadFluka {
  class ROOT_UsbSuw : public UsbSuw {
  private:
    TObjArray *fHistograms;
  public:
    ROOT_UsbSuw(const char *fname);
    virtual ~ROOT_UsbSuw();

    TString GetBinTitle(int i) const;
    TH3F *Histogram(int i) const;

    TObjArray *Histograms() const;
  };
}
#endif
