#include "ReadFluka.h"
#include <iostream>

#define READ_ERROR 1
#define CANT_OPEN_FILE 2
#define FILENAME_EMPTY 3

using namespace std;

unsigned int ReadFluka::gVerbose = kPRINT_NOTHING;

ReadFluka::ReadFluka(const char *fname)
{
	/*
		Constructor
	 */

  if (fname == 0) {
    cerr << "ReadFluka::ReadFluka: input file name is not specified" << endl;
    exit(FILENAME_EMPTY);
  }

  fin = new ifstream(fname);
  if (!fin->is_open()) {
    cerr << "ReadFluka::ReadFluka: can't open input file " << fname << endl;
    exit(CANT_OPEN_FILE);
  }
  fchar     = new char[81];
  fRunTitle = new char[81];
  fRunTime  = new char[32];
}

ReadFluka::~ReadFluka()
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

int ReadFluka::ReadInt(unsigned int n/* =1 */) const
{
  /*
		Read n integers
	*/

  int data[n];
  fin->read((char *)data, sizeof(int)*n);
  if (fin->fail()) {
    if (gVerbose>=kPRINT_HEADER) cerr << "read error in ReadFluka::ReadInt()" << endl;
  };

  return data[0];
}
 
float ReadFluka::ReadFloat(unsigned int n/* =1 */) const
{
  /*
		Read n doubles
	*/

  float data[n];
  fin->read((char *)data, sizeof(float)*n);
  if (fin->fail()) {
    if (gVerbose>=kPRINT_HEADER) cerr << "read error in ReadFluka::ReadFloat()" << endl;
  };

  return data[0];
}

bool ReadFluka::ReadBool(unsigned int n/* =1 */) const
{
  /* 
		 Read n bools
		 since we're reading FORTRAN binary output, sizeof(bool) = sizeof(int)
		 => read n ints:
	*/

  return (bool)ReadInt(n);
}

void ReadFluka::ReadRunTitle()
{
	ReadInt();
  fin->read(fchar, 80);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) cerr << "read error in ReadFluka::ReadRunTitle()" << endl;
  };
  fchar[80] = '\0';

  strcpy(fRunTitle, Trimmed(string(fchar)).c_str());
  if (gVerbose>=kPRINT_TITLE) cout << "title:\t" << fRunTitle << endl;
}

void ReadFluka::ReadRunTime()
{
  fin->read(fchar, 32);
  if (!fin->good()) {
    if (gVerbose>=kPRINT_TITLE) cerr << "read error in ReadFluka::ReadRunTime()" << endl;
  };
  fchar[32] = '\0';

  strcpy(fRunTime, Trimmed(string(fchar)).c_str());
  if (gVerbose>=kPRINT_TITLE) cout << "time:\t" << fRunTime << endl;
}

int ReadFluka::Nint(float x) const
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

string ReadFluka::Trimmed(std::string const& str, char const* sepSet)
{
  // Returns a string with leading/trailing characters of a set stripped

  // str - the original string
  // sepSet - C string with characters to be dropped

  string::size_type const first = str.find_first_not_of(sepSet);
  return ( first==string::npos )  ? string() : str.substr(first, str.find_last_not_of(sepSet)-first+1);
}
