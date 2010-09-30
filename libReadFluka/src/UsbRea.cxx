                                    NOT YET IMPLEMENTED
#include <iostream>
#include "UsbRea.h"

#include <TH2F.h>
#include <TH3F.h>
#include <TString.h>

ClassImp(UsbRea)

UsbRea::UsbRea(const char* fname) : UsrBin(fname)
{
	gVerbose = 2;
	fGMSTOR = 0;
	fGBSTOR = 0;
  fNdist = 0;
	vector<Float_t> val;
	vector<Float_t> err;

	fReadCounter = 0;
  while (ReadStatFlag() == false) {
		UsrBin::fUsbReaFlag = true;
    UsrBin::Read();
		val.clear();

    ReadValues();
		for (Int_t i=0; i<GetNbin(); i++)
			val.push_back(fGMSTOR[i]);
		
		fValues.push_back(val);

		fXmin.push_back(UsrBin::GetXmin());
		fXmax.push_back(UsrBin::GetXmax());
    fYmin.push_back(UsrBin::GetYmin());
    fYmax.push_back(UsrBin::GetYmax());
    fZmin.push_back(UsrBin::GetZmin());
    fZmax.push_back(UsrBin::GetZmax());

		fdX.push_back(UsrBin::GetdX());
		fdY.push_back(UsrBin::GetdY());
		fdZ.push_back(UsrBin::GetdZ());

		fNbinsX.push_back(UsrBin::GetNbinsX());
    fNbinsY.push_back(UsrBin::GetNbinsY());
    fNbinsZ.push_back(UsrBin::GetNbinsZ());

		fDistType.push_back(UsrBin::GetDistType());
		fBinType.push_back(UsrBin::GetBinType());
		fBinName.push_back(UsrBin::GetBinName());

    fNdist++;
  }

  for (Int_t i=0; i<fNdist; i++) {
		err.clear();
    ReadErrors();
		for (Int_t i=0; i<GetNbin(); i++)
      err.push_back(fGBSTOR[i]);
    fErrors.push_back(err);
	}
}

UsbRea::~UsbRea()
{
	if (fGMSTOR) { delete [] fGMSTOR; fGMSTOR = 0; }
	if (fGBSTOR) { delete [] fGBSTOR; fGBSTOR = 0; }
}

Bool_t UsbRea::Read()
{
	if (fReadCounter>=fNdist) return false;
	MakeHist();

	cout << GetValue(1) << '\t' << GetError(1) << '\t' << GetZmin() << '\t' << GetDistType() << '\t' << GetBinType() << '\t' << GetdZ() << '\t' << GetBinName() << endl;
	fReadCounter++;
	return true;
}

Bool_t UsbRea::ReadValues()
{
  ReadFloat(2);

	if (!fin->good()) return false;

	if (fGMSTOR) delete fGMSTOR;
	fGMSTOR = new Float_t[GetNbin()];


	cout << "fNbin: " << GetNbin() << endl;
  if (gVerbose>1) cout << "GMSTOR(values):\t";
  for (Int_t j=0;  j<GetNbin(); j++) {
      fGMSTOR[j] = ReadFloat();
      if (gVerbose>1)  cout << fGMSTOR[j] << ' ';
    }
  if (gVerbose>1) cout << endl;

  return true;
}

Bool_t UsbRea::ReadErrors()
{
  ReadFloat(2);

  if (!fin->good()) return false;

	if (fGBSTOR) delete fGBSTOR;
  fGBSTOR = new Float_t[GetNbin()];

  if (gVerbose>1) cout << "GBSTOR(errors):\t";
  for (Int_t j=0;  j<GetNbin(); j++) {
      fGBSTOR[j] = ReadFloat();
      if (gVerbose>1)  cout << fGBSTOR[j] << ' ';
    }
  if (gVerbose>1) cout << endl;

  return true;
}

Bool_t UsbRea::ReadStatFlag()
{
	ReadInt(2);
	Char_t *value = "STATISTICS";
	const UInt_t n = strlen(value);
	Char_t flag[n+1];
	fin->read(flag, n);
	flag[n] = '\0';
	ReadInt();
	if (gVerbose>2) cout << "stat.flag:\t" << flag << endl;

	if (strcmp(flag, value) == 0)
		return true;
	else {
		fin->seekg(-n*sizeof(Char_t)-3*sizeof(Int_t), ios::cur); // go back
		return false;
	}
}

Float_t UsbRea::GetValue(Int_t i, Int_t j, Int_t k)
{
	if (fScored) delete fScored;
	fScored = new Float_t[GetNbins()];
	for (Int_t ii=0; ii<GetNbins(); ii++) {
		fScored[ii] = fValues[fReadCounter][ii];
	}
	return UsrBin::GetScored(i, j, k);
}

Float_t UsbRea::GetRelError(Int_t i, Int_t j, Int_t k)
{
  if (fScored) delete fScored;
  fScored = new Float_t[GetNbins()];
  for (Int_t ii=0; ii<GetNbins(); ii++) {
    fScored[ii] = fErrors[fReadCounter][ii];
  }
  return UsrBin::GetScored(i, j, k);
}


void UsbRea::MakeHist()
{
  TH1 *h = 0;
  TString title, z="z";
	
	title = GetBinName();
  title = title.Strip(TString::kBoth);

  if (GetDistType() == 208) z = "z [GeV/cm^{3}/#omega]";
	else if (GetDistType() == 202) z = "z [# charged particles/cm^{3}]";

	if (GetNbinsZ() == 1) {
		h = new TH2F(title.Data(), Form("%s hits (%.2f < z < %.2f);x [cm];y [cm];%s",
                                      title.Data(), GetZmin(), GetZmax(), z.Data()),
                   GetNbinsX(), GetXmin(), GetXmax(),
                   GetNbinsY(), GetYmin(), GetYmax());
      h->GetXaxis()->SetTitleOffset(1.4); h->GetXaxis()->CenterTitle();
      h->GetYaxis()->SetTitleOffset(1.4); h->GetYaxis()->CenterTitle();
      h->GetZaxis()->SetTitleOffset(1.2); h->GetZaxis()->CenterTitle();
      h->SetStats(kFALSE);
    } else {
      h = new TH3F(title.Data(), Form("%s hits (%.2f < z < %.2f);x [cm];y [cm];%s",
                                      title.Data(), GetZmin(), GetZmax(), z.Data()),
                   GetNbinsX(), GetXmin(), GetXmax(),
                   GetNbinsY(), GetYmin(), GetYmax(),
                   GetNbinsZ(), GetZmin(), GetZmax());
    }

    if (GetNbinsZ() == 1) {
      for (Int_t j=0; j<GetNbinsY(); j++)
        for (Int_t i=0; i<GetNbinsX(); i++) {
          h->SetBinContent(i+1, j+1, GetValue(i+1, j+1));
					h->SetBinError(i+1, j+1, GetError(i+1, j+1));
				}
    } else {
      for (Int_t k=0; k<GetNbinsZ(); k++)
        for (Int_t j=0; j<GetNbinsY(); j++)
          for (Int_t i=0; i<GetNbinsX(); i++) {
            h->SetBinContent(i+1, j+1, k+1, GetValue(i+1, j+1, k+1));
						h->SetBinError(i+1, j+1, k+1, GetError(i+1, j+1, k+1));
					}
    }
  fHists.Add(h->Clone());
	fHists.Print();
	delete h; h=0;
	cout << "end" << endl;
}
