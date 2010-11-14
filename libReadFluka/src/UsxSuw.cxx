#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib> // for exit()
#include <algorithm>
#include <numeric>
//#include <cmath>

#include "UsxSuw.h" ///////                                !!! NOT YET FINISHED !!!

using namespace ReadFluka;

UsxSuw::UsxSuw(const char *fname) : Base(fname)
{
  Reset();

  fIsReadHeader = false;
  fWEIPRI = 0;
  fNCASE  = 0;

  ReadHeader();
}

UsxSuw::~UsxSuw()
{
  Reset();
}

void UsxSuw::Reset()
{
  // fNX = 0;
  fTITUSX.clear();
  fITUSBX.clear();
  fIDUSBX.clear();
  fNR1USX.clear();
  fNR2USX.clear();
  fAUSBDX.clear();
  fLWUSBX.clear();
  fLFUSBX.clear();
  fLLNUSX.clear();
  fEBXLOW.clear();
  fEBXHGH.clear();
  fNEBXBN.clear();
  fDEBXBN.clear();;
  fABXLOW.clear();
  fABXHGH.clear();;
  fNABXBN.clear();
  fDABXBN.clear();

  fIGMUSX.clear();
  fENGMAX.clear();
  fEPGMAX.clear();

  fGBSTOR.clear();
  fGDSTOR.clear();
  
  fTOTTOT.clear();
  fTOTERR.clear();

  fTotResp.clear();
  fTotRespErr.clear();

  fFlux.clear();
  fFluxErr.clear();
  fCumulFlux.clear();
  fCumulFluxErr.clear();
}

void UsxSuw::ReadHeader()
{
  if (fIsReadHeader == true) return;

  fIsReadHeader = true;
  ReadRunTitle();
  ReadRunTime();
  fWCTOT = ReadFloat();
  fNCTOT  = ReadInt();
  fMCTOT = ReadInt();
  fMBATCH = ReadInt();

  CheckFormat();
}

