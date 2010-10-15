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
  fDXUSBN.clear();
  fDYUSBN.clear();
  fDZUSBN.clear();
}

bool UsbSuw::Read()
{
  std::cout << "UsbSuw::Read()" << std::endl;
  if (fin->eof()) return false;
  Reset();

  char *mychar = new char[11];
  

  return true;
}

void UsbSuw::Print() const
{
  std::cout << "UsbSuw::Print" << std::endl;
}
