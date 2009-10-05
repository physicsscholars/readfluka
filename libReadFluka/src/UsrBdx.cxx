#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib> // for exit()

#include "UsrBdx.h"

using namespace ReadFluka;

UsrBdx::UsrBdx(const char *fname) : Base(fname)
{
  fIsReadHeader = false;
  fWEIPRI = 0;
  fNCASE  = 0;
  fENGMAX = 0;
  fScored = 0;
  Reset();

  ReadHeader();
}

UsrBdx::~UsrBdx()
{
  Reset();
}

void UsrBdx::Reset()
{
  fMX = 0;
  fTITUSX = "";
  fITUSBX = fIDUSBX = 0;
  fNR1USX = fNR2USX = 0;
  fAUSBDX = 0.0f;
  //  fLWUSBX = false;
  //  fLFUSBX = false;
  fEBXLOW = fEBXHGH = 0.0f;
  fNEBXBN = 0;
  fDEBXBN = 0.0f;
  fABXLOW = fABXHGH = 0.0f;
  fNABXBN = 0;
  fDABXBN = 0.0f;

  fIGMUSX = 0;
  if (fENGMAX) {
    delete [] fENGMAX;
    fENGMAX = 0;
  }

  fNScored = 0;
  if (fScored) {
    delete [] fScored;
    fScored = 0;
  }
}

void UsrBdx::ReadHeader()
{
  if (fIsReadHeader == true) return;

  fIsReadHeader = true;
  ReadRunTitle();
  ReadRunTime();
  fWEIPRI = ReadFloat();
  fNCASE  = ReadInt();

  if (gVerbose>=kPRINT_SCORED) {
        std::clog << "WEIPRI: " << fWEIPRI << std::endl;
    std::clog << "NCASE: "  << fNCASE << std::endl;
  }

  ReadInt(3);
  SizeStart();
}

bool UsrBdx::Read()
{
  if (fin->eof()) return false; // exit the while-loop if reached the end of the file
  Reset();

  //  std::cerr << "Read begin" << std::endl;
  fMX = ReadInt();  //std::cout << "MX: " << fMX << std::endl;

  char *mychar = new char[10];
  fin->read(mychar, 10); mychar[10]='\0';
  fTITUSX = Trimmed(std::string(mychar));  //std::cout << fTITUSX << std::endl;
  fITUSBX = ReadInt();// std::cout << "fITUSBX: " << fITUSBX << std::endl;
  fIDUSBX = ReadInt();   //std::cout << fIDUSBX << std::endl;
  fNR1USX = ReadInt();   
  fNR2USX = ReadInt();   
  fAUSBDX = ReadFloat();   
  fLWUSBX = ReadBool();   
  fLFUSBX = ReadBool();    
  fLLNUSX = ReadBool();   

  fEBXLOW = ReadFloat(); fEBXHGH = ReadFloat(); fNEBXBN = ReadInt();fDEBXBN = ReadFloat();
  fABXLOW = ReadFloat(); fABXHGH = ReadFloat(); fNABXBN = ReadInt(); fDABXBN = ReadFloat();


  SizeEnd(); SizeStart();

  if (fLLNUSX) { // read low energy neutrons
    fIGMUSX = ReadInt();
    fENGMAX = new float[fIGMUSX+1];
    for (int i=0; i<fIGMUSX+1; i++) fENGMAX[i] = ReadFloat();
    //    ReadFloat(2); // !!! Why? !!!
    SizeEnd(); SizeStart();
  } else fIGMUSX = 0;
  // + see page 239 for further reading

  // INTERV - total number of enery intervals
  // (interval above the limit of n-groups+ intervals below)
  int INTERV = fNEBXBN + fIGMUSX;
  fNScored = INTERV*fNABXBN;
  fScored = new float[fNScored];

  
  for (int i=0; i<fNScored; i++) fScored[i] = ReadFloat();

  SizeEnd(); SizeStart(); //std::cerr << "+++ OK in the end of UsrBdx::Read +++" << std::endl;

  return true;
}

float UsrBdx::GetScored(unsigned int ie, unsigned int ia) const
{
  /*
    Return the scored value in the ie-ja-cell.
    ie - energy bin
    ia - angular bin
    The cell indises must be >= 1 (FLUKA-Fortran style)
  */

  unsigned int e = ie-1;
  unsigned int a = ia-1;
  
  if (e>fNEBXBN) {
    std::cerr << "GetScored():\t" << e << " > " << fNEBXBN << std::endl;
    exit(FATAL_ERROR);
  }

  if (a>fNABXBN) {
    std::cerr << "GetScored():\t" << a << " > " << fNABXBN << std::endl;
    exit(FATAL_ERROR);
  }
  
  return fScored[e + a*fNEBXBN];
}

