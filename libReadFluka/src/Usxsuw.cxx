#include <iostream>
//#include <iomanip>
//#include <cstdlib>
#include <cstring>
//#include <sstream>

#include "Usxsuw.h"

using namespace ReadFluka;

Usxsuw::Usxsuw(const char *fname) : Base(fname)
{
  fIsReadHeader = false;
  fWEIPRI = 0;
  fNCASE  = 0;
  fENGMAX = 0;
  Reset();

  ReadHeader();
  fNCTOT = fNCTOT+fNCASE;
  fMCTOT = fMCTOT+fNCASE;
  if ( fNCTOT > 1000000000 ) {
    fNCTOT = fNCTOT - 1000000000;
    fMCTOT = fMCTOT + 1;
  }
  fWCTOT += (double)fWEIPRI;
  fMBATCH += fNBATCH;
}

Usxsuw::~Usxsuw()
{
  Reset();
}

void Usxsuw::Reset()
{
  fMCASE = 0;
  fNBATCH = fMBATCH = 0;
  fRECORD = 0;
  fNCTOT = fMCTOT = 0;
  fWCTOT = 0.0;
  fNX = fMX = 0;

  fTITUSX = "";
  fIDUSBX = 0;
  fNR1USX = 0;
  fNR2USX = 0;
  fAUSBDX = 0.0f;
  //  fLWUSBX = false;
  //  fLFUSBX = false;
  fEBXLOW = fEBXHGH = 0.0f;
  fNEBXBN = 0;
  fDEBXBN = 0.0f;
  fABXLOW = fABXHGH = 0.0f;
  fNABXBN = 0;
  fDABXBN = 0.0f;

  fIRECRD = 0;
  fIGMUSX = 0;
  if (fENGMAX) {
    delete [] fENGMAX;
    delete fENGMAX;
    fENGMAX = 0;
  }
}

void Usxsuw::ReadHeader()
{
  if (fIsReadHeader == true) return;

  fIsReadHeader = true;
  ReadRunTitle();
  ReadRunTime();
  fWEIPRI = ReadInt();
  fNCASE  = ReadInt();
  fMCASE  = ReadInt();
  fNBATCH = ReadInt(); std::cout << "NBATCH: " << fNBATCH << std::endl;
  fRECORD++;
  //  ReadInt();
  SizeStart();
}

void Usxsuw::Read()
{
  SizeStart();
  fMX = ReadInt(); std::cout << "MX: " << fMX << std::endl;

  char *mychar = new char[10];
  fin->read(mychar, 10); mychar[10]='\0';
  fTITUSX = Trimmed(std::string(mychar));  std::cout << "Bin name: \"" << fTITUSX << "\"" <<  std::flush;
  ReadInt(); // !!! -1 Why? !!!
  fIDUSBX = ReadInt();   std::cout << "\tIDUSBX: " << fIDUSBX << std::flush;
  fNR1USX = ReadInt();   std::cout << "\tfrom reg. " << fNR1USX << std::flush;
  fNR2USX = ReadInt();   std::cout << "\tto reg. " << fNR2USX << std::flush;
  fAUSBDX = ReadFloat();   std::cout << "\tarea: " << fAUSBDX << std::endl;
  fLWUSBX = ReadBool();    if (fLWUSBX == true) std::cout << "2 ways"; else std::cout << "1 way"; std::cout << std::endl;
  fLFUSBX = ReadBool();    if (fLFUSBX == true) std::cout << "fluence"; else std::cout << "current"; std::cout << std::endl;
  fLLNUSX = ReadBool();    if (fLLNUSX == true) std::cout << "with low energy neutrons"; else std::cout << "without low energy neutrons"; std::cout << std::endl;
  fEBXLOW = ReadFloat(); std::cout << "min energy: " << fEBXLOW << std::endl;
  fEBXHGH = ReadFloat(); std::cout << "min energy: " << fEBXHGH << std::endl;

  fNEBXBN = ReadInt();
  fDEBXBN = ReadFloat();

  fABXLOW = ReadFloat(); fABXHGH = ReadFloat(); fNABXBN = ReadInt(); fDABXBN = ReadFloat();

  SizeEnd(); SizeStart();

  fIRECRD++;

  if (fLLNUSX) { // read low energy neutrons
    std::cout << "read neutrons" << std::endl;
    fIGMUSX = ReadInt();
    fENGMAX = new float[fIGMUSX+1];
    for (int i=0; i<fIGMUSX+1; i++) fENGMAX[i] = ReadFloat();
    fIRECRD++;
  } else fIGMUSX = 0;
  // + see page 239 for further reading
}


void Usxsuw::Print() const
{
  
}
