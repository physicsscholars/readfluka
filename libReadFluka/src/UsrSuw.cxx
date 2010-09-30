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
    fNRN = ReadInt(); //std::cout << "NRN: " << fNRN << std::endl;
    
    fin->read(mychar, 10); mychar[10] = '\0'; // !!! is it necessary \0?
    fTIURSN = Trimmed(std::string(mychar)); std::cout << "name: " << fTIURSN << std::endl;
    
    fITURSN = ReadInt();
    fNRURSN = ReadInt();
    fVURSNC = ReadFloat();
    fIMRHGH = ReadInt(); 
    fIZRHGH = ReadInt();//std::cout << "dimentions: " << fIMRHGH << " " << fIZRHGH << std::endl;
    fK = ReadInt();
    
    //std::cout << "checkpoint1: " << fin->tellg() << std::endl;
    CheckFormat();
    //std::cout << "checkpoint2: " << fin->tellg() << std::endl;
    vec2.clear();
    for (int i=0; i<fIMRHGH; i++) {
      vec.clear();
      for (int j=0; j<fIZRHGH; j++) {
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
    for (int i=GetAmin(); i<=GetAmax(); i++) { 
      tmp = ReadFloat();
      vec.push_back(tmp);
    }
    fYieldA.push_back(vec);
    
    CheckFormat();
    vec.clear();
    for (int i=GetAmin(); i<=GetAmax(); i++) {
      tmp = ReadFloat();
      vec.push_back(tmp);
    }
    fYieldAErr.push_back(vec);
    
    CheckFormat();
    
    // Isotope Yield as a function of Atomic Number
    vec.clear();
    for (int i=GetZmin(); i<=GetZmax(); i++) {
      tmp = ReadFloat();
      vec.push_back(tmp);
    }  
    fYieldZ.push_back(vec);

    CheckFormat();
    vec.clear();
    for (int i=GetZmin(); i<=GetZmax(); i++) {
      tmp = ReadFloat();
      vec.push_back(tmp);
    }
    fYieldZ.push_back(vec);
    std::cerr << "done" << std::endl;
    
    CheckFormat();

    std::cerr << "residual nuclei distribution " << std::flush;
    
    // Residual nuclei distribution
    vec2.clear();
    for (int i=0; i<fIMRHGH; i++) {
      vec.clear();
      for (int j=0; j<fIZRHGH; j++) {
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

  if (Z>GetZmax()) {
    std::cerr << "WARNING by GetRNDATA: Z = " << Z << " > Zmax = " << GetZmax() << std::endl;
    return 0.0f;
  }
  if (A>GetAmax()) {
    std::cerr << "WARNING by GetRNDATA: A = " << A << " > Amax" << GetAmax() << std::endl;
    return 0.0f;
  }
  Z = Z-1;
  A = A-1-fK-2*(Z+1);
  //std::cout << "arguments for GetRNDATA: " << Z << " " << A << std::endl;
  return fRNDATA[i][A][Z];
}

float UsrSuw::GetRNERR(int i, int Z, int A) const
{
  /*
    Return residual nuclei production with specified Z and A
   */

  if (Z>GetZmax()) {
    std::cerr << "WARNING by GetRNDATA: Z = " << Z << " > Zmax = " << GetZmax() << std::endl;
    return 0.0f;
  }
  if (A>GetAmax()) {
    std::cerr << "WARNING by GetRNDATA: A = " << A << " > Amax" << GetAmax() << std::endl;
    return 0.0f;
  }
  Z = Z-1;
  A = A-1-fK-2*(Z+1);
  //std::cout << "arguments for GetRNDATA: " << Z << " " << A << std::endl;
  return fRNERR[i][A][Z];
}

void UsrSuw::Print() const
{
  /*
    Print the info about the current RESNUCLEi card
   */

 if (abs(GetITURSN())<1) { // see page 211
      std::cout << "Res. nuclei n. " << GetNRN() << " \"" << GetTIURSN() 
	   << "\", 'high' energy products, region n. " << GetNRURSN() << std::endl;
      std::cout << "\tdetector volume: " << GetVURSNC() << " cm**3" << std::endl;
      std::cout << "\tMax. Z: " << GetIZRHGH();
      std::cout << ", Max. N-Z: " << GetIMRHGH() + GetK();
      std::cout << ", Min. N-Z: " << GetK()+1 << std::endl;
    } else if (abs(GetITURSN())<2) {
      std::cout << "Res. nuclei n. " << GetNRN() << " \"" << GetTIURSN() 
	   << "\", 'low' energy products, region n. " << GetNRURSN() << std::endl;
      std::cout << "\tdetector volume: " << GetVURSNC() << " cm**3" << std::endl;
      std::cout << "\tMax. Z: " << GetIZRHGH();
      std::cout << ", Max. N-Z: " << GetIMRHGH() + GetK();
      std::cout << ", Min. N-Z: " << GetK()+1 << std::endl;
    } else {
      std::cout << "Res. nuclei n. " << GetNRN() << " \"" << GetTIURSN() 
	   << "\", all products, region n. " << GetNRURSN() << std::endl;
      std::cout << "\tdetector volume: " << GetVURSNC() << " cm**3" << std::endl;
      std::cout << "\tMax. Z: " << GetIZRHGH();
      std::cout << ", Max. N-Z: " << GetIMRHGH() + GetK();
      std::cout << ", Min. N-Z: " << GetK()+1 << std::endl;
    }  
  
 for (int iN=0; iN<fN; iN++) {
  std::cout << std::endl;
  std::cout << "**** Isotope Yield as a function of Mass Number ****" << std::endl;
  std::cout << "**** (nuclei / cmc / pr)                        ****" << std::endl;
  std::cout << std::endl << "A_min: " << GetAmin() << " - A_max: " << GetAmax() << std::endl << std::endl;
  for (int i=GetAmax()-1; i>=GetAmin()-1; --i) { 
    if (fYieldA[iN][i]>0)
      std::cout << "A:\t" << i+1 <<"\t"<< fYieldA[iN][i] << " +/- " << 100*fYieldAErr[iN][i] << " %" << std::endl;
  }
  std::cout << std::endl << std::endl;

  std::cout << "**** Isotope Yield as a function of Atomic Number ****" << std::endl;
  std::cout << "****   (nuclei / cmc / pr)                        ****" << std::endl;
  std::cout << std::endl << "Z_min: " << GetZmin()  << " - Z_max: " << GetZmax() << std::endl << std::endl;
  for (int i=GetZmax()-1; i>=GetZmin()-1; --i) {
    if (fYieldZ[iN][i]>0)
      std::cout << "Z:\t" << i+1 <<"\t"<< fYieldZ[iN][i] << " +/- " << 100*fYieldZErr[iN][i] << " %" << std::endl;
  }

  std::cout << std::endl;
  std::cout << "**** Residual nuclei distribution  ****" << std::endl
	    << "****    (nuclei / cmc / pr)        ****" << std::endl;
  std::cout << std::endl;

  float val, err;
  int iCount=0;
  for (int i=0; i<GetIZRHGH(); i++)
    for (int j=0; j<GetIMRHGH(); j++) {
      //   val = GetRNDATA()[iN][j][i];
      err = GetRNERR()[iN][j][i];
      //      cout << "\t" << i+1 << " " << j+1 << " " << GetK() << endl;
      if (val>0)
	std::cout << "\t" << GetA(i,j) << "\t" << i+1 << "\t" << val << "\t" << 100*err << std::endl;
      iCount++;
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
