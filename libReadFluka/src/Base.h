#ifndef ReadFluka_h
#define ReadFluka_h

#define READ_ERROR 1
#define CANT_OPEN_FILE 2
#define FILENAME_EMPTY 3
#define WRONG_FORMAT 4
#define FATAL_ERROR 5
#define NOT_IMPLEMENTED 6

///////////////////////////////////////////////////////////////
//
// ReadFluka
//
// the base class to read FLUKA binary outputs
//
//////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>

namespace ReadFluka {
  enum EVerboseLevel {kPRINT_NOTHING, kPRINT_TITLE, kPRINT_HEADER, kPRINT_MISC, kPRINT_SCORED};
  //                      steradian
  enum EUnit {kDEG, kRAD,    kSR       };
  
  class Base {
  private:
    char                 *fchar;           // temprorary char
    char                 *fRunTitle;       // run title
    char                 *fRunTime;        // run time
    bool                  bCheckFormat1st; // used by CheckFormat: true if we have not read the check format numbers yet
    
   
  protected:
    std::ifstream        *fin;             //! file descriptor
    int                   fNCASE;          // number of primaries handled so far (current one included)
    float                 fWEIPRI;         // weight of a primary particle
    int                   fMB;             // number of binning
    
    int                   fSize_start;
    int                   fSize_end;
    
  public:
    float fWCTOT; // total weight
    int fNCTOT; // total number of incident particles
    int fMCTOT; //
    int fMBATCH;

    static int gVerbose;
    Base(const char *fname);
    virtual ~Base();
    
    int                    ReadInt(unsigned int   n=1) const;    // read n integers
    float                  ReadFloat(unsigned int   n=1) const;  // read n floats
    bool                   ReadBool(unsigned int n = 1) const {return ReadInt(n);} //in fortran sizeof(bool)=sizeof(int)
    void                   ReadRunTitle();              // read run titie
    void                   ReadRunTime();               // read run time
    float                 *Read(int size) const;
    float                 *ReadFortran(); // Read a fortran structure
    char                  *GetRunTitle() const {return fRunTitle;}
    char                  *GetRunTime() const {return fRunTime;}

    const int              GetEntryNumber() const {return fNCASE;}
    inline const int       GetNCASE() const {return fNCASE;} // the same as GetEntryNumber

    const float            GetWeight() const {return fWEIPRI;}
    inline const float     GetWEIPRI() const {return fWEIPRI;} // the same as GetWeight

    const int              GetBinNumber() const {return fMB;}
    
    const char            *Code2Name(int code, bool replace = false) const;
    
    int                    Nint(float   x) const;
    int  SizeStart();
    bool SizeEnd(bool doExit=true);
    bool CheckFormat(bool doExit=true);

    bool ReadStatFlag(bool doExit=true);
    void Warning(const char *msg) const;
    std::string AsFortran(double val, int precision, std::ios_base::fmtflags flags=std::ios::fixed) const;
    void PrintInt(unsigned int n) const;
    void PrintFloat(unsigned int n) const;
    
    std::string                 Trimmed(std::string const& str, char const* sepSet=" ");
    std::ifstream              *GetFile() const {return fin;}
  };
}
#endif
