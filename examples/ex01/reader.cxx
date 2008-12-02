#include "UsrBin.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
	ReadFluka::Base::gVerbose = ReadFluka::kPRINT_FLUKA;
	ReadFluka::UsrBin *usrbin = new ReadFluka::UsrBin(argv[1]);

	unsigned int nbins = 0;
	while(usrbin->Read()) {
		//		usrbin->Print();
	}
	delete usrbin;

  return 0;
}
