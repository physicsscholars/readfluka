#include "Mgdraw.h"
#include <iostream>
#include <iomanip>

using namespace std;
 
int main(int argc, char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::Mgdraw *mgdraw = new ReadFluka::Mgdraw(argv[1]);
  
  mgdraw->ReadEvent(1);
  
  delete mgdraw;
  return 0;
}

