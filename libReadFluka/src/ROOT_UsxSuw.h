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

    TString GetBinTitle() const;
    TH2F *Histogram() const;
  };
}
#endif
