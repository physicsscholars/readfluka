#include "UsrBin.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
	ReadFluka::Reader::gVerbose = ReadFluka::kPRINT_HEADER;
	ReadFluka::UsrBin *usrbin = new ReadFluka::UsrBin(argv[1]);

	unsigned int nbins = 0;
	while(usrbin->Read()) {
		usrbin->Print();
	}
	delete usrbin;

  return 0;
}
