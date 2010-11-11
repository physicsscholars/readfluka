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

  ReadInt(2); // !!! What is this? !!!
  CheckFormat();
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


  CheckFormat();

  if (fLLNUSX) { //std::cout << " read low energy neutrons" << std::endl;
    fIGMUSX = ReadInt(); //std::cout << "igmusx: " << fIGMUSX << std::endl;
    fENGMAX = new float[fIGMUSX+1];
    for (int i=0; i<fIGMUSX+1; i++) {
      fENGMAX[i] = ReadFloat();
    }
    CheckFormat(); 
  } else fIGMUSX = 0;
  // + see page 239 for further reading

  // INTERV - total number of enery intervals
  // (interval above the limit of n-groups+ intervals below)
  int INTERV = fNEBXBN + fIGMUSX;
  fNScored = INTERV*fNABXBN;
  fScored = new float[fNScored];

  // read the scoring results as a 1-dimentional array
  for (int i=0; i<fNScored; i++) fScored[i] = ReadFloat();

  CheckFormat(); // std::cerr << "+++ OK in the end of UsrBdx::Read +++" << std::endl;


  delete [] mychar; mychar = 0; // is it ok? !!!
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
  if (abs(GetType())<=1) return "#Omega [sr]";
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
  std::cout << std::endl;
  std::cout << "Bdrx n. " << GetCardNumber() << " \"" << GetBinName() << "\"" <<  std::flush;
  std::cout << ", generalized particle n. " << GetID() << std::flush;
  std::cout << ", from region n. " << GetRegFrom() << " to region n. " << GetRegTo() << std::endl;
  std::cout << "\tdetector area: " << GetArea() <<  " cm**2" << std::endl;
  if (IsReadNeutrons()) {
    std::cout << "\tlow energy neutrons scored from group 1" << " to group " << GetMaxNeutronGroup() << std::endl;
  }
  if (IsOneWay() == true) 
    std::cout << "\tthis is a one way only estimator" << std::endl; 
  else
    std::cout << "\tthis is a two ways estimator" << std::endl;
  
  if (IsFluence() == true) 
    std::cout << "\tthis is a fluence like estimator" << std::endl;
  else
    std::cout << "\tthis is a current-like estimator" << std::endl;

  std::cout.precision(4);

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
  
  //  std::cout << "\tData follow in a matrix A(ie,ia), format (1(5x,1p,10(1x,e11.4)))" << std::endl;
  int n = GetNScored();
  n = GetNbinsE()*GetNbinsA();
  /*
    std::cout << std::endl << "\t";
    for (unsigned int i=1; i<=GetNbinsA(); i++) {
    for (unsigned int j=1; j<=GetNbinsE(); j++)
    std::cout << GetScored(j, i) << " ";
    std::cout << std::endl << "\t";
    }
  */
  std::cout << std::endl;
  
  // loop on angles - line 125
  double cumul = 0.0;
  for (unsigned int ia=1; ia<=fNABXBN; ia++) { // angular intervals
    if (abs(fITUSBX)<=1) { // linear in angle
      std::cout << "\tAngle " << ia << " between " << fABXLOW + (ia-1)*fDABXBN << " and " << fABXLOW + ia*fDABXBN << " sr" << std::endl;
    } else { // logarithmic in angle
      if (ia == 1) // fist bin
	std::cout << "\tAngle 1 between 0 and " << fABXLOW << " sr" << std::endl;
      else
	std::cout << "\tAngle " << ia << " between " << fABXLOW*pow(fDABXBN, ia-1) << " and " << fABXLOW*pow(fDABXBN, ia) << " sr" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "\t                                Double Differential      Angle-Integrated Cumulative" << std::endl;
    if (GetLFUSBX()) // fluence
      std::cout << "\t                              Fluence (dPhi/dE/dOmega)      dPhi/dE        Fluence" << std::endl;                 
    else // current
      std::cout << "\t                               Current (dJ/dE/dOmega)        dJ/dE         Current" << std::endl;
    
    std::cout << "\tLower energy     Upper energy    cm**-2 GeV**-1 sr-1      cm**2 GeV-1     cm**-2" << std::endl;
    
    float elimit = fEBXLOW;
    float en1,en2, angint;
    unsigned int nhigh;
    if (fLLNUSX) { // low energy neutrons - data are stored backwards
      int ig = fIGMUSX;// std::cout << "fIGMUSX: " << fIGMUSX << std::endl;
      en1 = fENGMAX[ig+1-1]; // +1-1 - it's a FORTRAN/C issue
      unsigned int jg1 = ia*(fNEBXBN+fIGMUSX);
      // kbat
      jg1--;
      unsigned int jg2 = ia*(fNEBXBN+fIGMUSX)-fIGMUSX+1;
      // kbat
      jg2--;
      //std::cout << "jg: " << jg1 << " " << jg2 << std::endl;
      for (unsigned int jg=jg1; jg>=jg2; jg--) {
	//	std::cout << "jg: " << jg << std::endl;
	en2 = fENGMAX[jg-1];
	angint = fScored[jg] * fDABXBN;
	cumul += angint*(en2-en1);
	std::cout  << "\t" << en1 << "\t" << en2 << "\t\t";
	std::cout.precision(7);
	std::cout << fScored[jg] << "\t" << angint << "\t" << cumul << std::endl;
	ig--;
	en1 = en2;
      }
      // find lower limit of first bin above or straddling the
      // n-group limit. Nhigh: counts the high energy bins
      nhigh = 0;
      // for the time being, set energy boundary at n-group limit
      elimit = en1;
      en1 = fEBXHGH;
      for (unsigned int ie = 1; ie<=fNEBXBN; ie++) {
	if (fITUSBX>0)  // type of the binning
	  en2 = en1 - fDEBXBN;
	else
	  en2 = en1/fDEBXBN;
	en1 = en2;
	nhigh++;
	if (en2<=elimit) break;
      }
    } else {
      en1 = fEBXLOW;
      nhigh = fNEBXBN;
    }
    //     first bin above or straddling the n-group limit
    if (fITUSBX>0)
      en2 = en1 + fDEBXBN;
    else
      en2 = en1 * fDEBXBN;
    float diff = fScored[ia*(fNEBXBN-nhigh+1)-1];
    angint = diff*fDABXBN;
    cumul += angint*(en2-elimit);
    std::cout << "\t" << elimit << "\t" << en2 << "\t\t";
    std::cout.precision(7);
    std::cout << diff << "\t" << angint << "\t" << cumul << std::endl;
    en1 = en2;

    //     -------- loop on energies above the n-group limit ----------
    // line 205 !!! this section have not yet been checked !!!
    //std::cout << " nhigh: " << nhigh << std::endl;
    for (unsigned int ie=2; ie<=nhigh; ie++) {
      if (fITUSBX > 0) // binning is linear
	en2 = en1 + fDEBXBN;
      else
	en2 = en1 * fDEBXBN;
      diff = fScored[ ia*(fNEBXBN-nhigh+ie) - 1 ];
      angint = diff * fDABXBN;
      cumul += angint*(en2-en1);
      std::cout << "here" << std::endl;
      std::cout << en1 << " " << en2 << " " << diff << " " << angint << " " << cumul << std::endl;
      en1 = en2;
    }

    //     Case of generalized particles .ne. 8 but including neutrons
    // !!! this section have not been checked yet !!!
    float cumule = 0.0; // used but not defined in rdbdx.f
    float cumula = 0.0; // used but not defined in rdbdx.f
    if (fLLNUSX && fIDUSBX != 8 && fNEBXBN > nhigh) {
      std::cout << "Particles other than neutrons below E=" << elimit << std::endl;
      en1 = fEBXLOW;
      for (unsigned int ie = 1; ie<=(fNEBXBN-nhigh); ie++) {
	if (fITUSBX>0)
	  en2 = en1 + fDEBXBN;
	else
	  en2 = en1 * fDEBXBN;
	diff = fScored[ (ia-1)*(fNEBXBN+fIGMUSX) + ie - 1 ];
	angint = diff * fDABXBN;
	cumul += angint * (en2-en1);
	std::cout << en1 << " " << en2 << " " << diff << " " << cumule << " " << cumula << std::endl;
      }
    }

  }
  
  
  /*  if (IsReadNeutrons()) {
    std::cout << "\tLow energy neutron data from group 1 to group " << GetMaxNeutronGroup() <<  " follow in a matrix A(ig,ia), format (1(5x,1p,10(1x,e11.4)))" << std::endl;
    
    
    std::cerr << "!!! check it again !!! WHY ZEROS? why do we need the neutron data?" << std::endl;
    
    
    n = GetMaxNeutronGroup();
    for (int i=0; i<n; i++) std::cout << GetENGMAX(i) << " "; std::cout << std::endl;
    n = GetNScored();
    //      for (int i= GetNbinsE()*GetNbinsA(); i<n; i++)  std::cout << GetScored(i) << " "; 
    std::cout << std::endl << std::endl;
    }*/
}
