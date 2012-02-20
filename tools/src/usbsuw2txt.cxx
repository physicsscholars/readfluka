#include "UsbSuw.h"
#include <iostream>
#include <cstring>

using namespace std;

int help()
{
  cerr << "Usage: usbsuw2txt [--format=format] file" << endl;
  cerr << "       'file' is the output produced by $FLUTIL/usbsuw" << endl;
  cerr << "       Available formats:" << endl;
  cerr << "        CFD - prints low edges of the bins, then value and relative error" << endl;
  return -1;
}

int main(int argc, const char **argv)
{
  const char *file_in = "";
  const char *format = "";
  if (argc == 2){
    file_in = argv[1];
  } else if (argc == 3) {
    format =  argv[1];
    file_in = argv[2];
  } else {
    return help();
  }

  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::UsbSuw *usbsuw = new ReadFluka::UsbSuw(file_in);
  
  /*cout << "***** " << usbsuw->GetRunTitle() << " *****" << endl;
  cout << usbsuw->GetRunTime() << endl;
  cout << "Total number of particles followed " << usbsuw->GetEntryNumber();
  cout << ", for a total weight of " << usbsuw->GetWeight() << endl;
  */

  //usbsuw->Read();
  int i=0;
  while (usbsuw->Read()) {
    if (!strcmp(format, "")) {
      usbsuw->Print();
    } else if (!strcmp(format, "--format=CFD")) {
	std::cout << std::scientific;// << std::setprecision(4);
	
	std::cout << "# " << usbsuw->GetRunTitle() << std::endl;
	std::cout << "# " << usbsuw->GetRunTime() << std::endl;
	std::cout << "# Averaged over " << usbsuw->GetNBATCH() << " cycles" << std::endl;
	std::cout << "# Format: x_center y_center z_center value rel_error" << std::endl;
	//	std::cout << "# Format: x_low y_low z_low value rel_error" << std::endl;
	
	for (int binx=0; binx<usbsuw->GetNXBIN(i); binx++) {
	  for (int biny=0; biny<usbsuw->GetNYBIN(i); biny++) {
	    for (int binz=0; binz<usbsuw->GetNZBIN(i); binz++) {
	      std::cout << 
		//usbsuw->GetXLOW(i,binx) << " " << usbsuw->GetYLOW(i,biny) << " " << usbsuw->GetZLOW(i,binz) << "  " <<
		(usbsuw->GetXHIGH(i,binx)+usbsuw->GetXLOW(i,binx))/2.0 << " " << (usbsuw->GetYHIGH(i,biny)+usbsuw->GetYLOW(i,biny))/2.0 << " " << (usbsuw->GetZHIGH(i,binz)+usbsuw->GetZLOW(i,binz))/2.0 << "  " <<
		usbsuw->GetScored(i,binx+1,biny+1,binz+1) << " " << usbsuw->GetError(i, binx+1,biny+1,binz+1) << std::endl;
	    }
	  }
	}
    } else {
      return help();
    }
    i++;
  }
  
  delete usbsuw;
  
  return 0;
}
  
