#include <iostream>
#include "UsrBin.h"

using namespace ReadFluka;

UsrBin::UsrBin(const char *fname) : Reader(fname)
{
  gVerbose = 1;
  fTITUSB = new char[11];
  fITUSBN = 0;
  fWEIPRI = 0;
  fNCASE = 0;
  fIDUSBN = 0;
  fXLOW = fXHIGH = fYLOW = fYHIGH = fZLOW = fZHIGH = 0;
  fNXBIN = fNYBIN = fNZBIN = fNbin = 0;
  fDXUSBN = fDYUSBN = fDZUSBN = 0;
  fScored = 0;
  fIsReadHeader = false;
  fUsbReaFlag = false;

  fReadHeader();
}

UsrBin::~UsrBin()
{
  if (fTITUSB) delete [] fTITUSB; fTITUSB = 0;
  if (fScored) delete [] fScored; fScored = 0;
}

void UsrBin::fReadHeader()
{
  // read the USRBIN header
  if (fIsReadHeader == false) { // did not read the header yet?
    fIsReadHeader = true;

    ReadRunTitle();
    ReadRunTime();

    fWEIPRI = ReadFloat();
    fNCASE  = ReadInt();
    if (gVerbose>=kPRINT_MISC) std::cout << "WEIPRI: " << fWEIPRI << std::endl << "NCASE: " << fNCASE << std::endl;
  }
}

bool UsrBin::Read()
{
  ReadInt(2);
  
  if (!fin->good()) return false;
	
  float mb = ReadInt();

	ReadInt(2); // these 2 ints were introduced in FLUKA2008 => read what are they and use them

  fin->read(fTITUSB, 10);
	fTITUSB[10] = '\0';

	// cut the spaces from the both sides (if any):
	std::string str_tmp(fTITUSB);
	sprintf(fTITUSB, "%s", Reader::Trimmed(str_tmp).c_str());

  fITUSBN = ReadInt();

  fIDUSBN = ReadInt();
  fXLOW  = ReadFloat();
  fXHIGH  = ReadFloat();
  fNXBIN = ReadInt();
  fDXUSBN    = ReadFloat();
 
  fYLOW  = ReadFloat();
  fYHIGH  = ReadFloat();
  fNYBIN = ReadInt();
  fDYUSBN    = ReadFloat();
  
  fZLOW  = ReadFloat();
  fZHIGH  = ReadFloat();
  fNZBIN = ReadInt();
	//  if (fNZBIN > 1) std::cerr << "Warning from UsrBin::Read(): number of Z bins > 1 in " << fTITUSB << std::endl;   
  fDZUSBN    = ReadFloat();
  
  if (gVerbose>=kPRINT_TITLE) std::cout << "mb: " << mb << std::endl;
  if (gVerbose>=kPRINT_HEADER) {
    std::cout << "  binning name: " << fTITUSB << ";\ttype: " << fITUSBN << ";\tdistribution to be scored: " << fIDUSBN << std::endl;
    std::cout << "    " << fXLOW << " < x < " << fXHIGH << ":\t" << fNXBIN << " intervals " << fDXUSBN << " cm wide"  << std::endl;
    std::cout << "    " << fYLOW << " < y < " << fYHIGH << ":\t" << fNYBIN << " intervals " << fDYUSBN << " cm wide"  << std::endl;
    std::cout << "    " << fZLOW << " < z < " << fZHIGH << ":\t" << fNZBIN << " intervals " << fDZUSBN << " cm wide"  << std::endl << std::endl;
  }

  bool LNTZER=false;
  float BKUSBN=0.0, B2USBN=0.0, TCUSBN=1.0E+38;
  LNTZER =  (bool)ReadInt();  if (gVerbose>1) std::cout << "LNTZER: " << LNTZER << std::endl;
  BKUSBN = ReadFloat(); if (gVerbose>1) std::cout << "BKUSBN: " << BKUSBN << std::endl;
  B2USBN = ReadFloat(); if (gVerbose>1) std::cout << "B2USBN: " << B2USBN << std::endl;
  TCUSBN = ReadFloat(); if (gVerbose>1) std::cout << "TCUSBN: " << TCUSBN << std::endl;

  fNbin = fNXBIN * fNYBIN * fNZBIN;

  if (fUsbReaFlag == false) {  
    const int ituhlp = fITUSBN % 10;
  
    if (ituhlp == 0)
      return fReadCartesian();
    else {
      std::cerr << "I can only read Cartesian binning for the time being." << std::endl;
      return false;
    }
  }
  return true;
}

