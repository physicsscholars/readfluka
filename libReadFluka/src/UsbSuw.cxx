#include "UsbSuw.h"
#include <iostream>
#include <sstream>
#include <cmath> // for max

using namespace ReadFluka;

UsbSuw::UsbSuw(const char *fname) : Base(fname)
{
  Reset();

  ReadRunTitle();
  ReadRunTime();

  fWEIPRI = ReadFloat();
  fNCASE = ReadInt();
  fMCASE = ReadInt();
  fNBATCH = ReadInt();

  CheckFormat();

  fNCTOT += fNCASE;
  fMCTOT += fMCASE;
  
  if ( fNCTOT > 1000000000 ) {
    fNCTOT -= 1000000000;
    fMCTOT += fMCTOT;
  }
  
  fWCTOT += fWEIPRI;
  fKLAST = 0;
  fKMAX = 0; // line 177
}

UsbSuw::~UsbSuw()
{
  Reset();
}

void UsbSuw::Reset()
{
  fReadCounter = 0;
  fMCASE = 0;
  fNBATCH = 0;
  fNCTOT = 0;
  fMCTOT = 0;
  fWCTOT = 0.0;
  fKLAST = 0;
  fKMAX = 0;
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

  fLNTZER.clear();
  fBKUSBN.clear();
  fB2USBN.clear();
  fTCUSBN.clear();

  fIRECRD = 0;

  fJB.clear();

  fLTRKBN.clear();
  fNUSRBN = 0;
  fLSTATI = false;
  fKBUSBN.clear();

  fScored.clear();
  fGBSTOR.clear();

  fN = 0;
}

bool UsbSuw::Read()
{
  std::cout << "UsbSuw::Read()" << std::endl;
  if (fin->eof()) return false;

  int K0 = 0;
  int K1 = 0;
  int NB, MB;
  int IB=0;
  std::vector<float> tmpvec; // what if we need to read double instead fo float? !!!

  //  CheckFormat();
  for (IB=0; IB<1; IB++) {
    //    CheckFormat();
    std::cout << "start loop IB" << std::endl;
    NB = IB;
    MB = ReadInt(); std::cout << MB << std::endl;
    
    char *strtmp = new char[11];
    fin->read(strtmp, 10); strtmp[10] = '\0';
    fTITUSB.push_back(Trimmed(std::string(strtmp)));
    delete strtmp;
    
    fITUSBN.push_back(ReadInt());
    fIDUSBN.push_back(ReadInt());
    
    fXLOW.push_back(ReadFloat());
    fXHIGH.push_back(ReadFloat());
    fNXBIN.push_back(ReadInt());
    fDXUSBN.push_back(ReadFloat());
    
    fYLOW.push_back(ReadFloat());
    fYHIGH.push_back(ReadFloat());
    fNYBIN.push_back(ReadInt());
    fDYUSBN.push_back(ReadFloat());
    
    fZLOW.push_back(ReadFloat());
    fZHIGH.push_back(ReadFloat());
    fNZBIN.push_back(ReadInt());
    fDZUSBN.push_back(ReadFloat());
    
    fLNTZER.push_back(ReadBool());
    fBKUSBN.push_back(ReadInt());
    fB2USBN.push_back(ReadInt());
    fTCUSBN.push_back(ReadFloat()); // line 186
    
    CheckFormat();

    fIRECRD ++;
    
    // line 223
    fJB.push_back(NB);

    if ((fIDUSBN[NB]!=208) && (fIDUSBN[NB]!=211) && (fITUSBN[NB]>=10) )
      fLTRKBN.push_back(true); // this is a track-length binning
    else
      fLTRKBN.push_back(false);
    
    K0 = fKLAST+1;
    fKBUSBN.push_back(K0);
    K1 = GetNbins(IB) + K0 - 1;
    fKLAST = K1;
    fKMAX = std::max(fKMAX, fKLAST);
    //    std::cout << "K0 and K1: " << K0 << " " << K1 << std::endl;
    
    for (int i=K0; i<=K1; i++)
      tmpvec.push_back(ReadFloat());
    
    fScored.push_back(tmpvec);
    tmpvec.clear();
    
    CheckFormat();
    
    fIRECRD++;
    fN++;
  }
  
  // line 242
  IB--;
  fNUSRBN = IB;
  
  if (fLSTATI) { // 'STATISTICS'
    std::cout << "statistics" << std::endl;
    fKLAST = 0;
    for (int jj = 0; jj < IB; jj++) {
      NB = fJB[jj];
      K0 = fKLAST+1;
      K1 = GetNbins(NB) + K0-1;
      fKLAST = K1;
      tmpvec.clear();
      for (int j=K0; j<K1; j++)
	tmpvec.push_back(ReadFloat());
      fGBSTOR.push_back(tmpvec);
      
      CheckFormat();
    }
    } else
    std::cout << "no statistics" << std::endl;
  
  //    if (ReadStatFlag(false) == true) break;


  //  fReadCounter++;
  return true;
}

