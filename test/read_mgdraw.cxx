#include "Mgdraw.h"
#include <iostream>
#include <iomanip>

using namespace std;
 
int main(int argc, char **argv)
{
	ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
	ReadFluka::Mgdraw *mgdraw = new ReadFluka::Mgdraw(argv[1]);

	for (int i=0; i<2; i++) {
		cout << "*********** event " << i << endl;
		mgdraw->ReadEvent();
	}
	mgdraw->Print();

	delete mgdraw;
	return 0;
}

