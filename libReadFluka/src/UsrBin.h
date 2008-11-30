#ifndef UsrBin_h
#define UsrBin_h

/////////////////////////////////////////////////////
//
// UsrBin
//
// Class to read FLUKA's USRBIN output
// The numbering of fScored array is like this:
// 
//          | y
//    ------------|
//    |     |     |
//    |  3  |  4  |
//    |     |     |
//    ------o------- x
//    |     |     |
//    |  1  |  2  |
//    |     |     |
//    |------------
//
////////////////////////////////////////////////////

#include <fstream>
#include "Reader.h"

namespace ReadFluka {
	
	class UsrBin : public Reader {
	private:
		char         *fTITUSB;         // title of scored distribution
		int         fITUSBN;         //
		
		float       fWEIPRI;         // weight of primary particle
		int         fIDUSBN;          // distribution to be scored
		
		float       fXLOW;           // minimum value of x
		float       fXHIGH;           // maximum value of x
		float       fYLOW;           // minimum value of y
		float       fYHIGH;           // maxumum value of y
		float       fZLOW;           // minimum value of z
		float       fZHIGH;           // maximum value of z
		
		int         fNXBIN;          // number of x intervals
		int         fNYBIN;          // number of y intervals
		int         fNZBIN;          // number of z intervals
		int         fNbin;           // fXbin * fYbin * fZbin
		
		float       fDXUSBN;             // x bin width
		float       fDYUSBN;             // y bin width
		float       fDZUSBN;             // z bin width
		
		bool        fIsReadHeader;
		void          fReadHeader();    // read the header
		
		bool        fReadCartesian(); // read cartesian binning
		bool        fCheckBounds(int x, int y=1, int z=1) const;
		
	protected:
		float      *fScored;          // array of scored values
		bool        fUsbReaFlag;
	public:
		UsrBin(const char *fname);
		virtual ~UsrBin();
		
		const float GetXmin() const {return fXLOW;}
		const float GetXmin(int i, int j=1, int k=1) const;
		
		const float GetXmax() const {return fXHIGH;}
		const float GetXmax(int i, int j=1, int k=1) const;
		
		const float GetYmin() const {return fYLOW;}
		const float GetYmin(int i, int j, int k=1) const;
		
		const float GetYmax() const {return fYHIGH;}
		const float GetYmax(int i, int j, int k=1) const;
		
		const float GetZmin() const {return fZLOW;}
		const float GetZmin(int i, int j, int k) const;
		
		const float GetZmax() const {return fZHIGH;}
		const float GetZmax(int i, int j, int k) const;
		const float GetVolume(int i, int j, int k) const;
		
		const int GetNbinsX() const {return fNXBIN;}
		const int GetNbinsY() const {return fNYBIN;}
		const int GetNbinsZ() const {return fNZBIN;}
		const int GetNbin()  const {return fNbin; }
		
		const float GetdX() const {return fDXUSBN;}
		const float GetdY() const {return fDYUSBN;}
		const float GetdZ() const {return fDZUSBN;}
		
		const int GetDistType() const {return fIDUSBN;}
		const int GetBinType() const {return fITUSBN;}
		const float *GetScored() const {return fScored;}
		float  GetScored(int x, int y=1, int z=1) const;
		
		const char *GetBinName() const {return fTITUSB;}
		bool      Read(); // read 1 user binning
		void Print() const;
	};
}

#endif
