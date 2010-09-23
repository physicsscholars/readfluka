#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib> // for exit()

#include "ResNuclei.h"

using namespace ReadFluka;

ResNuclei::ResNuclei(const char *fname) : Base(fname)
{
  //  fIsReadHeader = false;
  //fWEIPRI = 0;
  //fNCASE  = 0;
  //fENGMAX = 0;
  //fScored = 0;
  Reset();

  ReadHeader();
}

ResNuclei::~ResNuclei()
{
  Reset();
}

void ResNuclei::Reset()
{
  /*
    Reset the private members
   */
  fIsReadHeader = false;
  fNRN = 0;
  fTIURSN = "";
  fITURSN = 0;
  fNRURSN = 0;
  fVURSNC = 0.0f;
  fIMRHGH = fIZRHGH = 0;
  fK = 0;
}

void ResNuclei::ReadHeader()
{
  if (fIsReadHeader == true) return;

  fIsReadHeader = true;
  ReadRunTitle();
  ReadRunTime();
  fWEIPRI = ReadFloat();
  fNCASE  = ReadInt();
  //CheckFormat();
}

bool ResNuclei::Read()
{
  ReadInt(4);
  fNRN = ReadInt();

  char *mychar = new char[10];
  fin->read(mychar, 10); mychar[10] = '\0';
  fTIURSN = Trimmed(std::string(mychar));

  fITURSN = ReadInt();
  fNRURSN = ReadInt();
  fVURSNC = ReadFloat();
  fIMRHGH = ReadInt();
  fIZRHGH = ReadInt();
  fK = ReadInt();

 


  delete [] mychar; mychar = 0;

  return true;
}