bool UsrBin::fReadCartesian()
{
  if (gVerbose>1) std::clog << "read cartesian binning" << std::endl;

  if (fScored) delete fScored;
  fScored = new float[fNbin];

  ReadFloat(2);
  if (gVerbose>1)
    std::cout << "fScored:\t";
  for (int i=0; i<fNbin; i++) {
    fScored[i]  = ReadFloat();
    if (gVerbose>5)	std::cout << fScored[i] << ' ';
  }
  if (gVerbose>5) std::cout << std::endl;
  
  return true;
}

float UsrBin::GetScored(int i, int j, int k) const
{
	// Return the scored value in the ijk-cell. The cell indises must be >= 1

  int x=0, y=0, z=0;
  if (fNZBIN == 1) {
    x = i-1;
    y = j-1;
    z = k-1;
  } else {
    x = i-1;
    y = j-1;
    z = k-1;
  }

	return fScored[x + y*fNXBIN+z*fNXBIN*fNYBIN];
} 

const float UsrBin::GetXmin(int i, int j/*=1*/, int k/*=1*/) const
{
  i -= 1;
  j -= 1;
  k -= 1;
  return fCheckBounds(i, j, k) ? (fXLOW + j*fDXUSBN) : 0;
}

const float UsrBin::GetXmax(int i, int j/*=1*/, int k/*=1*/) const
{
  i -= 1;
  j -= 1;
  k -= 1;
  return fCheckBounds(i, j, k) ? (fXLOW + (j+1)*fDXUSBN) : 0;
}

const float UsrBin::GetYmin(int i, int j, int k/*=1*/) const
{
  i -= 1;
  j -= 1;
  k -= 1;
  return fCheckBounds(i, j, k) ? (fYLOW + i*fDYUSBN) : 0;
}

const float UsrBin::GetYmax(int i, int j, int k/*=1*/) const
{
  i -= 1;
  j -= 1;
  k -= 1;
  return fCheckBounds(i, j, k) ? (fYLOW + (i+1)*fDYUSBN) : 0;
}

const float UsrBin::GetZmin(int i, int j, int k) const
{
  i -= 1;
  j -= 1;
  k -= 1;
  return fCheckBounds(i, j, k) ? (fZLOW + k*fDZUSBN) : 0;
}

const float UsrBin::GetZmax(int i, int j, int k) const
{
  i -= 1;
  j -= 1;
  k -= 1;
  return fCheckBounds(i, j, k) ? (fZLOW + (k+1)*fDZUSBN) : 0;
}

const float UsrBin::GetVolume(int i, int j, int k) const
{
	// Return the ijk-bin volume.
	// Works only for Cartesian binning

  return (GetXmax(i, j, k)-GetXmin(i, j, k)) * (GetYmax(i, j, k)-GetYmin(i, j, k)) * (GetZmax(i, j, k)-GetZmin(i, j, k));
}

bool UsrBin::fCheckBounds(int x, int y, int z) const
{
  if ((x<fNXBIN) && (y<fNYBIN) && (z<fNZBIN)) {
    return true;
  } else {
    std::cerr << "from UsrBin:\tbin [" << x << "][" << y << "]["<< z << "] out of bounds:\t";
    std::cerr << "NXBIN = " << fNXBIN << " ";
    std::cerr << "NYBIN = " << fNYBIN << " ";
    std::cerr << "NZBIN = " << fNZBIN << std::endl;

    return false;
  }
}

void UsrBin::Print() const
{
	std::cout << "UsrBin::Print: " << std::flush;
  if (fScored == 0) exit(0);
  
  for (int i=0; i<GetNbin(); i++)
    std::cout << fScored[i] << " ";
  std::cout << std::endl;

  /*  
  if (fNZBIN == 1) {
    for (int i=0; i<fNXBIN; i++)
      for (int j=0; j<fNYBIN; j++)
	std::cout << GetXmin(i+1, j+1) << "\t" <<  GetXmax(i+1, j+1) << "\t" <<  GetScored(i+1, j+1) << std::endl;
  }
  */
}

