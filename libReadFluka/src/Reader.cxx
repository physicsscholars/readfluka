#include "Reader.h"
#include <iostream>

#define READ_ERROR 1
#define CANT_OPEN_FILE 2
#define FILENAME_EMPTY 3

using namespace ReadFluka;

unsigned int Reader::gVerbose = kPRINT_NOTHING;

Reader::Reader(const char *fname)
{
	/*
		Constructor
	 */

  if (fname == 0) {
    std::cerr << "Reader::Reader: input file name is not specified" << std::endl;
    exit(FILENAME_EMPTY);
  }

  fin = new std::ifstream(fname);
  if (!fin->is_open()) {
    std::cerr << "Reader::Reader: can't open input file " << fname << std::endl;
    exit(CANT_OPEN_FILE);
  }
  fchar     = new char[81];
  fRunTitle = new char[81];
  fRunTime  = new char[32];
}

Reader::~Reader()
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

int Reader::ReadInt(unsigned int n/* =1 */) const
{
  /*
		Read n integers
	*/

  int data[n];
  fin->read((char *)data, sizeof(int)*n);
  if (fin->fail()) {
    if (gVerbose>=kPRINT_HEADER) std::cerr << "read error in Reader::ReadInt()" << std::endl;
  };

  return data[0];
}
 
float Reader::ReadFloat(unsigned int n/* =1 */) const
{
  /*
		Read n doubles
	*/

  float data[n];
  fin->read((char *)data, sizeof(float)*n);
  if (fin->fail()) {
    if (gVerbose>=kPRINT_HEADER) std::cerr << "read error in Reader::ReadFloat()" << std::endl;
  };

  return data[0];
}

bool Reader::ReadBool(unsigned int n/* =1 */) const
{
  /* 
		 Read n bools
		 since we're reading FORTRAN binary output, sizeof(bool) = sizeof(int)
		 => read n ints:
	*/

  return (bool)ReadInt(n);
}

void Reader::ReadRunTitle()
{
	ReadInt();
  fin->read(fchar, 80);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) std::cerr << "read error in Reader::ReadRunTitle()" << std::endl;
  };
  fchar[80] = '\0';

  strcpy(fRunTitle, Trimmed(std::string(fchar)).c_str());
  if (gVerbose>=kPRINT_TITLE) std::cout << "title:\t" << fRunTitle << std::endl;
}

void Reader::ReadRunTime()
{
  fin->read(fchar, 32);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) std::cerr << "read error in Reader::ReadRunTime()" << std::endl;
  };
  fchar[32] = '\0';

  strcpy(fRunTime, Trimmed(std::string(fchar)).c_str());
  if (gVerbose>=kPRINT_TITLE) std::cout << "time:\t" << fRunTime << std::endl;
}

int Reader::Nint(float x) const
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

std::string Reader::Trimmed(std::string const& str, char const* sepSet)
{
  /* Return a string with leading/trailing characters of a set stripped
		 str - the original string
		 sepSet - C string with characters to be dropped
	*/

	std::string::size_type const first = str.find_first_not_of(sepSet);
  return ( first==std::string::npos )  ? std::string() : str.substr(first, str.find_last_not_of(sepSet)-first+1);
}
