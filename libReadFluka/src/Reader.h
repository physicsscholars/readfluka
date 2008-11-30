#ifndef ReadFluka_h
#define ReadFluka_h

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

	class Reader {
	private:
		char                 *fchar;            // temprorary char
		char                 *fRunTitle;        // run title
		char                 *fRunTime;         // run time
		
	protected:
		std::ifstream             *fin;              //! file descriptor
		int                   fNCASE;           // number of primaries handled so far (current one included)
		
	public:
		static unsigned int gVerbose; // debug mode level
		Reader(const char *fname);
		virtual ~Reader();
		
		int                    ReadInt(unsigned int   n=1) const;    // read n integers
		float                  ReadFloat(unsigned int   n=1) const;  // read n floats
		bool                   ReadBool(unsigned int   n = 1) const; // read n bools
		void                   ReadRunTitle();              // read run titie
		void                   ReadRunTime();               // read run time
		char                  *GetRunTitle() const {return fRunTitle;}
		char                  *GetRunTime() const {return fRunTime;}
		const int              GetEntries() const {return fNCASE;}
		void                   SetVerboseLevel(unsigned int   level) {gVerbose = level;}
		int                    Nint(float   x) const;
		
		std::string                 Trimmed(std::string const& str, char const* sepSet=" ");
		std::ifstream              *GetFile() const {return fin;}
	};
}
#endif
