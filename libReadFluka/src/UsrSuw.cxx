#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib> // for exit()

#include "UsrSuw.h"

using namespace ReadFluka;

UsrSuw::UsrSuw(const char *fname) : ResNuclei(fname)
{
}

UsrSuw::~UsrSuw()
{
  Reset();
}

void UsrSuw::Reset()
{
  /*
    Reset the private members
   */
  fNRN.clear();
  fTIURSN.clear();
  fITURSN.clear();
  fNRURSN.clear();
  fVURSNC.clear();
  fIMRHGH.clear();
  fIZRHGH.clear();
  fK.clear();
  fN = 0;
  fRNERR.clear();
  fTotalResp.clear();
  fTotalRespErr.clear();
  fYieldA.clear();
  fYieldAErr.clear();
  fYieldZ.clear();
  fYieldZErr.clear();
}



bool UsrSuw::Read()
{
  if (fin->eof()) return false;
  Reset();

  char *mychar = new char[11];
  std::vector <float> vec; // scored values
  std::vector< std::vector<float> > vec2;
  float tmp, tmperr;
  int before = fin->tellg();
  int after;
  for (;;) {
    after = fin->tellg();
    //    std::cout << "before and after: " << before << " " << after << std::endl;
    fNRN.push_back(ReadInt()); //std::cout << "NRN: " << fNRN << std::endl;
    
    fin->read(mychar, 10); mychar[10] = '\0'; // !!! is it necessary \0?
    fTIURSN.push_back(Trimmed(std::string(mychar))); std::cout << "name: " << mychar << std::endl;
    
    fITURSN.push_back(ReadInt());
    fNRURSN.push_back(ReadInt());
    fVURSNC.push_back(ReadFloat());
    fIMRHGH.push_back(ReadInt());
    fIZRHGH.push_back(ReadInt());
    fK.push_back(ReadInt());
    
    //std::cout << "checkpoint1: " << fin->tellg() << std::endl;
    CheckFormat();
    //std::cout << "checkpoint2: " << fin->tellg() << std::endl;
    vec2.clear();
    for (int i=0; i<fIMRHGH[fN]; i++) {
      vec.clear();
      for (int j=0; j<fIZRHGH[fN]; j++) {
	tmp = ReadFloat();
	vec.push_back(tmp); //std::cout << tmp << std::endl;
      }
      vec2.push_back(vec);
    }
    fRNDATA.push_back(vec2);
    

    fN++;
    CheckFormat();
    if (ReadStatFlag(false) == true) break;
    before = fin->tellg();
  }
  
  std::cout << "total number of histograms found: " << fN << std::endl;

  CheckFormat();
  
  for (unsigned short iN=0; iN<fN; iN++) {
    tmp = ReadFloat();
    tmperr = ReadFloat();
    fTotalResp.push_back(tmp);
    fTotalRespErr.push_back(tmperr);
    std::cout << "total responce: " << tmp << " ± "  << tmperr << std::endl;
    
    CheckFormat();
    
    // Isotope Yield as a function of Mass Number
    std::cerr << "yield " << std::flush;
    vec.clear();
    for (int i=GetAmin(iN); i<=GetAmax(iN); i++) { 
      tmp = ReadFloat();
      vec.push_back(tmp);
    }
    fYieldA.push_back(vec);
    
    CheckFormat();
    vec.clear();
    for (int i=GetAmin(iN); i<=GetAmax(iN); i++) {
      tmp = ReadFloat();
      vec.push_back(tmp);
    }
    fYieldAErr.push_back(vec);
    
    CheckFormat();
    
    // Isotope Yield as a function of Atomic Number
    vec.clear();
    for (int i=GetZmin(iN); i<=GetZmax(iN); i++) {
      tmp = ReadFloat();
      vec.push_back(tmp);
    }  
    fYieldZ.push_back(vec);

    CheckFormat();
    vec.clear();
    for (int i=GetZmin(iN); i<=GetZmax(iN); i++) {
      tmp = ReadFloat();
      vec.push_back(tmp);
    }
    fYieldZErr.push_back(vec);
    std::cerr << "done" << std::endl;
    
    CheckFormat();

    std::cerr << "residual nuclei distribution " << std::flush;
    
    // Residual nuclei distribution
    vec2.clear();
    for (int i=0; i<fIMRHGH[iN]; i++) {
      vec.clear();
      for (int j=0; j<fIZRHGH[iN]; j++) {
	tmp = ReadFloat();//*100.0;
	vec.push_back(tmp);
      }
      //std::cout << std::endl << std::endl;
      vec2.push_back(vec);
    }
    fRNERR.push_back(vec2);

    CheckFormat();

    std::cerr << " done" << std::endl;
    
  }

  delete [] mychar; mychar = 0;

  return true;
}
float UsrSuw::GetRNDATA(int i, int Z, int A) const
{
  /*
    Return residual nuclei production with specified Z and A
   */

  if (Z>GetZmax(i)) {
    std::cerr << "WARNING by GetRNDATA: Z = " << Z << " > Zmax = " << GetZmax(i) << std::endl;
    return 0.0f;
  }
  if (A>GetAmax(i)) {
    std::cerr << "WARNING by GetRNDATA: A = " << A << " > Amax" << GetAmax(i) << std::endl;
    return 0.0f;
  }
  Z = Z-1;
  A = A-1-fK[i]-2*(Z+1);
  //std::cout << "arguments for GetRNDATA: " << Z << " " << A << std::endl;
  return fRNDATA[i][A][Z];
}

