/*
  Author: Konstantin Batkov    kbat at mail.ru
  http://readfluka.googlecode.com
*/

#ifndef ROOT_UsrBin_h
#define ROOT_UsrBin_h

#include <TObject.h>
#include <TH2F.h>
#include <TH3F.h>
#include "UsrBin.h"


namespace ReadFluka {

  class ROOT_UsrBin : public UsrBin {
  public:
    ROOT_UsrBin(const char *fname);
    TH2F *Histogram2(unsigned int z) const;
    TH3F *Histogram() const; // note that this is a 3D histogram
    
    // since I'm not going to save this object in a .root file I do not need the class definition
    //ClassDef(ROOT_UsrBin, 0); // ROOT wrapper for UsrBin
  };
}
#endif
