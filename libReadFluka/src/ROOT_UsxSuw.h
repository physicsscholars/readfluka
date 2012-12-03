/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
*/

#ifndef ROOT_UsxSuw_h
#define ROOT_UsxSuw_h

#include <TObject.h>
#include <TH2F.h>
#include "UsxSuw.h"


namespace ReadFluka {

  class ROOT_UsxSuw : public UsxSuw {
  public:
    ROOT_UsxSuw(const char *fname);

    TString GetBinTitle(UInt_t i) const;
    TH2F *Histogram(UInt_t i) const;
  };
}
#endif