bool UsxSuw::Read()
{
  if (fin->eof()) return false; // exit the while-loop if reached the end of the file
  Reset();

  int NX;
  int KLAST = 0;
  int K0, K1, K2;
  char *mychar = new char[11];
  std::vector<float> vtmp;
  double totresp, dtmp;

  //std::cerr << "Read begin" << std::endl;
  int record=0;
  for (;;) {
    totresp = 0.0;
    NX = ReadInt();  //std::cout << "NX: " << NX << std::endl;
    
    // line 526 in usxsuw.f
    
    fin->read(mychar, 10); mychar[10]='\0';
    fTITUSX.push_back(Trimmed(std::string(mychar)));
    fITUSBX.push_back(ReadInt()); // type of the binning
    fIDUSBX.push_back(ReadInt()); // distr to be scored
    fNR1USX.push_back(ReadInt()); // 1st region
    fNR2USX.push_back(ReadInt()); // 2nd region
    fAUSBDX.push_back(ReadFloat());    // area
    fLWUSBX.push_back(ReadBool());   // one/two ways
    fLFUSBX.push_back(ReadBool());   // current/fluence
    fLLNUSX.push_back(ReadBool());   // low energy neutrons
    
    // energy binning:
    fEBXLOW.push_back(ReadFloat()); fEBXHGH.push_back(ReadFloat()); fNEBXBN.push_back(ReadInt());fDEBXBN.push_back(ReadFloat());
    // angular binning:
    fABXLOW.push_back(ReadFloat()); fABXHGH.push_back(ReadFloat()); fNABXBN.push_back(ReadInt()); fDABXBN.push_back(ReadFloat());

    CheckFormat();

    if (fLLNUSX[record]) {  // low energy neutrons
      // std::cout << " read low energy neutrons" << std::endl;

      fIGMUSX.push_back(ReadInt()); //std::cout << "igmusx: " << fIGMUSX << std::endl;
      if (fIGMUSX[record] != 260) {
	std::cerr << std::endl << "UsxSuw::Read: strange, but number of neutron groups is " << fIGMUSX[record] << " but not 260" << std::endl;
      }
      
      // line 534 in usxsuw.f
      vtmp.clear();
      for (int j=0; j<fIGMUSX[record]+1; j++) { // loop over all low energy groups (260) plus one bin
	dtmp = ReadFloat();
	vtmp.push_back(dtmp);
      }
      totresp +=  std::accumulate(vtmp.begin(), vtmp.end(), 0.0f);
      fENGMAX.push_back(vtmp);

      CheckFormat();
    } else fIGMUSX.push_back(0);

    K0 = KLAST + 1;
    K1 = fNEBXBN[record] * fNABXBN[record] + K0 - 1; // total number of bins + K0 - 1
    K2 = K1 + fIGMUSX[record]*fNABXBN[record];
    KLAST = K2;

    vtmp.clear();
    std::cerr << "data1: ";
    for (int j=K0; j<=K2; j++) {
      dtmp = ReadFloat(); std::cout << j-K0 << " " << dtmp << "\t";
      vtmp.push_back(dtmp);
    } 
    std::cout << std::endl;
    totresp += std::accumulate(vtmp.begin(), vtmp.end(), 0.0f);
    fGDSTOR.push_back(vtmp); // line 540 in usxsuw.f

    //std::cout << "totresp: " << totresp << std::endl;
    fTOTTOT.push_back(totresp); // !!! the same numbers can be read from file - see below fTotResp

    CheckFormat();

    //    Print(record);
    
    if (ReadStatFlag(false) == true) {
      break;
    }
    record++;
  }
  
  const int Nrecords = record;
  for (record=0; record<=Nrecords; record++) {
    NX = record;
    //  MX = 0; //!!! check this !!! - total number of detectors?
    CheckFormat();
    //    fTOTTOT.push_back(ReadFloat());
    //    float t1 = ReadFloat();
    //float t2 = ReadFloat();
    fTotResp.push_back(ReadFloat());
    fTotRespErr.push_back(ReadFloat());
    //std::cout << "Total responce read from file: " << t1 << " +- " << t2 << std::endl;
    //fTOTERR.push_back(t2);
    CheckFormat();
    
    int nebxbn = ReadInt(); // line 654
    int igmusx = ReadInt();

    std::cout << "ebxlow: old=" << fEBXLOW[record] << std::endl;
    PrintFloat(1);

    std::cout << "epgmax (energy boundaries):" << std::endl;
    for (int ii=0; ii<nebxbn+igmusx+1; ii++) {
      std::cerr << ii << " " << ReadFloat() << std::endl;
    }
    
    CheckFormat();

    vtmp.clear();
    for (unsigned int ii=0; ii<GetNbinsE(record); ii++) { // flux
      vtmp.push_back(ReadFloat());
    }
    // FLUKA writes the array in a reverse way, so we reverse it again:
    //std::reverse(vtmp.begin(), vtmp.end());
    fFlux.push_back(vtmp); 

    CheckFormat();

    vtmp.clear();
    for (unsigned int ii=0; ii<GetNbinsE(record); ii++) { // flux error
      vtmp.push_back(ReadFloat());
    }
    //std::reverse(vtmp.begin(), vtmp.end());
    fFluxErr.push_back(vtmp);


    CheckFormat();

    vtmp.clear();
    for (unsigned int ii=0; ii<GetNbinsE(record); ii++) { // cumulative flux
      vtmp.push_back(ReadFloat());
    }
    fCumulFlux.push_back(vtmp);

    CheckFormat();

    vtmp.clear();
    for (unsigned int ii=0; ii<GetNbinsE(record); ii++) { // cumulative flux error
      vtmp.push_back(ReadFloat());
    }
    fCumulFluxErr.push_back(vtmp);

    CheckFormat();

    std::cout << "angles: " << std::endl;
    if (fNABXBN[record]>1) { // more than one angular interval
      PrintFloat(4);
      CheckFormat();
    }
  }


  //  for (int j=0; j<10; j++) std::cout << ReadFloat() << std::endl;


  /*  // INTERV - total number of enery intervals
  // (interval above the limit of n-groups+ intervals below)
  int INTERV = fNEBXBN + fIGMUSX;
  fNScored = INTERV*fNABXBN;
  fScored = new float[fNScored];

  // read the scoring results as a 1-dimentional array
  for (int i=0; i<fNScored; i++) fScored[i] = ReadFloat();

  CheckFormat(); // std::cerr << "+++ OK in the end of UsxSuw::Read +++" << std::endl;
  */

  delete [] mychar; mychar = 0; // is it ok? !!!

  Print(0);

  return true;
}

