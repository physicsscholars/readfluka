#include "UstSuw.h"

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
    
    for (int j=0; j<GetNbinsTotal(record); j++) {
      dtmp =  ReadFloat();
      fGMSTOR.push_back(dtmp);
    }

    CheckFormat();

    if (ReadStatFlag(false)) break;

    record++;
  }

  CheckFormat();

  fNRecords = record+1;

  for (record=0; record<fNRecords; record++) {

    fTotResp.push_back(ReadFloat());
    fTotRespErr.push_back(ReadFloat());
    std::cerr << "here1" << std::endl;
    if ((record>0) && (IsReadNeutrons(record))) PrintFloat(2);
    CheckFormat();
    std::cerr << "here2" << std::endl;

    int nebxbn = ReadInt();
    int igmusx = ReadInt();
    
    if (GetNbinsTotal(record) != nebxbn+igmusx) {
      std::cerr << Warning("UstSuw::Read") << "\tnebxbn: " << nebxbn << "\t+\tigmusx: " << igmusx << "\t!=\t" << GetNbinsTotal(record) << std::endl;
    }

    PrintFloat(1);

    for (int j=0; j<GetNbinsTotal(record)+1; j++) { // +1 since number of boundaries are ONE more than number of intervals
      fEPGMAX.push_back(ReadFloat());
    }
    //std::cout << "fEPGMAX.size(): " << fEPGMAX.size() << std::endl;
    CheckFormat();
    
    for (int j=0; j<GetNbinsE(record); j++) fFlux.push_back(ReadFloat());
    for (int j=0; j<GetMaxNeutronGroup(record); j++) fFluxLEN.push_back(ReadFloat());

    CheckFormat();

    for (int j=0; j<GetNbinsE(record); j++) fFluxErr.push_back(ReadFloat());
    for (int j=0; j<GetMaxNeutronGroup(record); j++) fFluxLENErr.push_back(ReadFloat());
    
    CheckFormat();
    // Read Cumulative flux
    for (int j=0; j<GetNbinsE(record); j++) fCumulFlux.push_back(ReadFloat());
    for (int j=0; j<GetMaxNeutronGroup(record); j++) fCumulFluxLEN.push_back(ReadFloat());

    CheckFormat();

    for (int j=0; j<GetNbinsE(record); j++) fCumulFluxErr.push_back(ReadFloat());
    for (int j=0; j<GetMaxNeutronGroup(record); j++) fCumulFluxLENErr.push_back(ReadFloat());

    CheckFormat();
  }
 
}

void UstSuw::Print(int record)
{
  std::cout << std::endl;
  std::cout << "\tDetector n: " << record+1 <<  " " << fTITUTC[record] << std::endl;
  std::cout << "\t (Volume: " << fVUSRTC[record] << " cmc," << std::endl;
  std::cout << "\t  distr. scored: " << fIDUSTC[record] << " ," << std::endl;
  std::cout << "\t  from reg. " << fNRUSTC[record] << " )" << std::endl;
  std::cout << std::scientific;
  std::cout << "\t Tot. response (p/cmq/pr) " << fTotResp[record] << " +/- " << 100*fTotRespErr[record] << " %" << std::endl;
  std::cout << "\t ( --> Track l. (cm/pr) " << GetTrackLength(record) << " +/- " << 100*fTotRespErr[record] << " % )"  << std::endl;

  std::cout << std::endl;
  std::cout << "\t**** Different. Fluxes as a function of energy ****" << std::endl; 
  std::cout << "\t****     (integrated over solid angle)         ****" << std::endl;

  PrintEnergyBoundaries(record);

  std::cout << std::endl;
  std::cout << "\t Flux (Part/GeV/cmq/pr):" << std::endl << "\t ";
  for (int i=0; i<GetNbinsE(record); i++) {
    std::cout << GetFlux(i) << " +/- " << 100*GetFluxErr(i) << " %\t ";
    if ((i+1)%2==0) std::cout << std::endl << "\t ";
  }
  std::cout << std::endl;

  if (IsReadNeutrons(record)) {
    PrintLowEnergyBoundaries(record);
    
    std::cout << std::endl;
    std::cout << "\t Flux (Part/GeV/cmq/pr):" << std::endl << "\t ";
    for (int i=0; i<GetMaxNeutronGroup(record); i++) {
      std::cout << GetFluxLEN(i) << " +/- " << 100*GetFluxLENErr(i) << " %\t ";
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
    std::cout << GetCumulFlux(i) << " +/- " << 100*GetCumulFluxErr(i) << " %\t ";
    if ((i+1)%2==0) std::cout << std::endl << "\t  ";
  }
  std::cout << std::endl;

  if (IsReadNeutrons(record)) {
    PrintLowEnergyBoundaries(record);
    
    std::cout << std::endl;
    std::cout << "\t Cumul. Flux (Part/cmq/pr):" << std::endl << "\t ";
    for (int i=0; i<GetMaxNeutronGroup(record); i++) {
      std::cout << GetCumulFluxLEN(i) << " +/- " << 100*GetCumulFluxLENErr(i) << " %\t ";
      if ((i+1)%2==0) std::cout << std::endl << "\t ";
    }
    std::cout << std::endl;
  }
}


void UstSuw::PrintEnergyBoundaries(int record) const
{
  std::cout << "\t Energy boundaries (GeV):" << std::endl << "\t  ";
  for (int i=0; i<GetNbinsE(record); i++) {
    std::cout << fEPGMAX[i] << " ";
    if (((i+1)%5) == 0) std::cout << std::endl << "\t  ";
  }
  std::cout << std::endl;
  std::cout << "\t  Lowest boundary (GeV): " << fEPGMAX[GetNbinsE(record)] << std::endl;
}

void UstSuw::PrintLowEnergyBoundaries(int record) const
{
  if (IsReadNeutrons(record)==false) return;

  std::cout << std::endl;
  std::cout << "\t  Energy boundaries (GeV):" << std::endl << "\t  ";
  for (int i=GetNbinsE(record); i<GetNbinsTotal(record); i++) {
    std::cout << fEPGMAX[i] << " ";
    if ((i+1-GetNbinsE(record))%5==0) std::cout << std::endl << "\t  ";
  }
  std::cout << std::endl;
  std::cout << "\t Lowest boundary (GeV): " << fEPGMAX[GetNbinsTotal(record)] << std::endl; 
}
