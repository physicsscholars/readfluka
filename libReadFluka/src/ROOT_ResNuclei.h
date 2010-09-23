/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
*/

#ifndef ROOT_ResNuclei_h
#define ROOT_ResNuclei_h

#include <TObject.h>
#include <TH2F.h>
#include "ResNuclei.h"


namespace ReadFluka {

  class ROOT_ResNuclei : public ResNuclei {
  public:
    ROOT_ResNuclei(const char *fname);

    TString GetBinTitle() const;
    TH2F *Histogram() const;
  };
}
#endif