/*float UsxSuw::GetScored(unsigned int ie, unsigned int ia) const
{
  
    Return the scored value in the ie-ja-cell.
    ie - energy bin
    ia - angular bin
    The cell indises must be >= 1 (FLUKA-Fortran style)
  

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
*/

std::string UsxSuw::GetBinTitle(int i) const
{
  std::ostringstream tmp;
  if (IsFluence(i)) tmp << "Fluence of ";
  else tmp << "Current of ";
  
  tmp << Code2Name(GetID(i)) << " (reg. " << GetRegFrom(i);

  if (IsOneWay(i)) tmp << " -> ";
  else tmp << " <-> ";
  tmp << GetRegTo(i);
  tmp << ", ";

  tmp << "S = ";
  tmp << GetArea(i);
  tmp << " cm^{2})";

  return tmp.str();
}

const char* UsxSuw::GetXtitle(int i) const
{
  if (GetType(i)>0) return "E_{kin} [GeV]";
  else return "lg(E_{kin}/1 GeV)";
}

const char* UsxSuw::GetYtitle(int i) const
{
  if (abs(GetType(i))<=1) return "#Omega [sr]";
  else return "lg(#Omega/1 sr)";
}

/*bool UsxSuw::IsLogA() const
{
  int i= int(std::abs(float(GetType())));
  std::cout << "nint: " << i << std::endl;
  if (i<=1) std::cout << "nint says: logA" << std::endl;

  return (i<=1);
  }*/

