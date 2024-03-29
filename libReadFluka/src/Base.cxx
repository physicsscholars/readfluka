#include "Base.h"
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <sstream>

using namespace ReadFluka;

int Base::gVerbose = kPRINT_TITLE;

Base::Base(const char *fname)
{
  /*
    Constructor
  */
  
  if (fname == 0) {
    std::cerr << "Base::Base: input file name is not specified" << std::endl;
    exit(FILENAME_EMPTY);
  }

  fin = new std::ifstream(fname);
  if (!fin->is_open()) {
    std::cerr << "Base::Base: can't open input file " << fname << std::endl;
    exit(CANT_OPEN_FILE);
  }
  fchar     = new char[81];
  fRunTitle = new char[81];
  fRunTime  = new char[32];

  bCheckFormat1st = true;
  fWCTOT = 0.0f;
  fNCTOT = fMCTOT = fMBATCH = 0;
}

Base::~Base()
{
  delete [] fRunTime;  fRunTime = 0;
  delete [] fRunTitle; fRunTitle = 0;
  delete [] fchar;     fchar = 0;
  if (fin) {
    fin->close();
    delete fin;
    fin = 0;
  }
}

int Base::ReadInt(unsigned int n/* =1 */) const
{
  /*
    Read n integers
  */
  
  int data[n];
  fin->read((char *)data, sizeof(int)*n);
  if (fin->fail()) {
    if (gVerbose>=kPRINT_HEADER) std::cerr << "read error in Base::ReadInt()" << std::endl;
  }
  
  return data[0];
}

float Base::ReadFloat(unsigned int n/* =1 */) const
{
  /*
    Read n floats
  */
  
  float data[n];
  fin->read((char *)data, sizeof(float)*n);
  if (fin->fail()) {
    if (gVerbose>=kPRINT_HEADER) std::cerr << "read error in Base::ReadFloat()" << std::endl;
  };

  return data[0];
}

void Base::ReadRunTitle()
{
  SizeStart(); //ReadInt();
	
  fin->read(fchar, 80);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) std::cerr << "read error in Base::ReadRunTitle()" << std::endl;
  };
  fchar[80] = '\0';

  strcpy(fRunTitle, Trimmed(std::string(fchar)).c_str());
  if (gVerbose>kPRINT_TITLE) std::clog << "Title:\t" << fRunTitle << std::endl;
}

void Base::ReadRunTime()
{
  fin->read(fchar, 32);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) std::cerr << "read error in Base::ReadRunTime()" << std::endl;
  };
  fchar[32] = '\0';
  
  strcpy(fRunTime, Trimmed(std::string(fchar)).c_str());
  if (gVerbose>=kPRINT_TITLE) std::clog << "Time:\t" << fRunTime << std::endl;
}

float *Base::Read(int size) const
{
  float *data = new float(size);
  fin->read((char *)data, sizeof(float)*size);
  if (fin->fail()) {
    std::cerr << "read error in Base::Read()" << std::endl;
  };
  
  return data; // must be deleted
}

float *Base::ReadFortran()
{
  int size = ReadInt();
  float *data = Read(size-3);
  int size2 = ReadInt();
  if (size != size2) {
    std::cerr << "Base::ReadFortran(): read error: " << size << " != " << size2 << std::endl;
    //	delete [] data;
    return 0;
  }
  return data;
}

const char *Base::Code2Name(int code, bool replace) const
{
  /*
    Converts FLUKA particle code to its name.
    if 'replace' is true then replaces all non-alphanumeric characters
    (helpfull when using this name for a branch of a tree or as a histogram name). 
   */
  //  std::cerr << "code: " << code << std::endl;

  switch(code) {
  case -6:
    return replace ? "He4" : "4-HELIUM";
  case -5:
    return replace ? "He3" : "3-HELIUM";
  case -1:
    return "OPTIPHOT";
  case 1:
    return "PROTON";
  case 7:
    return "PHOTON";
  case 8:
    return "NEUTRON";
  case 201:
    return replace ? "ALL_PART" : "ALL-PART";
  case 202:
    return replace ? "ALL_CHAR" : "ALL-CHAR";
  case 203:
    return replace ? "ALL_NEUT" : "ALL-NEUT";
  case 204:
    return replace ? "ALL_NEGA" : "ALL-NEGA";
  case 205:
    return replace ? "ALL_POSI" : "ALL-POSI";
  case 206:
    return "NUCLEONS";
  case 208:
    return "ENERGY";
  case 209:
    return replace ? "PIONSPM" : "PIONS+-";
  case 210:
    return "BEAMPART";
  case 222:
    return replace ? "NEU_BALA" : "NEU-BALA";
  case 228:
    return "DOSE";
  case 234:
    return "ACTIVITY";
  case 235:
    return "ACTOMASS";
  case 239:
    std::cerr << "here 239" << std::endl;
    return replace ? "DPA_SCO" : "DPA-SCO";
  case 240:
    return replace ? "DOSE_EQ" : "DOSE-EQ";
  default:
    //    std::cerr << "Warning: Default is using in Base::Code2Name for " << code << std::endl;
    std::ostringstream name;
    name << "id" << code;
    return name.str().c_str();
  }

}

int Base::Nint(float x) const
{
  /* 
     Round to nearest integer. Rounds half integers to the nearest even integer.
  */
  
  int i;
  if (x >= 0) {
    i = int(x + 0.5);
    if (x + 0.5 == float(i) && i & 1) i--;
  } else {
    i = int(x - 0.5);
    if (x - 0.5 == float(i) && i & 1) i++;
    
  }
  
  return i;
}

