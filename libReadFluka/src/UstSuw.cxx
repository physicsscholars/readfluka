#include "UstSuw.h"
#include <algorithm>

using namespace ReadFluka;

UstSuw::UstSuw(const char *fname) : Base(fname)
{
  Reset();

  fIsReadHeader = false;
  fWEIPRI = 0;
  fNCASE  = 0;

  ReadHeader();
}

UstSuw::~UstSuw()
{
  Reset();
}

void UstSuw::Reset()
{
  fMTC = 0;
  fTITUTC.clear();
  fITUSTC.clear();
  fIDUSTC.clear();
  fNRUSTC.clear();
  fVUSRTC.clear();
  fLLNUTC.clear();
  fETCLOW.clear(); fETCHGH.clear(); fNETCBN.clear(); fDETCBN.clear();
  // fATCLOW.clear(); fATCHGH.clear(); fNATCBN.clear(); fDATCBN.clear();

  fIGMUTC.clear();
  fENGMAX.clear();
  fNRecords = 0;
  fTotResp.clear();
  fTotRespErr.clear();
  
  fEPGMAX.clear();
  fFlux.clear();
  fFluxErr.clear();
  fFluxLEN.clear();
  fFluxLENErr.clear();

  fCumulFlux.clear();
  fCumulFluxErr.clear();
  fCumulFluxLEN.clear();
  fCumulFluxLENErr.clear();
}

bool UstSuw::ReadHeader()
{
  if (fIsReadHeader == true) return false;
  fIsReadHeader = true;

  ReadRunTitle();
  ReadRunTime();

  fWEIPRI = ReadFloat();
  fNCASE = ReadInt();
  fMCASE = ReadInt();
  fNBATCH = ReadInt();

  CheckFormat();

  return true;
}

bool UstSuw::Read()
{
  if (fin->eof()) return false;
  Reset();

  unsigned int record = 0;
  char *mychar = new char[11];
  std::vector<float> vtmp;
  double dtmp;

  for(;;) {
    fMTC = ReadInt();
    fin->read(mychar, 10); mychar[10] = '\0';
    fTITUTC.push_back(Trimmed(std::string(mychar)));
    fITUSTC.push_back(ReadInt());
    fIDUSTC.push_back(ReadInt());
    fNRUSTC.push_back(ReadInt());
    fVUSRTC.push_back(ReadFloat());
    fLLNUTC.push_back(ReadBool());
    
    fETCLOW.push_back(ReadFloat()); //std::cout << fETCLOW[record] << std::endl;
    fETCHGH.push_back(ReadFloat()); //std::cout << fETCHGH[record] << std::endl;
    fNETCBN.push_back(ReadInt()); //std::cout << fNETCBN[record] << std::endl;
    fDETCBN.push_back(ReadFloat()); //std::cout << fDETCBN[record] << std::endl;
    
    CheckFormat();
    
    if (IsReadNeutrons(record)) {
      fIGMUTC.push_back(ReadInt());
      dtmp = 0.0;
      for (int ig=0; ig<fIGMUTC[record]+1; ig++) {
	dtmp = ReadFloat();
	fENGMAX.push_back(dtmp);
      }
      CheckFormat();
    } else fIGMUTC.push_back(0);
    
    vtmp.clear();
    for (int j=0; j<GetNbinsTotal(record); j++) {
      dtmp =  ReadFloat();
      vtmp.push_back(dtmp);
      //      std::cerr << record << "here: " << dtmp << std::endl;
    }
    fGMSTOR.push_back(vtmp);

    CheckFormat();

    if (ReadStatFlag(false)) break;

    record++;
  }

  CheckFormat();

  fNRecords = record+1;

  for (record=0; record<fNRecords; record++) {

    fTotResp.push_back(ReadFloat()); std::cerr << "total response: " << fTotResp[record] << std::endl;
    fTotRespErr.push_back(ReadFloat());
    std::cerr << "here1" << std::endl;
    CheckFormat();
    std::cerr << "here2" << std::endl;

    int nebxbn = ReadInt();
    int igmusx = ReadInt();
    
    if (GetNbinsTotal(record) != nebxbn+igmusx) {
      std::cerr << Warning("UstSuw::Read") << "\tnebxbn: " << nebxbn << "\t+\tigmusx: " << igmusx << "\t!=\t" << GetNbinsTotal(record) << std::endl;
    }

    PrintFloat(1);

    vtmp.clear();
    for (int j=0; j<GetNbinsTotal(record)+1; j++) { // +1 since number of boundaries are ONE more than number of intervals
      vtmp.push_back(ReadFloat());
    }
    fEPGMAX.push_back(vtmp);
    //std::cout << "fEPGMAX.size(): " << fEPGMAX.size() << std::endl;
    CheckFormat();
    
    vtmp.clear();
    for (int j=0; j<GetNbinsE(record); j++) vtmp.push_back(ReadFloat());
    fFlux.push_back(vtmp);

    vtmp.clear();
    for (int j=0; j<GetMaxNeutronGroup(record); j++) vtmp.push_back(ReadFloat());
    fFluxLEN.push_back(vtmp);

    CheckFormat();

    vtmp.clear();
    for (int j=0; j<GetNbinsE(record); j++) vtmp.push_back(ReadFloat());
    fFluxErr.push_back(vtmp);

    vtmp.clear();
    for (int j=0; j<GetMaxNeutronGroup(record); j++) vtmp.push_back(ReadFloat());
    fFluxLENErr.push_back(vtmp);
    
    CheckFormat();
    // Read Cumulative flux
    vtmp.clear();
    for (int j=0; j<GetNbinsE(record); j++) vtmp.push_back(ReadFloat());
    fCumulFlux.push_back(vtmp);

    vtmp.clear();
    for (int j=0; j<GetMaxNeutronGroup(record); j++) vtmp.push_back(ReadFloat());
    fCumulFluxLEN.push_back(vtmp);

    CheckFormat();

    vtmp.clear();
    for (int j=0; j<GetNbinsE(record); j++) vtmp.push_back(ReadFloat());
    fCumulFluxErr.push_back(vtmp);

    vtmp.clear();
    for (int j=0; j<GetMaxNeutronGroup(record); j++) vtmp.push_back(ReadFloat());
    fCumulFluxLENErr.push_back(vtmp);

    CheckFormat();

    PrintFloat(GetNbinsTotal(record));

    CheckFormat();

  }
  return true;
}

