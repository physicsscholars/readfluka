/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
*/

#ifndef ROOT_UsrBdx_h
#define ROOT_UsrBdx_h

#include <TObject.h>
#include <TH2F.h>
#include "UsrBdx.h"


namespace ReadFluka {

  class ROOT_UsrBdx : public UsrBdx {
  public:
    ROOT_UsrBdx(const char *fname);

    TString GetBinTitle() const;
    TH2F *Histogram() const; // note that this is a 3D histogram
  };
}
#endif