int Base::SizeStart()
{
  fSize_start = ReadInt();
  bCheckFormat1st = false;
  //  std::clog << std::setw(100) << std::right << "*** SizeStart: " << fSize_start << std::endl;
  return fSize_start;
}

bool Base::SizeEnd(bool doExit)
{
  /*  fSize_end = ReadInt();
      std::clog << std::setw(100) << std::right << "*** SizeEnd: " << fSize_end << std::endl;
  
  if (fSize_start != fSize_end) {
    std::cerr << "Base::CheckSize() warning:\t" << fSize_start << " " << fSize_end << std::endl;
    exit(WRONG_FORMAT);
    return false;
  } //else std::cerr << "SizeEnd: \t record OK" << std::endl;
  
  //std::cerr << "->check format ok: " << fSize_start << std::endl;
  return true;
  */

  // Read size end and compare it with size start
  int position = fin->tellg();
  int tmp = fSize_end;
  fSize_end = ReadInt();
  
  if (fSize_start != fSize_end) {
    if (doExit == true) {
      std::cout << Warning("Base::CheckSize():\t") << fSize_start << " " << fSize_end << std::endl;
      exit(WRONG_FORMAT);
    } else {
      fSize_end = tmp;
      fin->seekg(position, std::ios::beg);
    }
    return false;
  }  

  if (doExit == false) fin->seekg(position, std::ios::beg);

  return true;
}

bool Base::CheckFormat(const char *message, bool doExit)
{
  std::cout << "Base::CheckFormat:\t" << message << std::endl;
  return CheckFormat(doExit); 
}

bool Base::CheckFormat(bool doExit)
{
  //std::cerr << "Base::CheckFormat" << std::endl;
  bool status;
  if (bCheckFormat1st == false) {
    //    std::cerr << "bCheckFormat1st is false" << std::endl;
    status = SizeEnd(doExit); 
    if (doExit == true) {
      bCheckFormat1st = true;
    }
    if (status == false) return false; // !!! is it ok? put it in prev if?
  }
  
  if (doExit == true) SizeStart();
  
  return true;
}

bool Base::ReadStatFlag(bool doExit)
{
  /*
    Read STATISTICS, returns true if succeeded
  */
  //  std::cerr << "       read stat flag" << std::endl;
  int position = fin->tellg(); // remember the current position
  //CheckFormat(doExit);
  //int position0 = fin->tellg();
  //  std::cout << "position: " << position0 << " " << position << std::endl;
  const char *value = "STATISTICS";
  const unsigned short n = strlen(value);
  char flag[n];
  fin->read(flag, n);
  flag[n] = '\0';

  if (gVerbose>kPRINT_MISC) std::cout << "stat.flag:\t" << flag << std::endl;
  
  if (true) ReadInt();
  else std::cout << "\t->ReadStatFlag: int = " <<  ReadInt() << std::endl;
  
  CheckFormat(doExit);

  if (strcmp(flag, value) == 0) {
    if (gVerbose>kPRINT_MISC)  std::cout << "statistics ok" << std::endl;
    return true;
  } else {
    //   std::cout << "not statistics" << std::endl;
    //        fin->seekg(-n*sizeof(char)-3*sizeof(int), std::ios::cur); // go back
    fin->seekg(position, std::ios::beg);
    return false;
  }
}

std::string Base::Trimmed(std::string const& str, char const* sepSet)
{
  /* Return a string with leading/trailing characters of a set stripped
		 str - the original string
		 sepSet - C string with characters to be dropped
	*/

  std::string::size_type const first = str.find_first_not_of(sepSet);
  return ( first==std::string::npos )  ? std::string() : str.substr(first, str.find_last_not_of(sepSet)-first+1);
}

std::string Base::Warning(const char *msg) const
{
  std::ostringstream str;
  str << "\033[33m WARNING: " << msg << "\033[0m";
  return str.str();
}

void Base::Error(const char *msg) const
{
  std::ostringstream str;
  std::cout << "\033[31mERROR " << msg << "\033[0m" << std::endl;
  exit(1);
}

std::string Base::AsFortran(double val, int precision, std::ios_base::fmtflags flags) const
{
  /*
    This method is to be used to print 'val' with a given precision, so we can diff it with the Fortran output.
   */
  //  std::ios_base::fmtflags flags=std::ios::fixed;
  std::ostringstream str;
  str.precision(precision);
  str.setf(flags);
  str << val;

  return str.str();
}

void Base::PrintInt(unsigned int n) const
{
  /*
    Read and print n integers. Usefull for guessing the format of undocumented FLUKA files.
   */
  std::cerr << "int:\t";
  for (unsigned int i=0; i<n; i++) {
    if (fin->eof()) {
      std::cerr << Warning("PrintInt: end of file") << std::endl;
      break;
    }
    std::cerr << ReadInt() << " ";
  }
  std::cerr << std::endl;
}

void Base::PrintFloat(unsigned int n) const
{
  /*
    Read and print n float. Usefull for guessing the format of undocumented FLUKA files.
   */
  std::cerr << "float:\t";
  for (unsigned int i=0; i<n; i++) {
    if (fin->eof()) {
      std::cerr << Warning("PrintFloat: end of file") << std::endl;
      break;
    }
    std::cerr << ReadFloat() << " ";
  }
  std::cerr << std::endl;
}
