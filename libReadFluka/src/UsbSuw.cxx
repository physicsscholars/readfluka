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
}

bool UsbSuw::Read()
{
  return true;
}

void UsbSuw::Print() const
{
  std::cout << "UsbSuw::Print" << std::endl;
}
