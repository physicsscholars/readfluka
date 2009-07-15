#ifndef ReadFluka_h
#define ReadFluka_h

#define READ_ERROR 1
#define CANT_OPEN_FILE 2
#define FILENAME_EMPTY 3
#define WRONG_FORMAT 4

///////////////////////////////////////////////////////////////
//
// ReadFluka
//
// base class to read FLUKA binary outputs
//
//////////////////////////////////////////////////////////////

#include <fstream>

namespace ReadFluka {
	enum EVerboseLevel {kPRINT_NOTHING, kPRINT_TITLE, kPRINT_HEADER, kPRINT_MISC, kPRINT_SCORED};

	class Base {
	private:
		char                 *fchar;            // temprorary char
		char                 *fRunTitle;        // run title
		char                 *fRunTime;         // run time
		
	
	protected:
		std::ifstream             *fin;              //! file descriptor
		int                   fNCASE;           // number of primaries handled so far (current one included)
		float       fWEIPRI;         // weight of primary particle
		int         fMB;             // number of binning

		int                   fSize_start;
		int                   fSize_end;

	public:
		static int gVerbose;
		Base(const char *fname);
		virtual ~Base();
		
		int                    ReadInt(unsigned int   n=1) const;    // read n integers
		float                  ReadFloat(unsigned int   n=1) const;  // read n floats
		bool                   ReadBool(unsigned int   n = 1) const; // read n bools
		void                   ReadRunTitle();              // read run titie
		void                   ReadRunTime();               // read run time
		float                 *Read(int size) const;
		float                 *ReadFortran(); // Read a fortran structure
		char                  *GetRunTitle() const {return fRunTitle;}
		char                  *GetRunTime() const {return fRunTime;}
		const int              GetEntryNumber() const {return fNCASE;}
		const float            GetWeight() const {return fWEIPRI;}
		const int              GetBinNumber() const {return fMB;}

		const char            *Code2Name(int code, bool replace = false) const;

		int                    Nint(float   x) const;
		int  SizeStart();
		bool SizeEnd();

		
		std::string                 Trimmed(std::string const& str, char const* sepSet=" ");
		std::ifstream              *GetFile() const {return fin;}
	};
}
#endif
