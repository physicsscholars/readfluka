#include <iostream>
#include <iomanip>
#include "UsrBin.h"

using namespace ReadFluka;

UsrBin::UsrBin(const char *fname) : Base(fname)
{
  fTITUSB = new char[11];
  fITUSBN = 0;
  fMB = 0;
  fWEIPRI = 0.0f;
  fNCASE  = 0;
  fIDUSBN = 0;
  fXLOW = fXHIGH = fYLOW = fYHIGH = fZLOW = fZHIGH = 0.0f;
  fNXBIN = fNYBIN = fNZBIN = 0;
  fDXUSBN = fDYUSBN = fDZUSBN = 0.0f;
  fScored = 0;
  fScoredVec.clear();
  fIsReadHeader = false;
  fFirstRead = true;
  
  fReadHeader();
}

UsrBin::~UsrBin()
{
  if (fTITUSB) delete [] fTITUSB; fTITUSB = 0;
  if (fScored) delete [] fScored; fScored = 0;
  fScoredVec.clear();
}

void UsrBin::fReadHeader()
{
  /*
    read the USRBIN header
  */
  
  if (fIsReadHeader == false) { // did not read the header yet?
    fIsReadHeader = true;
    
    ReadRunTitle();
    ReadRunTime();
    
    fWEIPRI = ReadFloat();
    fNCASE  = ReadInt();

    if (gVerbose>=kPRINT_MISC) {
      std::cout << "\tTotal number of particles followed\t" << fNCASE;
      std::cout << ", for a total weight of  ";
      std::cout.setf(std::ios::scientific);
      std::cout << std::setprecision(4) << fWEIPRI << std::endl;
    }
  }

  std::cout << "UsrBin::ReadHeader: " << ReadInt() << " " << ReadInt() << std::endl;
  //  std::cout << "here" << std::endl;
  CheckFormat();
  //  std::cout << "b" << std::endl; 
}

bool UsrBin::Read()
{
  if (!fin->good()) return false;

  fMB = ReadInt();
  
  /*if (fFirstRead == true) {
    std::cout << "here2" << std::endl;
    //   ReadInt(2); // these 2 ints were introduced in FLUKA2008 => understand what are they and use them
    if (gVerbose==kPRINT_NOTHING) ReadInt(2);
    else std::cerr << "fluka2008: " << ReadInt() << " " << ReadInt() << std::endl;
    fFirstRead = false;
    }*/
  
  fin->read(fTITUSB, 10);
  fTITUSB[10] = '\0';
  
  // cut the spaces from the both sides (if any):
  sprintf(fTITUSB, "%s", Base::Trimmed(std::string(fTITUSB)).c_str());
  //  std::cout << "titusb: " << fTITUSB << std::endl;

  fITUSBN = ReadInt();
  if ((fITUSBN<0) || (fITUSBN>17)) std::cerr << "error in UsrBin::Read():\tfITUSBN=" << fITUSBN << " - unknown type of binning" << std::endl;
  
  fIDUSBN = ReadInt();
  fXLOW   = ReadFloat();
  fXHIGH  = ReadFloat();
  fNXBIN  = ReadInt();
  fDXUSBN = ReadFloat();
  
  fYLOW   = ReadFloat();
  fYHIGH  = ReadFloat();
  fNYBIN  = ReadInt();
  fDYUSBN = ReadFloat();
  
  fZLOW   = ReadFloat();
  fZHIGH  = ReadFloat();
  fNZBIN  = ReadInt();
  //  if (fNZBIN > 1) std::cerr << "Warning from UsrBin::Read(): number of Z bins > 1 in " << fTITUSB << std::endl;   
  fDZUSBN    = ReadFloat();
  
  if (gVerbose>=kPRINT_MISC) std::cout << "number of binning: " << fMB << std::endl;
  if (gVerbose>=kPRINT_HEADER) {
    std::cout << "  binning name: " << fTITUSB << ";\ttype: " << fITUSBN << ";\tdistribution to be scored: " << fIDUSBN << std::endl;
    std::cout << "    " << fXLOW << " < x < " << fXHIGH << ":\t" << fNXBIN << " intervals " << fDXUSBN << " cm wide"  << std::endl;
    std::cout << "    " << fYLOW << " < y < " << fYHIGH << ":\t" << fNYBIN << " intervals " << fDYUSBN << " cm wide"  << std::endl;
    std::cout << "    " << fZLOW << " < z < " << fZHIGH << ":\t" << fNZBIN << " intervals " << fDZUSBN << " cm wide"  << std::endl << std::endl;
  }
  
  bool LNTZER=false;
  float BKUSBN=0.0, B2USBN=0.0, TCUSBN=1.0E+38;
  LNTZER = ReadBool();
  BKUSBN = ReadFloat();
  B2USBN = ReadFloat();
  TCUSBN = ReadFloat();

  CheckFormat();

  if (gVerbose>=kPRINT_MISC) {
    std::cout << "LNTZER: " << LNTZER << std::endl;
    std::cout << "BKUSBN: " << BKUSBN << std::endl;
    std::cout << "B2USBN: " << B2USBN << std::endl;
    std::cout << "TCUSBN: " << TCUSBN << std::endl;
  }
  
  const int ituhlp = fITUSBN % 10;
  
  if (ituhlp == 0) {
    return fReadCartesian();
  } else if (ituhlp==2) {
    return fReadRegions();
  } else {
    //    std::cerr << "I can only read Cartesian (0) and Region (2) binnings for the time being: " << ituhlp << std::endl;
    std::cerr << "WARNING from UsrBin::Read():\t default reading function is used for this kind of binning" << std::endl;
    return fReadRegions();
  }

  return true;
}