void UsxSuw::Print(int i) const
{
  std::cerr << "UsxSuw::Print" << std::endl;
  std::cout << std::endl;
  std::cout << "Detector n: " << i+1 << " (" << i+1 << ") " << GetBinName(i) <<  std::endl;
  std::cout << "\t(Area: " << GetArea(i) << " cmq," << std::endl;
  std::cout << "\t distr. scored: " << GetID(i) << "," << std::endl;
  std::cout << "\t from reg. " << GetRegFrom(i) << " to " << GetRegTo(i) << "," << std::endl;
  if (IsReadNeutrons(i)) {
    std::cout << "\t low energy neutrons scored from group 1" << " to group " << GetMaxNeutronGroup(i) << std::endl;
  }
  if (IsOneWay(i) == true) 
    std::cout << "\t one way scoring," << std::endl; 
  else
    std::cout << "\t this is a two ways estimator" << std::endl;
  
  if (IsFluence(i) == true) 
    std::cout << "\t fluence scoring)" << std::endl;
  else
    std::cout << "\t current scoring)" << std::endl;

  std::cout << std::endl;
  std::cout << "\tTot. resp. (Part/cmq/pr) " << AsFortran( fTotResp[i] , 6 )
	    << " +/- " << AsFortran( 100.0*fTotRespErr[i], 5) << " %" << std::endl;
  std::cout << "\t( -->      (Part/pr)     " <<AsFortran( fTotResp[i]/fAUSBDX[i], 6 )
	    << " +/- " << AsFortran( 100.0*fTotRespErr[i]/fAUSBDX[i], 5) << " % )" << std::endl;

  std::cout << std::endl;
  std::cout << "\t**** Different. Fluxes as a function of energy ****" << std::endl;
  std::cout << "\t****      (integrated over solid angle)        ****" << std::endl;
  
  std::cout << "\t Energy boundaries (GeV):" << std::endl;

  std::vector<float> elowedges = GetELowEdge(i);
  std::cout << "\t  ";
  for (unsigned int ii=elowedges.size()-1; ii>0; ii--) { // do not print the lowest boundary here (ii>0) instead of (ii>=0)
    std::cout << AsFortran(elowedges[ii], 6) << " ";
  }
  std::cout << std::endl;
  std::cout << "\t Lowest boundary (GeV): " << AsFortran(elowedges[0], 6) << std::endl;

  std::cout << std::endl;
  std::cout << "\t Flux (Part/GeV/cmq/pr):" << std::endl;
  std::cout << "\t  ";

  for (unsigned int ii=0; ii<fFlux[i].size(); ii++) {
    /*
      This line is actually the same as the uncommented one (fFlux):
      std::cout << AsFortran(fGDSTOR[i][ii]*(fABXHGH[i]-fABXLOW[i]), 6) << " ";
    */
    std::cout << AsFortran( fFlux[i][ii], 6 ) << " +/- " << AsFortran( 100.0*fFluxErr[i][ii], 5 ) << " %\t";
  }
  std::cout << std::endl << std::endl;

  std::cout << "\t**** Cumulative Fluxes as a function of energy ****" << std::endl
	    << "\t****      (integrated over solid angle)        ****" << std::endl;

  std::cout << "\t Energy boundaries (GeV):" << std::endl;

  //  std::vector<float> elowedges = GetELowEdge(i);
  std::cout << "\t  ";
  for (unsigned int ii=elowedges.size()-1; ii>0; ii--) { // do not print the lowest boundary here (ii>0) instead of (ii>=0)
    std::cout << AsFortran(elowedges[ii], 6) << " ";
  }
  std::cout << std::endl;
  std::cout << "\t Lowest boundary (GeV): " << AsFortran(elowedges[0], 6) << std::endl;

  std::cout << "\t Cumul. Flux (Part/cmq/pr):" << std::endl;
  std::cout << "\t  ";
  for (unsigned int ii=0; ii<elowedges.size()-1; ii++) {
    std:: cout << AsFortran(fCumulFlux[i][ii], 6) << " +/- "
	       << AsFortran(100*fCumulFluxErr[i][ii], 5) << " %\t";
  }
  std::cout << std::endl;

  if (fNABXBN[i]>1) {
    std::cout << std::endl;
    std::cout << "\t**** Double diff. Fluxes as a function of energy ****" << std::endl;
    std::vector<float> alowedges = GetALowEdge(i, kRAD);
    
    std::cout << "\t Solid angle minimum value (sr): " << AsFortran(alowedges[0], 6) << std::endl;
    std::cout << "\t Solid angle upper boundaries (sr):" << std::endl;
    std::cout << "\t  ";
    for (unsigned int ii=1; ii<alowedges.size(); ii++)
      std::cout << AsFortran(alowedges[ii], 6) << " ";
    std::cout << std::endl;

    std::vector<float> alowedgesdeg = GetALowEdge(i, kDEG);
    std::cout << "\t Angular minimum value (deg.): " << AsFortran( alowedgesdeg[0], 6) << std::endl;
    std::cout << "\t Angular upper boundaries (deg.):" << std::endl;
    std::cout << "\t  ";
    for (unsigned int ii=1; ii<alowedgesdeg.size(); ii++)
      std::cout << AsFortran(alowedgesdeg[ii], 6) << " ";
    std::cout << std::endl;

    for (unsigned int ie=elowedges.size()-1; ie>0; ie--) {
      std::cout << "\t Energy interval (GeV): "
		<< AsFortran(elowedges[ie], 6) << " "
		<< AsFortran(elowedges[ie-1], 6) << std::endl;
      for (int icase=0; icase<2; icase++) {
	if (icase==0) {
	  std::cout << "\t  Flux (Part/sr/GeV/cmq/pr):" << std::endl << "\t   ";
	  for (unsigned int ia=alowedges.size()-1; ia>0; ia--) {
	    std::cout << ie-1 << "," << ia-1 << ": " << GetData(i, ie-1, ia-1) << "\t";
	  }
	} else if (icase==1) {
	  std::cout << "\t  Flux (Part/deg/GeV/cmq/pr):" << std::endl << "\t   ";
	  for (unsigned int ia=alowedges.size()-1; ia>0; ia--) {
	    std::cout << "here" << " ";
	  }
	}
	std::cout << std::endl;
      }
    }
  }


  //  for (unsigned int ii=0; ii<GetNbinsA(i); ii++)
  //  std::cout << "awidth: " << GetAwidthRAD(i, ii)  << " rad"<< std::endl;
  
  //  for (int j=1; j<fNEBXBN(i); j++)
  //  std::cout <<  << std::endl;

  /* std::cout.precision(4);

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
  int n = GetNScored(i);
  n = GetNbinsE(i)*GetNbinsA(i);
*/
  /*
    std::cout << std::endl << "\t";
    for (unsigned int i=1; i<=GetNbinsA(); i++) {
    for (unsigned int j=1; j<=GetNbinsE(); j++)
    std::cout << GetScored(j, i) << " ";
    std::cout << std::endl << "\t";
    }
  */
  /*std::cout << std::endl;
  
  // loop on angles 
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
      int ig = fIGMUSX;// 260 - maximum low energy group to be scored
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
    Warning("UsxSuw::Read: Loop on energies above the n-group limit has not yet been checked !!!")
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
  */
  
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

/*double *UsxSuw::XbinsE_len(double *values, double *limits) const
{
  
    Fills two arrays:
    values - an array of low-energy neutron data. Dimention is GetMaxNeutronGroup()+1 = 261
    limits - an array of low-edges of each low energy neutron energy bin.  This is an array of size GetMaxNeutronGroup()+1+1 = 262
    Note that it is a user's responsability to delete these arrays.
   

  if (!IsReadNeutrons()) return 0; // nothing to do if there are no neutron data

  short nlims = fIGMUSX+2; // 261+1=262 std::cout << "nlims: " << nlims << std::endl;
  double *lims = new double[nlims];
  
  // The rest of the code is very similar to UsxSuw::Print and rdbdx.f

  unsigned int ia = 1; // ??? set the 1st angular interval and assume it's the same for all the others - is it correct ??? 
  unsigned int jg1 = ia*(fNEBXBN+fIGMUSX);
  unsigned int jg2 = ia*(fNEBXBN+fIGMUSX)-fIGMUSX+1;
  double en1 = fENGMAX[fIGMUSX];
  double en2 = 0.0;

  std::cout << "here" << std::endl;
  for (unsigned int jg = jg1; jg>=jg2-1; jg--) { // here we say jg2-1 but not jg2 as in UsxSuw::Print and rdbdx.f since otherwise we lose the last interval
    en1 = fENGMAX[jg-1];
    std::cout << jg << " " << nlims-jg << "\t" << en1 << std::endl;
    lims[nlims-jg] = en1;
    //en1 = en2;
  }
  
   // find lower limit of first bin above or straddling the
  // n-group limit. Nhigh: counts the high energy bins
  unsigned int nhigh = 0;
  // for the time being, set energy boundary at n-group limit
  float elimit = en1;
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

  //     first bin above or straddling the n-group limit
  if (fITUSBX>0)
    en2 = en1 + fDEBXBN;
  else
    en2 = en1 * fDEBXBN;
  float diff = fScored[ia*(fNEBXBN-nhigh+1)-1];
  std::cout << en2 << "\t\t";
  std::cout.precision(7);
  std::cout << diff << std::endl;
  en1 = en2;
  
  return lims; 
}
*/


std::vector<float> UsxSuw::GetALowEdge(unsigned int i, EUnit unit) const
{
  /*
    Return low edges for angular bins. The array size is bin+1 (since we want to know the high edge for the last bin)
    ---- It seems the returned array is the same as OMGMAX in usxsuw.f ---
    If 'unit' is kRAD the angles (solid) are in radians, 
    if unit==kDEG - in degrees (I still don't understand this strange conversion - see below.)
   */

  std::vector<float> vec;
  float val;

  //  std::cout << "angular intervals: " << fNABXBN[i] << " between " << fABXLOW[i] << " and " << fABXHGH[i] << " rad" << std::endl;
  for (unsigned int ia=1; ia<=fNABXBN[i]+1; ia++) {
    if (abs(fITUSBX[i])<=1) { // linear in angle
      val = fABXLOW[i] + (ia-1)*fDABXBN[i];
      vec.push_back(val);
    } else { // logarithmic in angle
      if (ia == 1) {// fist bin
	vec.push_back(0.0f); // see note 2 for USRBDX on page 237
	val = fABXLOW[i]; //std::cout << "first bin log val: " << val << std::endl;
        vec.push_back(val);
      } else {
	if (ia!=fNABXBN[i]+1) {
	  val = fABXLOW[i]*pow(fDABXBN[i], ia-1); //std::cout << "log val: " << val << std::endl;
	  vec.push_back(val);
	}
      }
    }
  }
  if (unit == kDEG) {
    double val;
    for (unsigned int i=0; i<vec.size(); i++) {
      val = vec[i];
      val = std::acos(std::max(1.0-val/M_PI/2.0, -1.0)) * 180.0/M_PI;
      vec[i] = (float)val;
    }
  }

  return vec;
}

float UsxSuw::GetAwidthRAD(unsigned int i, unsigned int bin) const
{
  /*
    Return the width of 'bin'th bin in the 'i'th distribution. The result is always in Radians even if log binning has been requested.
   */

  std::vector<float> edges = GetALowEdge(i);
  if (bin>edges.size()) {
    std::cerr << "WARNING: UsxSuw::GetAwidthRAD: bin = " << bin << " > " << edges.size() << std::endl;
    return 0;
  }
  return edges[bin+1]-edges[bin];
}

std::vector<float> UsxSuw::GetELowEdge(unsigned int i) const
{
  /*
    Return low edges for energy bins. ???The array size is bin+1 (since we want to know the high edge for the last bin)???
   */

  //  std::cerr << "UsxSuw::ELowEdge: NOT YET IMPLEMENTED" << std::endl;

  std::vector<float> vec;
  float val;
  unsigned int nint = fNEBXBN[i]; // number of energy intervals without low energy neutrons

  //  std::cout << "energy intervals: " << nint << " between " << fEBXLOW[i] << " and " << fEBXHGH[i] << " GeV" << std::endl;
  for (unsigned int ie=1; ie<=nint+1; ie++) {
    if (fITUSBX[i]>0) { // linear in energy
      val = fEBXLOW[i] + (ie-1)*fDEBXBN[i];
      vec.push_back(val);
    } else { // logarithmic in energy
      val = fEBXLOW[i]*pow(fDEBXBN[i], ie-1); //std::cout << "log val: " << val << std::endl;
      vec.push_back(val);
    }
  }
  //std::cout << "esize: " << vec.size() << std::endl;
  //for (unsigned int ie=0; ie<vec.size(); ie++) 
  //std::cout << "ebin" << ie << " "  << vec[ie] << " " << std::endl;

  return vec;
}


float UsxSuw::GetTotalResponce(unsigned int i) const
{
  /*
    --- The same as fTotResp[i] ---
    Return the total responce of the i-th detector [Part/GeV/cmq/primary]
    Note that in *_sum.lis produced by usxsuw it's somewhat written [Part/cmq/primary] - without GeV. WHY???
   */
  
  // why divide by fNEBXBN[i]
  return fTOTTOT[i]*(fEBXHGH[i]-fEBXLOW[i])*(fABXHGH[i]-fABXLOW[i])/fNEBXBN[i];
}

float UsxSuw::GetData(unsigned int i, unsigned int ie, unsigned int ia) const
{
  /*
    Return data (above low energy neutrons) in energy bin 'ie' and angular bin 'ia'
   */

  return fGDSTOR[i][ie+ia];
}