void UstSuw::Print(int record) const
{
  std::cout << std::endl;
  std::cout << "\tDetector n: " << record+1 <<  " " << fTITUTC[record] << std::endl;
  std::cout << "\t (Volume: " << fVUSRTC[record] << " cmc," << std::endl;
  std::cout << "\t  distr. scored: " << fIDUSTC[record] << " ," << std::endl;
  std::cout << "\t  from reg. " << fNRUSTC[record] << " )" << std::endl;
  std::cout << std::scientific;
  // total response is sum of cumulative fluxes:
  std::cout << "\t Tot. response (p/cmq/pr) " << fTotResp[record] << " +/- " << 100*fTotRespErr[record] << " %" << std::endl;
  std::cout << "\t ( --> Track l. (cm/pr) " << GetTrackLength(record) << " +/- " << 100*fTotRespErr[record] << " % )"  << std::endl;

  std::cout << std::endl;
  std::cout << "\t**** Different. Fluxes as a function of energy ****" << std::endl; 
  std::cout << "\t****     (integrated over solid angle)         ****" << std::endl;

  PrintEnergyBoundaries(record);

  std::cout << std::endl;
  std::cout << "\t Flux (Part/GeV/cmq/pr):" << std::endl << "\t ";
  for (int i=0; i<GetNbinsE(record); i++) {
    std::cout << GetFlux(record, i) << " +/- " << 100*GetFluxErr(record, i) << " %\t ";
    if ((i+1)%2==0) std::cout << std::endl << "\t ";
  }
  std::cout << std::endl;

  if (IsReadNeutrons(record)) {
    PrintLowEnergyBoundaries(record);
    
    std::cout << std::endl;
    std::cout << "\t Flux (Part/GeV/cmq/pr):" << std::endl << "\t ";
    for (int i=0; i<GetMaxNeutronGroup(record); i++) {
      std::cout << GetFluxLEN(record, i) << " +/- " << 100*GetFluxLENErr(record, i) << " %\t ";
      if ((i+1)%2==0) std::cout << std::endl << "\t ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << "\t **** Cumulative Fluxes as a function of energy ****" << std::endl;
  std::cout << "\t ****        (integrated over solid angle)      ****" << std::endl; // !!! NOT - integrated over Energy !!!

  PrintEnergyBoundaries(record);
  std::cout << std::endl;
  std::cout << "\t Cumul. Flux (Part/cmq/pr):" << std::endl << "\t  ";
  for (int i=0; i<GetNbinsE(record); i++) {
    std::cout << GetCumulFlux(record, i) << " +/- " << 100*GetCumulFluxErr(record, i) << " %\t ";
    if ((i+1)%2==0) std::cout << std::endl << "\t  ";
  }
  std::cout << std::endl;

  if (IsReadNeutrons(record)) {
    PrintLowEnergyBoundaries(record);
    
    std::cout << std::endl;
    std::cout << "\t Cumul. Flux (Part/cmq/pr):" << std::endl << "\t ";
    for (int i=0; i<GetMaxNeutronGroup(record); i++) {
      std::cout << GetCumulFluxLEN(record, i) << " +/- " << 100*GetCumulFluxLENErr(record, i) << " %\t ";
      if ((i+1)%2==0) std::cout << std::endl << "\t ";
    }
    std::cout << std::endl;
  }
}

float UstSuw::GetLowestBoundary(int record) const
{
  /*
    Return the lowest boundary [GeV]
  */
  if (!IsReadNeutrons(record)) return fEPGMAX[record][GetNbinsE(record)];
  else return 0.02; //fEPGMAX[record][GetNbinsE(record)-1];
}

void UstSuw::Print() const
{
  for (unsigned int i=0; i<fNRecords; i++) Print(i);
}


void UstSuw::PrintEnergyBoundaries(int record) const
{
  std::cout << "\t Energy boundaries (GeV):" << std::endl << "\t  ";
  for (int i=0; i<GetNbinsE(record); i++) {
    std::cout << fEPGMAX[record][i] << " ";
    if (((i+1)%5) == 0) std::cout << std::endl << "\t  ";
  }
  std::cout << std::endl;
  //  std::cout << "\t  aLowest boundary (GeV): " << fEPGMAX[GetNbinsE(record)] << " " << GetLowestBoundary(record) << std::endl;
  std::cout << "\t  Lowest boundary (GeV): " << GetLowestBoundary(record) << std::endl;
}

void UstSuw::PrintLowEnergyBoundaries(int record) const
{
  /*
    Prints energy boundaries for low energy neutrons.
   */

  if (IsReadNeutrons(record)==false) return;

  std::cout << std::endl;
  std::cout << "\t  Energy boundaries (GeV):" << std::endl << "\t  ";
  for (int i=GetNbinsE(record); i<GetNbinsTotal(record); i++) {
    std::cout << fEPGMAX[record][i] << " ";
    if ((i+1-GetNbinsE(record))%5==0) std::cout << std::endl << "\t  ";
  }
  std::cout << std::endl;
  std::cout << "\t Lowest boundary (GeV): " << fEPGMAX[record][GetNbinsTotal(record)] << std::endl; 
}

std::vector<float> UstSuw::GetEnergyBoundaries(int record) const
{
  /*
    Return a vector with energy boundaries starting from low energy.
    The number of boundaries is ONE more than the number of intervals.
    If low energy neutron flux is there, it is included in this vector.
    The method is used in ROOT_UstSuw::HistFlux(int)
  */
  std::vector<float> vec = fEPGMAX[record];
  std::reverse(vec.begin(), vec.end());

  /*  std::cout << "GetEnergyBoundaries:" << std::endl;
  std::cout << "nbinsE: " << GetNbinsE(record) << std::endl;
  std::cout << "EPGMAX length: " << fEPGMAX[record].size() << std::endl;
  for (int i=0; i<vec.size(); i++) std::cout << vec[i] << " ";
  std::cout << std::endl;*/

  return vec;
}

std::vector<float> UstSuw::GetFluxAll(int record) const
{
  /*
    Return a vector with flux starting from low energy.
    If low energy neutron flux is there, it is included in this vector. (!!! not yet implemented!!!)
    The method is used in ROOT_UstSuw::HistFlux(int)
   */

  std::vector<float> vec = fFlux[record];
  std::reverse(vec.begin(), vec.end());

  if (IsReadNeutrons(record)) {
    std::vector<float> veclen = fFluxLEN[record];
    std::reverse(veclen.begin(), veclen.end());
    veclen.insert(veclen.end(), vec.begin(), vec.end());
    vec.clear(); // ??? do we need it?
    return veclen;
  }
  
  return vec;
}

std::vector<float> UstSuw::GetFluxErrAll(int record) const
{
  /*
    Return a vector with flux errors starting from low energy.
    If low energy flux is there, it is included in this vector. (!!! not yet implemented!!!)
    The method is used in ROOT_UstSuw::HistFlux(int)
   */

  std::vector<float> vec = fFluxErr[record];
  std::reverse(vec.begin(), vec.end());

  if (IsReadNeutrons(record)) {
    std::vector<float> veclen = fFluxLENErr[record];
    std::reverse(veclen.begin(), veclen.end());
    veclen.insert(veclen.end(), vec.begin(), vec.end());
    vec.clear(); // ??? do we need it ???
    return veclen;
  }

  return vec;
}
