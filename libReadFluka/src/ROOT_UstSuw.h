#ifndef ROOT_UstSuw_h
#define ROOT_UstSuw_h

#include "UstSuw.h"
#include <TH1F.h>
#include <TObjArray.h>

namespace ReadFluka {

  class ROOT_UstSuw : public UstSuw {
  public:
    ROOT_UstSuw(const char *fname);

    //TString GetBinTitle() const;
    TObjArray *Histograms(int record) const;
    TObjArray *Histograms() const;
    TH1F *HistTotalResponse(int record) const;
    TH1F *HistTotalTrackLength(int record) const;
  };
}


#endif
