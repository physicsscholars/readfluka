#include "UsrYield.h"

#include <string.h>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace ReadFluka;

UsrYield::UsrYield(const char *fname) : Base(fname)
{
  fIsReadHeader = false;
  fWEIPRI = 0;
  fNCASE  = 0;
  fIJUSYL = fJTUSYL = 0;
  fPUSRYL = fSQSUYL = 0.0;
  fUUSRYL = fVUSRYL = fWUSRYL = 0.0;
  fTITUYL = new char[11];
  fSCORED = 0;
  fENGMAX = 0;

  Reset();
  ReadHeader();
}

void UsrYield::Reset()
{
  fMY = fITUSYL = fIXUSYL = fIDUSYL = fNR1UYL = fNR2UYL = 0;
  fUSNRYL = fSGUSYL = 0.0;
  fLLNUYL = false;
  //  for (int i=0; i<10; i++) fTITUYL[i] = "";
  fEYLLOW = fEYLHGH = 0.0;
  fNEYLBN = 0;
  fDEYLBN = 0.0;
  fAYLLOW = fAYLHGH = 0;
  fIGMUYL = 0;
  if (fSCORED) { delete [] fSCORED; fSCORED = 0;}
  if (fENGMAX) { delete [] fENGMAX; fENGMAX = 0;}
  fINTERV = 0;
  fie = fia = 0;
  fixa = fixm = 0;
}



UsrYield::~UsrYield()
{
  Reset();
  delete [] fTITUYL; fTITUYL = 0;
}

void UsrYield::ReadHeader()
{
  if (fIsReadHeader == false) { // did not read the header yet?
    fIsReadHeader = true;
    
    
    ReadRunTitle();
    ReadRunTime();
    
		fWEIPRI = ReadFloat();  // 0x74    
    fNCASE  = ReadInt();  //0x78

		ReadInt(2);

		/*    fIJUSYL = ReadInt(); // 0x80
    fJTUSYL = ReadInt();       
    fPUSRYL = ReadFloat();    
    fSQSUYL = ReadFloat();     
    fUUSRYL = ReadFloat();     
    fVUSRYL = ReadFloat();    
    fWUSRYL = ReadFloat();   */
    
    if (1) {
      std::cout << "WEIPRI:\t" << fWEIPRI << std::endl;
      std::cout << "NCASE:\t" << fNCASE << std::endl;
      std::cout << "IJUSYL:\t" << fIJUSYL << std::endl;
      std::cout << "JTUSYL:\t" << fJTUSYL << std::endl;
      std::cout << "PUSRYL:\t" << fPUSRYL << std::endl;
      std::cout << "SQSUYLL:\t" << fSQSUYL << std::endl;
      std::cout << "UUSRYL:\t" << fUUSRYL << std::endl;
      std::cout << "VUSRYL:\t" << fVUSRYL << std::endl;
      std::cout << "WUSRYL:\t" << fWUSRYL << std::endl;
    }
  }
}