float UsrSuw::GetRNERR(int i, int Z, int A) const
{
  /*
    Return residual nuclei production with specified Z and A
   */

  if (Z>GetZmax(i)) {
    std::cerr << "WARNING by GetRNDATA: Z = " << Z << " > Zmax = " << GetZmax(i) << std::endl;
    return 0.0f;
  }
  if (A>GetAmax(i)) {
    std::cerr << "WARNING by GetRNDATA: A = " << A << " > Amax" << GetAmax(i) << std::endl;
    return 0.0f;
  }
  Z = Z-1;
  A = A-1-fK[i]-2*(Z+1);
  //std::cout << "arguments for GetRNDATA: " << Z << " " << A << std::endl;
  return fRNERR[i][A][Z];
}

void UsrSuw::Print() const
{
  /*
    Print the info about the current RESNUCLEi card
  */
  
  for (int iN=0; iN<fN; iN++) {
    std::cout << std::endl;
    std::cout << "Res. nuclei n. " << fNRN[iN] << " \"" << fTIURSN[iN]; 
    if (abs(fITURSN[iN])<1) // see page 211
      std::cout	<< "\", 'high' energy products, region n. " << fNRURSN[iN] << std::endl;
    else if (abs(fITURSN[iN])<2) 
      std::cout << "\", 'low' energy products, region n. " << fNRURSN[iN] << std::endl;
    else 
      std::cout << "\", all products, region n. " << fNRURSN[iN] << std::endl;
    std::cout << "\tdetector volume: " << fVURSNC[iN] << " cm**3" << std::endl;
    std::cout << "\tMax. Z: " << fIZRHGH[iN];
    std::cout << ", Max. N-Z: " << fIMRHGH[iN] + fK[iN];
    std::cout << ", Min. N-Z: " << fK[iN]+1 << std::endl;

    std::cout << std::endl;
    std::cout << "Tot. response (n/cmc/pr) " << fTotalResp[iN] << " +/- " << 100*fTotalRespErr[iN] << " %" << std::endl;
    std::cout << "( --> Nuclei/pr          " << fTotalResp[iN] << " +/- " << 100*fTotalRespErr[iN] << " % )" << std::endl;
    

    std::cout << std::endl;
    std::cout << "**** Isotope Yield as a function of Mass Number ****" << std::endl;
    std::cout << "**** (nuclei / cmc / pr)                        ****" << std::endl;
    std::cout << std::endl << "A_min: " << GetAmin(iN) << " - A_max: " << GetAmax(iN) << std::endl << std::endl;
    for (int i=GetAmax(iN)-1; i>=GetAmin(iN)-1; --i) { 
      if (fYieldA[iN][i]>0)
	std::cout << "A:\t" << i+1 <<"\t"<< fYieldA[iN][i] << " +/- " << 100*fYieldAErr[iN][i] << " %" << std::endl;
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "**** Isotope Yield as a function of Atomic Number ****" << std::endl;
    std::cout << "****   (nuclei / cmc / pr)                        ****" << std::endl;
    std::cout << std::endl << "Z_min: " << GetZmin(iN)  << " - Z_max: " << GetZmax(iN) << std::endl << std::endl;
    for (int i=GetZmax(iN)-1; i>=GetZmin(iN)-1; --i) {
      //std::cerr << "here" << std::endl;
      if (fYieldZ[iN][i]>0)	std::cout << "Z:\t" << i+1 <<"\t"<< fYieldZ[iN][i] << " +/- " << 100*fYieldZErr[iN][i] << " %" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "**** Residual nuclei distribution  ****" << std::endl
	      << "****    (nuclei / cmc / pr)        ****" << std::endl;
    std::cout << std::endl;
    
    float val, err;
    //    int iCount=0;
    for (int i=0; i<fIZRHGH[iN]; i++)
      for (int j=0; j<fIMRHGH[iN]; j++) {
	val = GetRNDATA()[iN][j][i];
	err = GetRNERR()[iN][j][i];
	//      cout << "\t" << i+1 << " " << j+1 << " " << GetK() << endl;
	if (val>0)
	  std::cout << "\t" << GetA(iN, i,j) << "\t" << i+1 << "\t" << val << "\t" << 100*err << std::endl;
	//	iCount++;
      }
  }
}

std::string UsrSuw::GetBinTitle() const
{
  std::ostringstream tmp;

  if (abs(GetITURSN())<1) { // see page 211
    tmp << "Res. nuclei production rate: 'high' energy products in the region " << GetNRURSN() << std::flush;
  } else if (abs(GetITURSN())<2) {
    tmp << "Res. nuclei production rate: 'low' energy products in the region " << GetNRURSN() << std::flush;
  } else {
    tmp << "Res. nuclei production rate: all products in the region " << GetNRURSN() << std::flush;
  }  
  tmp << ", detector volume: " << GetVURSNC() << " cm^{3}" << std::flush;
  tmp << " ( Z_{max} = " << GetIZRHGH();
  tmp << ", (N-Z)_{max} = " << GetIMRHGH() + GetK();
  tmp << ", (N-Z)_{min} = " << GetK()+1 << " )" << std::flush;

  return tmp.str();
}
