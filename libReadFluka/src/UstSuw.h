#ifndef UstSuw_h
#define UstSuw_h

/*
  UstSuw - a class to read the $FLUTIL/ustsuw binary output (USRTRACK)
 */

#include <vector>
#include <string>
#include "Base.h"

namespace ReadFluka {
  class UstSuw : public Base {
    int fNCTOT; // total number of incident particles
    int fMCTOT; //
    int fMBATCH;
    float fWCTOT; // total weight
    
  };
}

#endif
