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
  fRNDATA.clear();
}

bool ResNuclei::ReadHeader()
{
  if (fIsReadHeader == true) return false;
  fIsReadHeader = true;
  ReadRunTitle(); // here also SizeStart called 
  ReadRunTime();
  //ReadInt();
  //std::cerr << SizeStart() << std::endl;
  fWEIPRI = ReadFloat();
  fNCASE  = ReadInt();
  std::cout << "->strange numbers: " << ReadInt() << " " << ReadInt() << std::endl;
  CheckFormat();

  return true;
}

bool ResNuclei::Read()
{
  //ReadInt(4);
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

  CheckFormat();

  std::vector <float> val; // scored values
  float tmp;
  std::cout << "IZRHGH and IMRHGH: " << fIZRHGH << " " << fIMRHGH << std::endl;
  for (int i=0; i<fIZRHGH; i++) {
    val.clear();
    for (int j=0; j<fIMRHGH; j++) {
      tmp = ReadFloat(); 
      if (tmp>0) // add +1 since in C++ we count from 0
	std::cout << i << " " << j << " " << tmp << std::endl;
      //	std::cout << i+1 << " " << j+fK+2*i+1 << " " << tmp << std::endl;
      //      val.push_back(tmp);
    }
    //    fRNDATA.push_back(val);
  }
  CheckFormat();


  delete [] mychar; mychar = 0;

  return true;
}
