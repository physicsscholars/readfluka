#include "UsrYield.h"
#include <iostream>
#include <iomanip>

using namespace std;
 
int main(int argc, char **argv)
{
	ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
	ReadFluka::UsrYield *usryield = new ReadFluka::UsrYield(argv[1]);

	cout << scientific << setprecision(4);

	cout << " *****  " << usryield->GetRunTitle() << " *****" << endl;
	cout << '\t' << usryield->GetRunTime() << endl;
	cout << "\tTotal number of particles followed\t" << usryield->GetEntries();
	cout << ", for a total weight of  ";
	cout << usryield->GetWeight() << endl;

	while (usryield->Read()) {
		cout << "usryield bin number: " << usryield->GetBinNumber()+1 << endl;
		cout << "title: " << usryield->GetTitle() << endl;
		cout << "my: " << usryield->GetMY() << endl;
		//		cout << "generalized particle n. " << usryield->GetDistType() << endl;
		
	}

  delete usryield;
  return 0;
}