std::string UsrBdx::GetBinTitle() const
{
  std::ostringstream tmp;
  if (IsFluence()) tmp << "Fluence of ";
  else tmp << "Current of ";
  
  tmp << Code2Name(GetID()) << " (reg. " << GetRegFrom();

  if (IsOneWay()) tmp << " -> ";
  else tmp << " <-> ";
  tmp << GetRegTo();
  tmp << ", ";

  tmp << "S = ";
  tmp << GetArea();
  tmp << " cm^{2})";

  return tmp.str();
}

const char* UsrBdx::GetXtitle() const
{
  if (GetType()>0) return "E_{kin} [GeV]";
  else return "lg(E_{kin}/1 GeV)";
}

const char* UsrBdx::GetYtitle() const
{
  if (abs((float)GetType())<=1) return "#Omega [sr]";
  else return "lg(#Omega/1 sr)";
}

/*bool UsrBdx::IsLogA() const
{
  int i= int(std::abs(float(GetType())));
  std::cout << "nint: " << i << std::endl;
  if (i<=1) std::cout << "nint says: logA" << std::endl;

  return (i<=1);
  }*/

void UsrBdx::Print() const
{
  std::cout << "Bdrx n. " << GetCardNumber() << " \"" << GetBinName() << "\"" <<  std::flush;
  std::cout << ", generalized particle n. " << GetID() << std::flush;
  std::cout << ", from region n. " << GetRegFrom() << " to region n. " << GetRegTo() << std::endl;
  std::cout << "\tdetector area: " << GetArea() <<  " cm**2" << std::endl;
  if (IsReadNeutrons()) {
    std::cout << "\tlow energy neutrons scored from group 1" << " to group " << GetMaxNeutronGroup() << std::endl;
  }
  if (IsOneWay() == true) 
    std::cout << "\tthis is a two ways estimator" << std::endl; 
  else
    std::cout << "\tthis is a one way only estimator" << std::endl;
  
  if (IsFluence() == true) 
    std::cout << "\tthis is a fluence like estimator" << std::endl;
  else
    std::cout << "\tthis is a current like estimator" << std::endl;
  
  if (!IsLogE()) {
    std::cout << "\tlinear energy binning from " << GetEmin() << " to " << GetEmax()  << " GeV,\t" << std::flush;
    std::cout << GetNbinsE() << " bins ("  << GetEWidth() <<  " GeV wide)" << std::endl;
  } else {
    std::cout << "\tlogar. energy binning from " << GetEmin() << " to " << GetEmax() << " GeV,\t" << std::flush;
    std::cout << GetNbinsE() << " bins (ratio: " << GetEWidth() << ")" << std::endl;
  }
  
  if (!IsLogA()) {
    std::cout << "\tlinear angular binning from "<<GetAmin()<<" to "<< GetAmax() << " sr,\t"<< std::flush;
    std::cout <<  GetNbinsA() << " bins (" << GetAwidth() << " sr wide)"  << std::endl;
  } else {
    std::cout << "\tlogar. angular binning from "<<GetAmin()<<" to "<< GetAmax() << " sr,\t"<< std::flush;
    std::cout <<  GetNbinsA() << " bins (ratio: " << GetAwidth() << ")"  << std::endl;
  }
  
  std::cout << "\tData follow in a matrix A(ie,ia), format (1(5x,1p,10(1x,e11.4)))" << std::endl;
  int n = GetNScored();
  n = GetNbinsE()*GetNbinsA();
  std::cout << std::endl << "\t";
  for (unsigned int i=1; i<=GetNbinsA(); i++) {
    for (unsigned int j=1; j<=GetNbinsE(); j++)
      std::cout << GetScored(j, i) << " ";
    std::cout << std::endl << "\t";
  }
  std::cout << std::endl;
  
  
  if (IsReadNeutrons()) {
    std::cout << "\tLow energy neutron data from group 1 to group " << GetMaxNeutronGroup() <<  " follow in a matrix A(ig,ia), format (1(5x,1p,10(1x,e11.4)))" << std::endl;
    
    
    std::cerr << "!!! check it again !!! WHY ZEROS? why do we need the neutron data?" << std::endl;
    
    
    //  n = GetMaxNeutronGroup();
    //  for (int i=0; i<n; i++) std::cout << GetENGMAX(i) << " "; std::cout << std::endl;
    n = GetNScored();
    //      for (int i= GetNbinsE()*GetNbinsA(); i<n; i++)  std::cout << GetScored(i) << " "; 
    std::cout << std::endl << std::endl;
  }
}