float UsbSuw::GetScored(int i, int x, int y, int z) const
{
  /*
    Return the scored value for xyz cell in the i-th histogram.
    The sell indises must be >= 1
   */
  x = x-1;
  y = y-1;
  z = z-1;
  
  int ishift = x + y*fNXBIN[i] + z*fNXBIN[i]*fNYBIN[i];

  return fScored[i][ishift];
}

float UsbSuw::GetError(int i, int x, int y, int z) const
{
  /*
    Return the scored value for xyz cell in the i-th histogram.
    The sell indises must be >= 1
   */
  x = x-1;
  y = y-1;
  z = z-1;
  
  int ishift = x + y*fNXBIN[i] + z*fNXBIN[i]*fNYBIN[i];

  return fGBSTOR[i][ishift];
}


void UsbSuw::Print() const
{
  std::cout << std::scientific;// << std::setprecision(4);

  std::cout << "UsbSuw::Print" << std::endl;
  std::cout << GetRunTitle() << std::endl;
  std::cout << GetRunTime() << std::endl;
  std::cout << "weight: " << GetWEIPRI() << std::endl;
  std::cout << "ncase: " << GetNCASE() << std::endl;
  std::cout << "mcase: " << GetMCASE() << std::endl;
  std:: cout << "nbatch: " << GetNBATCH() << std::endl;

  for (int i=0; i<1; i++) {
    if (fITUSBN[i] == 10) std::cout << "Cartesian ";
    std::cout << " binning n. " << fMCASE+1 << " \"" << fTITUSB[i] << "\"";
    std::cout << ", generalized particle n. " << fIDUSBN[i] << std::endl;
    
    std::cout << "\tX coordinate from " << fXLOW[i] << " to " << fXHIGH[i] << " cm,\t" << fNXBIN[i] << " bins (" << fDXUSBN[i] << " cm wide)" << std::endl;
    std::cout << "\tY coordinate from " << fYLOW[i] << " to " << fYHIGH[i] << " cm,\t" << fNYBIN[i] << " bins (" << fDYUSBN[i] << " cm wide)" << std::endl;
    std::cout << "\tZ coordinate from " << fZLOW[i] << " to " << fZHIGH[i] << " cm,\t" << fNZBIN[i] << " bins (" << fDZUSBN[i] << " cm wide)" << std::endl;

    std::cout << "\tData follow in a matrix A(ix,iy,iz), format (1(5x,1p,10(1x,e11.4)))" << std::endl << std::endl;
    if (fITUSBN[i]>=10) std::cout << "\taccurate deposition along the tracks requested" << std::endl;
    if (fIDUSBN[i]<200) std::cout << "\tthis is a track-length binning" << std::endl; // is it correct?

    //    std::cout << "lntzer: " << fLNTZER[i] << std::endl;
    // std::cout << "Birk's parameters: " << fBKUSBN[i] << " " << fB2USBN[i] << std::endl;
    //std::cout << "TCUSBN: " << fTCUSBN[i] << " sec" << std::endl;
    //return;
    
    std::cerr << "nbins: " << GetNbins(i) << std::endl;

    for (int ibin=0; ibin<GetNbins(i); ibin++) {
      if (ibin==0) std::cout << "\t";
      std::cout << fScored[i][ibin] << " " << std::flush;
      if ( (ibin+1)%10 == 0) std::cout << std::endl << '\t';
    }
    
  }
}