bool UsrBin::fReadBinning()
{
  if (fScored) delete fScored;
  fScored = new float[GetNbins()];
  fScoredVec.clear();
  
  if (gVerbose>=kPRINT_SCORED) std::cout << "fScored:\t";
  for (int i=0; i<GetNbins(); i++) {
    fScored[i]  = ReadFloat();
    fScoredVec.push_back(fScored[i]);
    if (gVerbose>=kPRINT_SCORED)	std::cout << fScored[i] << ' ';
  }
  if (gVerbose>=kPRINT_TITLE) std::cout << std::endl;

  CheckFormat();

  return true;
}

bool UsrBin::fReadCartesian()
{
  if (gVerbose>=kPRINT_MISC) {
    std::cout << 1 << std::endl << "   Cartesian binning n. ";
    if (fMB == 128) std::cout << 2;		else std::cout << fMB+1; // is it correct? why it's like that?
    std::cout << " \""  << fTITUSB << "\" , ";
    std::cout << "generalized particle n. " << fIDUSBN << std::endl;
    
    std::cout<<"\tX coordinate: from "<<fXLOW<<" to "<<fXHIGH<<" cm, "<<fNXBIN<<" bins ("<<fDXUSBN<<" cm wide)"<<std::endl;
    std::cout<<"\tY coordinate: from "<<fYLOW<<" to "<<fYHIGH<<" cm, "<<fNYBIN<<" bins ("<<fDYUSBN<<" cm wide)"<<std::endl;
    std::cout<<"\tZ coordinate: from "<<fZLOW<<" to "<<fZHIGH<<" cm, "<<fNZBIN<<" bins ("<<fDZUSBN<<" cm wide)"<<std::endl;
    
    std::cout<<"\tData follow in a matrix A(ix,iy,iz), format (1(5x,1p,10(1x,e11.4)))" << std::endl << std::endl;
    if (fITUSBN>=10) std::cout<<"\taccurate deposition along the tracks requested" << std::endl;
    if (fIDUSBN<200) std::cout<<"\tthis is a track-length binning" << std::endl; // is it correct?
  }
  else if (gVerbose>kPRINT_TITLE) std::clog << std::endl << "read cartesian binning" << std::endl;

  return fReadBinning();
}

bool UsrBin::fReadRegions()
{
  return fReadBinning();
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
	if (fScored == 0) return;
  
  for (int i=0; i<GetNbins(); i++)
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

