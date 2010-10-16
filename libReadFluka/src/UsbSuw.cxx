#include "UsbSuw.h"
#include <iostream>
#include <sstream>

using namespace ReadFluka;

UsbSuw::UsbSuw(const char *fname) : UsrBin(fname)
{
  
}

UsbSuw::~UsbSuw()
{
  Reset();
}

void UsbSuw::Reset()
{
  fITUSBN.clear();
  fTITUSB.clear();
  fIDUSBN.clear();
  fXLOW.clear();
  fXHIGH.clear();
  fYLOW.clear();
  fYHIGH.clear();
  fZLOW.clear();
  fZHIGH.clear();

  fNXBIN.clear();
  fNYBIN.clear();
  fNZBIN.clear();
  
  fDXUSBN.clear();
  fDYUSBN.clear();
  fDZUSBN.clear();

  fScored.clear();
}

bool UsbSuw::Read()
{
  std::cout << "UsbSuw::Read()" << std::endl;
  if (fin->eof()) return false;
  Reset();

  char *mychar = new char[11];
  
  //std::cout << ReadInt() << std::endl;
  //  fin->read(mychar, 10); mychar[10] = '\0';
  //std::cout << mychar << std::endl;

  for (;;) {
    UsrBin::Read();
    
    fITUSBN.push_back(UsrBin::GetITUSBN());
    fTITUSB.push_back(UsrBin::GetBinName());
    
    fXLOW.push_back(UsrBin::GetXmin());
    fXHIGH.push_back(UsrBin::GetXmax());
    fNXBIN.push_back(UsrBin::GetNbinsX());
    fDXUSBN.push_back(UsrBin::GetdX());
    
    fYLOW.push_back(UsrBin::GetYmin());
    fYHIGH.push_back(UsrBin::GetYmax());
    fNYBIN.push_back(UsrBin::GetNbinsY());
    fDYUSBN.push_back(UsrBin::GetdY());
    
    fZLOW.push_back(UsrBin::GetZmin());
    fZHIGH.push_back(UsrBin::GetZmax());
    fNZBIN.push_back(UsrBin::GetNbinsZ());
    fDZUSBN.push_back(UsrBin::GetdZ());
    
    fScored.push_back(UsrBin::GetScoredVec());

    ReadInt(2);

    std::cout << "read" << std::endl;
    if (ReadStatFlag(false) == true) break;
  }

  return true;
}

void UsbSuw::Print() const
{
  std::cout << "UsbSuw::Print" << std::endl;
  for (int i=0; i<1; i++) {
    std::cout << "bin type: " << fITUSBN[i] << std::endl;
    std::cout << "bin name: " << fTITUSB[i] << std::endl;
    std::cout << fXLOW[i] << " < x < " << fXHIGH[i] << "\t" << fNXBIN[i] << "\t" << fDXUSBN[i] << std::endl;
    std::cout << fYLOW[i] << " < y < " << fYHIGH[i] << "\t" << fNYBIN[i] << "\t" << fDYUSBN[i] << std::endl;
    std::cout << fZLOW[i] << " < z < " << fZHIGH[i] << "\t" << fNZBIN[i] << "\t" << fDZUSBN[i] << std::endl;

    /*
    std::cout << "scored: " << std::flush;
    for (int ibin=0; ibin<GetNbins(i); ibin++)
      std::cout << fScored[i][ibin] << " " << std::flush;
    std::cout << std::endl;
    */
  }
}