bool UsrYield::Read()
{
  Reset();
	std::string str;
	if (1) {
		std::cout << "float: " << ReadFloat() << " " << ReadFloat() << std::endl;
	} else ReadFloat(2);

  if (!fin->good()) {
		std::cerr << "UsrYield::Read:\t read error" << std::endl;
		return false;
	}
  for (int i=0; i<1; i++) {
    fMY = ReadInt();
    fin->read(fTITUYL, 10);
    fTITUYL[10] = '\0';
    str = fTITUYL;
    strcpy(fTITUYL, Trimmed(str.c_str()).c_str());
    
    fITUSYL = ReadInt();
    fie = fITUSYL % 100;
    fia = (fITUSYL-fie) / 100;
    
    fIXUSYL = ReadInt();
    fixa = fIXUSYL % 100;
    fixm = (fIXUSYL-fixa) / 100;
    
    fIDUSYL = ReadInt();
    fNR1UYL = ReadInt();
    fNR2UYL = ReadInt();
    fUSNRYL = ReadFloat();
    fSGUSYL = ReadFloat();
    fLLNUYL = ReadBool();
    fEYLLOW = ReadFloat();
    fEYLHGH = ReadFloat();
    fNEYLBN = ReadInt();
    fDEYLBN = ReadFloat();
    fAYLLOW = ReadFloat();
    fAYLHGH = ReadFloat();
    
    ReadInt();
    
    if (1) {
      std::cout << "MY:\t" << fMY << std::endl;
      std::cout << "TITUYL:\t" << fTITUYL << std::endl;
      std::cout << "ITUSYL:\t" << fITUSYL << std::endl;
      std::cout << "IXUSYL:\t" << fIXUSYL << std::endl;
      std::cout << "IDUSYL:\t" << fIDUSYL << std::endl;
      std::cout << "from region " << fNR1UYL << " to region " << fNR2UYL << std::endl;
      std::cout << "user normalization factor:\t" << fUSNRYL << std::endl;
      std::cout << "adopted cross section:\t" << fSGUSYL << std::endl;
      std::cout << fLLNUYL << std::endl;

      std::cout << "first variable: from " << fEYLLOW;
      if (fITUSYL % 100 <=2 )	std::cout << " GeV,";
      else if (fITUSYL % 100 == 14) std::cout << " rad, ";
      std::cout << " to " << fEYLHGH;
      if (fITUSYL % 100 <=2 ) std::cout << " GeV,";
      else if (fITUSYL % 100 == 14) std::cout << " rad, ";
      std::cout << " " << fNEYLBN << " bins (";
      std::cout << fDEYLBN <<" GeV wide)"<< std::endl;
      std::cout << "angle from " << fAYLLOW << " to " << fAYLHGH << " ster" << std::endl;
    }
    double AAUSYL = 0.01L * fITUSYL;
    int IAUSYL=0;

    // NINT
    if (AAUSYL >= 0) {
      IAUSYL = int(AAUSYL + 0.5);
      if (AAUSYL + 0.5 == float(IAUSYL) && IAUSYL & 1) IAUSYL--;
    } else {
      IAUSYL = int(AAUSYL - 0.5);
      if (AAUSYL - 0.5 == float(IAUSYL) && IAUSYL & 1) IAUSYL++;
    }
    //
    //    int IEUSYL = fITUSYL - IAUSYL*100;
    
    if (fLLNUYL){
      ReadInt();
      // if low-energy neutrons, read group energies
      if (gVerbose) std::cout << "low-energy neutrons requested" << std::endl;
      fIGMUYL = ReadInt();
      if (gVerbose>2) std::cout << "ENGMAX[" << fIGMUYL+1 << "]:\t";
      fENGMAX = new float[fIGMUYL+1];
      for (int i=0; i<fIGMUYL+1; i++) {
	fENGMAX[i] = ReadFloat();
	if (gVerbose>2) std::cout << fENGMAX[i] << ' ';
      }
      ReadFloat();
      if (gVerbose>2) std::cout << std::endl;
    } else fIGMUYL = 0;
    
    //  ----- if second variable is angle, differential is in sr ------
    double TTLOW=0L, TTHGH=0L, DAYLBN=0L;
    if ( (IAUSYL == 14) || (IAUSYL == 15) ) {
      if (fAYLHGH < 0.3L * M_PI) {
	// small angle (exact!)
	TTLOW = tan(fAYLLOW/2.0L);
	TTHGH = tan(fAYLHGH/2.0L);
	DAYLBN = M_PI*2.0L * 2.0L * ( TTHGH + TTLOW ) * ( TTHGH - TTLOW ) / ( 1.0L + pow(TTLOW, 2) ) / ( 1.0L + pow(TTHGH, 2) );
      } else {
	// large angle:
	DAYLBN = M_PI*2.0L * ( cos((double)fAYLLOW) - cos((double)fAYLHGH) );
      }
    } else {
      DAYLBN = (double)fAYLHGH - (double)fAYLLOW;
    }
    
    fINTERV = fNEYLBN + fIGMUYL;
		//    std::cout << "number of scored values: " << fNEYLBN << std::endl;
    fSCORED = new float[fNEYLBN];
    
		//    std::cout << "scored[" << fNEYLBN << "]:\t";
    ReadFloat();
    for (int i=0; i<fNEYLBN; i++) {
      fSCORED[i] = ReadFloat();
    }
    ReadFloat(fIGMUYL); // read these zeros
    std::cout << std::endl;
    
    //    if (IEUSYL > 0)

  }
  
	//  MakeHist();
  
  return true;
}

const char *UsrYield::GetXTitle() const {
  std::string x1("x");
  
  if (fie<0) x1 = "log ";
  else x1 = "";
  
  switch ((int)std::abs((float)fie)) {
  case (5): x1 += "#eta_{lab}";
    break;
  case (6): x1 += "#eta_{cms}";
    break;
  case (14): x1 += "#theta_{lab}";
    break;
  }
  
  return x1.c_str();
}

const char *UsrYield::GetYTitle() const {
  std::string y("y");
  std::string x2;
  
  switch((int)std::abs((float)fia)) {
  case(1):
    x2 = "E_{kin}";
    break;
  }
  
  switch (fixa) {
  case(3): 
    if (x2.length())
      y = "#frac{d^{2}N}{d";
    else y = "#frac{dN}{d";
    y += GetXTitle();
    if (x2.length()) {
      y += "d";
      y += x2;
    }
    y += "}";
    break;
  }
  return y.c_str();
}

const char *UsrYield::GetNameOfScoredParticles() const {
	std::string name = "";
  //  sprintf(name, "%d", GetTypeOfScoredParticles());
  
  switch(GetTypeOfScoredParticles()) {
  case(202):
    name = "ALL_CHAR";
    break;
  }
  return name.c_str();
}
