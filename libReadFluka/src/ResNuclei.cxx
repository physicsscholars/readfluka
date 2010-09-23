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
  fAmax = -1;
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
  for (int i=0; i<fIMRHGH; i++) {
    val.clear();
    for (int j=0; j<fIZRHGH; j++) {
      tmp = ReadFloat();
      if ((tmp>0) && (GetA(j,i)>fAmax)) fAmax = GetA(j,i);
      //      std::cout << tmp << "\t\t" << std::flush;
      val.push_back(tmp);
    }
    fRNDATA.push_back(val);
  }

  CheckFormat();


  delete [] mychar; mychar = 0;

  return true;
}

float ResNuclei::GetRNDATA(unsigned int Z, unsigned int A) const
{
  /*
    Return residual nuclei production with specified Z and A
   */

  if (Z>GetZmax()) {
    std::cerr << "WARNING by GetRNDATA: Z = " << Z << " > Zmax = " << GetZmax() << std::endl;
    return 0.0f;
  }
  if (A>GetAmax()) {
    std::cerr << "WARNING by GetRNDATA: A = " << A << " > Amax" << GetAmax() << std::endl;
  }
  Z = Z-1;
  A = A-1-fK-2*(Z+1);
  //std::cout << "arguments for GetRNDATA: " << Z << " " << A << std::endl;
  return fRNDATA[A][Z];
}

void ResNuclei::Print() const
{
  /*
    Print the info about the current RESNUCLEi card
   */
  
  std::cout << "\tZ\tA\tresidual nuclei" << std::endl;
  std::cout << "\t\t\tper cm**3 per primary" << std::endl;

  float tmp;
  int iCount=0;
  for (int i=0; i<GetIZRHGH(); i++)
    for (int j=0; j<GetIMRHGH(); j++) {
      tmp = GetRNDATA()[j][i];
      //      cout << "\t" << i+1 << " " << j+1 << " " << GetK() << endl;
      if (tmp>0)
	std::cout << "\t" << i+1 << "\t" << GetA(i,j) << "\t" << tmp << std::endl;
      iCount++;
    }
  std::cout << "total: " << iCount << std::endl;
  
}

std::string ResNuclei::GetBinTitle() const
{
  std::ostringstream tmp;

  if (abs(GetITURSN())<1) { // see page 211
    tmp << "Res. nuclei n. " << GetNRN() << " \"" << GetTIURSN() 
	 << "\", 'high' energy products, region n. " << GetNRURSN() << std::flush;
    tmp << "\tdetector volume: " << GetVURSNC() << " cm**3" << std::flush;
    tmp << "\tMax. Z: " << GetIZRHGH();
    tmp << ", Max. N-Z: " << GetIMRHGH() + GetK();
    tmp << ", Min. N-Z: " << GetK()+1 << std::flush;
  } else if (abs(GetITURSN())<2) {
    tmp << "Res. nuclei n. " << GetNRN() << " \"" << GetTIURSN() 
	 << "\", 'low' energy products, region n. " << GetNRURSN() << std::flush;
    tmp << "\tdetector volume: " << GetVURSNC() << " cm**3" << std::flush;
    tmp << "\tMax. Z: " << GetIZRHGH();
    tmp << ", Max. N-Z: " << GetIMRHGH() + GetK();
    tmp << ", Min. N-Z: " << GetK()+1 << std::flush;
  } else {
    tmp << "Res. nuclei n. " << GetNRN() << " \"" << GetTIURSN() 
	 << "\", all products, region n. " << GetNRURSN() << std::flush;
    tmp << "\tdetector volume: " << GetVURSNC() << " cm**3" << std::flush;
    tmp << "\tMax. Z: " << GetIZRHGH();
    tmp << ", Max. N-Z: " << GetIMRHGH() + GetK();
    tmp << ", Min. N-Z: " << GetK()+1 << std::flush;
  }  

  return tmp.str();
}
