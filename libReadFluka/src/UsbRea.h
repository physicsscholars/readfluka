                               NOT YET IMPLEMENTED
#ifndef UsbRea_h
#define UsbRea_h

//////////////////////////////////////////////////////
//
// UsbRea
//
// a class to read output from the FLUKA utility usbsuw
// (reads several USRBIN outputs to a single file)
// it does the same thing as $FLUTIL/usbrea
//
// format: header, fGMSTOR[NBINS] * number_of_binnings, "STATISTICS", 1, fGBSTOR[NBINS] * number_of_binnings
//
//////////////////////////////////////////////////////

#include "UsrBin.h"
#include <vector>
#include <string>

#include <TObject.h>
#include <TH1.h>
#include <TObjArray.h>

using namespace std;

class UsbRea : public UsrBin, public TObject {
private:
	Float_t                  *fGMSTOR; // current values
	Float_t                  *fGBSTOR; // current percentage errors
	vector< vector<Float_t> > fValues; // values of all binnings
	vector< vector<Float_t> > fErrors; // percentage errors of all binnings
	Bool_t                    ReadValues();
	Bool_t                    ReadErrors();
	Bool_t                    ReadStatFlag();
	Int_t                     fReadCounter;      // read counter
	Int_t                     fNdist;           // number of distributions in input file

	vector<Float_t>           fXmin;
	vector<Float_t>           fXmax;
	vector<Float_t>           fYmin;
	vector<Float_t>           fYmax;
	vector<Float_t>           fZmin;
	vector<Float_t>           fZmax;
	vector<Int_t>             fNbinsX;
	vector<Int_t>             fNbinsY;
	vector<Int_t>             fNbinsZ;
	vector<Float_t>           fdX;
	vector<Float_t>           fdY;
	vector<Float_t>           fdZ;
	vector<Int_t>             fDistType; // distribution to be scored
	vector<Int_t>             fBinType; // type of the binning
	vector<string>            fBinName; // name of the binning

	TH1F                     *fHist;  // current histogram
	TObjArray                 fHists; // list of histograms
	void                      MakeHist(); // creates a histogram
public:
	UsbRea(const char* fname);
	virtual ~UsbRea();

	Float_t GetValue(Int_t i, Int_t j=1, Int_t k=1);
	Float_t GetRelError(Int_t i, Int_t j=1, Int_t k=1);  // relative error
	Float_t GetError(Int_t i, Int_t j=1, Int_t k=1) {return GetRelError(i, j, k)*GetValue(i, j, k); }  // absolute error


	Float_t GetXmin() const {return fXmin[fReadCounter]; }
	Float_t GetXmax() const {return fXmax[fReadCounter]; }
  Float_t GetYmin() const {return fYmin[fReadCounter]; }
  Float_t GetYmax() const {return fYmax[fReadCounter]; }
  Float_t GetZmin() const {return fZmin[fReadCounter]; }
  Float_t GetZmax() const {return fZmax[fReadCounter]; }

	Float_t GetdX() const {return fdX[fReadCounter]; }
	Float_t GetdY() const {return fdY[fReadCounter]; }
	Float_t GetdZ() const {return fdZ[fReadCounter]; }

	Int_t GetNbinsX() const {return fNbinsX[fReadCounter]; }
  Int_t GetNbinsY() const {return fNbinsY[fReadCounter]; }
  Int_t GetNbinsZ() const {return fNbinsZ[fReadCounter]; } 
	Int_t GetNbins() const {return GetNbinsX()*GetNbinsY()*GetNbinsZ(); }

	Int_t GetDistType() const {return fDistType[fReadCounter]; }
	Int_t GetBinType() const {return fBinType[fReadCounter]; }
	const Char_t *GetBinName() const {return fBinName[fReadCounter].c_str(); }
	TH1F *GetHistogram() const { return (TH1F*)fHists[fReadCounter-1]; }
	TObjArray *GetHistograms() {return &fHists; }

	Bool_t Read();

	ClassDef(UsbRea, 1)
};

#endif
