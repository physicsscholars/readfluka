#include "Base.h"
#include <iostream>

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
	ReadInt();
  fin->read(fchar, 80);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) std::cerr << "read error in Base::ReadRunTitle()" << std::endl;
  };
  fchar[80] = '\0';

  strcpy(fRunTitle, Trimmed(std::string(fchar)).c_str());
	if (gVerbose>kPRINT_TITLE) std::cout << "Title:\t" << fRunTitle << std::endl;
}

void Base::ReadRunTime()
{
  fin->read(fchar, 32);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) std::cerr << "read error in Base::ReadRunTime()" << std::endl;
  };
  fchar[32] = '\0';

  strcpy(fRunTime, Trimmed(std::string(fchar)).c_str());
	if (gVerbose>=kPRINT_TITLE) std::cout << "Time:\t" << fRunTime << std::endl;
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
	return fSize_start;
}

bool Base::SizeEnd()
{
	// Read size end and compare it with size start

	fSize_end = ReadInt();
	if (fSize_start != fSize_end) {
		std::cerr << "Base::CheckSize() warning:\t" << fSize_start << " " << fSize_end << std::endl;
		exit(-1);
		return false;
	} //else std::cerr << "SizeEnd: \t record OK" << std::endl;

	return true;
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