#include "UsrBin.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
	ReadFluka::Reader::gVerbose = ReadFluka::kPRINT_HEADER;
	ReadFluka::UsrBin *usrbin = new ReadFluka::UsrBin(argv[1]);

	while(usrbin->Read()) {
		cout << "here" << endl;
	}
	delete usrbin;

  return 0;
}
