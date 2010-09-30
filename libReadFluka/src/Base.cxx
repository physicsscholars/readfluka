#include "Base.h"
#include <iostream>
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
}

Base::~Base()
{
  /*
    Destructor
  */
  
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
  };

  return data[0];
}
 
float Base::ReadFloat(unsigned int n/* =1 */) const
{
  /*
    Read n doubles
  */
  
  float data[n];
  fin->read((char *)data, sizeof(float)*n);
  if (fin->fail()) {
    if (gVerbose>=kPRINT_HEADER) std::cerr << "read error in Base::ReadFloat()" << std::endl;
  };

  return data[0];
}

bool Base::ReadBool(unsigned int n/* =1 */) const
{
  /* 
     Read n bools
     since we're reading FORTRAN binary output, sizeof(bool) = sizeof(int)
     => read n ints:
  */
  
  return (bool)ReadInt(n);
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

  switch(code) {
  case -1:
    return "OPTIPHOT";
  case 1:
    return "PROTON";
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
  default:
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
  //std::clog << std::setw(100) << std::right << "*** SizeStart: " << fSize_start << std::endl;
  return fSize_start;
}

bool Base::SizeEnd(bool doExit)
{
  /*  fSize_end = ReadInt();
  //std::clog << std::setw(100) << std::right << "*** SizeEnd: " << fSize_end << std::endl;
  
  if (fSize_start != fSize_end) {
    std::cerr << "Base::CheckSize() warning:\t" << fSize_start << " " << fSize_end << std::endl;
    exit(WRONG_FORMAT);
    return false;
  } //else std::cerr << "SizeEnd: \t record OK" << std::endl;
  
  //std::cerr << "->check format ok: " << fSize_start << std::endl;
  return true;
  */

  // Read size end and compare it with size start
  int tmp = fSize_end;
  fSize_end = ReadInt();
  //std::clog << std::setw(100) << std::right << "*** SizeEnd: " << fSize_end << std::endl;
  
  if (fSize_start != fSize_end) {
    if (doExit == true) {
      std::cerr << "Base::CheckSize() warning:\t" << fSize_start << " " << fSize_end << std::endl;
      exit(WRONG_FORMAT);
    } else {
      fSize_end = tmp;
    }
    return false;
  }  

  return true;
}

bool Base::CheckFormat(bool doExit)
{
  bool status = false;
  int position = fin->tellg();
  if (bCheckFormat1st == false) {
    status = SizeEnd(doExit); 
    if (doExit == false) bCheckFormat1st = true;
    if ( (doExit == true) && (status == false) ) {
      fin->seekg(position, std::ios::beg);
      return false;
    }
  }
  if (doExit == true) SizeStart();
  else fin->seekg(position, std::ios::beg);
  return true;
}

bool Base::ReadStatFlag(bool doExit)
{
  /*
    Read STATISTICS, returns true if succeeded
  */
  int position = fin->tellg(); // remember the current position
  CheckFormat(doExit);
  int position0 = fin->tellg();
  std::cout << "position: " << position0 << " " << position << std::endl;
  char *value = "STATISTICS";
  const unsigned short n = strlen(value);
  char flag[n];
  fin->read(flag, n);
  flag[n] = '\0';

  if (gVerbose>kPRINT_MISC) std::cout << "stat.flag:\t" << flag << std::endl;
  
  std::cout << "int: " <<  ReadInt() << std::endl;
  
  CheckFormat(doExit);

  if (strcmp(flag, value) == 0) {
    if (gVerbose>kPRINT_MISC) std::cout << "statistics ok" << std::endl;
    return true;
  } else {
    std::cout << "not statistics" << std::endl;
    //    fin->seekg(-n*sizeof(char)-3*sizeof(int), std::ios::cur); // go back
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
