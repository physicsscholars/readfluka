#include "Mgdraw.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits.h>
#include <cstring>

using namespace ReadFluka;

Track::Track(int n, int m, int j, float e, float w, unsigned int l)
{
  fn = n;	fm = m;	fj = j; fe = e; fw = w; fl = l;
  fx = new float[fn+1];
  fy = new float[fn+1];
  fz = new float[fn+1];
  fd = new float[fm];
  fc = 0.0;
}

Track::~Track()
{
  //	std::cerr << "track destructor" << std::endl;
  delete [] fx; fx = 0;
  delete [] fy; fy = 0;
  delete [] fz; fz = 0;
  delete [] fd; fd = 0;
}

bool Track::Check() const
{
  if (fe<0.0) {
    std::cerr << "Track::Check():\t wrong fe" << std::endl;
    exit(WRONG_FORMAT);
  }

  if (fw<0.0) {
    std::cerr << "Track::Check():\t wrong fw" << std::endl;
    exit(WRONG_FORMAT);
  }

  for (int i=0; i<fm; i++) {
    if (fd[i]<0.0) {
      std::cerr << "Track::Check():\t wrong fd["<<i<<"]" << std::endl;
      exit(WRONG_FORMAT);
    }
  }

  return true;
}

void Track::Print(const char *option) const 
{
  /*
    option == "first_last" => print only first and last point of the track
   */

  std::cout << "*** continuous energy deposition (track)" << std::endl;

  std::cout<<"\t ntrack,mtrack,jtrack,etrack,wtrack,gen: "<<fn<<" "<<fm << " " <<fj << " " <<fe << " " <<fw << " " << fl << std::endl;

  if (!strcmp(option, "first_last")) {
    for (int i=0; i<fn+1; i+=fn) {
      std::cout << "\t\t i,x,y,z: " << i << " " << fx[i] << " " << fy[i] << " " << fz[i] << std::endl;
    }
  } else {
    for (int i=0; i<fn+1; i++) {
      std::cout << "\t\t i,x,y,z: " << i << " " << fx[i] << " " << fy[i] << " " << fz[i] << std::endl;
    }
  }

  for (int j=0; j<fm; j++) {
    std::cout << "\t\t j,dtrack: " << j << " " << fd[j] << std::endl;
  }
  std::cout << "\t\t path: " << GetPath() << std::endl;
  
}


Point::Point(int code, int jtrack, float etrack, float wtrack, unsigned int ltrack)
{
  fcode = code;
  fid = jtrack;
  fe = etrack;
  fw = wtrack;
  fl = ltrack;
  fx = fy = fz = frull = 0.0;
}

bool Point::Check() const
{
  if (fw<0.0) {
    std::cerr << "Point::Check():\t wrong fw" << std::endl;
    //    exit(WRONG_FORMAT);
  }

  if (fe<0.0) {
    std::cerr << "Point::Check():\t wrong fe" << std::endl;
    //    exit(WRONG_FORMAT);
  }

  if (frull<0.0) {
    std::cerr << "Point::Check():\t wrong frull: " << frull << std::endl;
    //    exit(WRONG_FORMAT);
  }  
  return true;
}

void Point::Print() const
{
  std::cout << "*** point energy deposition" << std::endl;
  std::cout << "\t icode,jtrack,e,w,gen: "<< GetCode() << "\t" << GetID() << " " << GetE() <<"\t"<< GetW() << " " << GetGen() << std::endl;
  std::cout << "\t x,y,z,rull: " << GetX() << " " << GetY() << " " << GetZ() << "\t" << GetEd() << std::endl;
}

Source::Source(int ncase, int npflka, int nstmax, float tkesum, float weipri)
{
  fncase = ncase; fnpflka = npflka; fnstmax = nstmax; ftkesum = tkesum; fw = weipri;
  filoflk = new int[fnpflka];
  fetot = new float[fnpflka];
  fwtflk = new float[fnpflka];
  fx = new float[fnpflka];
  fy = new float[fnpflka];
  fz = new float[fnpflka];
  fcosX = new float[fnpflka];
  fcosY = new float[fnpflka];
  fcosZ = new float[fnpflka];
}

Source::~Source()
{
  delete [] filoflk; filoflk = 0;
  delete [] fetot; fetot = 0;
  delete [] fwtflk; fwtflk = 0;
  delete [] fx; fx = 0;
  delete [] fy; fy = 0;
  delete [] fz; fz = 0;
  delete [] fcosX; fcosX = 0;
  delete [] fcosY; fcosY = 0;
  delete [] fcosZ; fcosZ = 0;
}

bool Source::Check() const
{
  if (fw<0.0) {
    std::cerr << "Source::Check():\t wrong fw" << std::endl;
    exit(WRONG_FORMAT);
  }

  for (int i=0; i<fnpflka; i++) {
    if (GetE(i)<0.0) {
      std::cerr << "Source::Check():\t wrong energy for primary " << i << std::endl;
      exit(WRONG_FORMAT);
    }
    if (GetW(i)<0.0) {
      std::cerr << "Source::Check():\t wrong weight for primary " << i << std::endl;
      exit(WRONG_FORMAT);
    }
    if (GetTX(i)>1.0) {
      std::cerr << "Source::Check():\t wrong cosX for primary " << i << std::endl;
      exit(WRONG_FORMAT);
    }
    if (GetTY(i)>1.0) {
      std::cerr << "Source::Check():\t wrong cosY for primary " << i << std::endl;
      exit(WRONG_FORMAT);
    }
    if (GetTZ(i)>1.0) {
      std::cerr << "Source::Check():\t wrong cosZ for primary " << i << std::endl;
      exit(WRONG_FORMAT);
    }
  }
  return true;
}

void Source::Print() const
{
  std::cout << "*** source particle" << std::endl;
  //	std::cout << "icase: " << icase << "\t- type of this record" << std::endl;
  std::cout << "\tncase: " << GetNcase() << "\t- number of primaries treated so far (including the current one)" << std::endl;
  std::cout << "\tnpflka: " << GetNpflka() << "\t- number of particles in the stack" << std::endl;
  std::cout << "\tnstmax: " << GetNstmax() << std::endl;
  std::cout <<"\ttkesum: " << GetTkesum() << "\t- total kinetic energy of the primaries of a user written SOURCE" << std::endl;
  std::cout << "\tweipri: " << GetWeipri() << "\t- total weight of the primaries handled so far" << std::endl;

  for (int i=0; i<GetNpflka(); i++) {
    std::cout << "\thit: " << GetID(i) << " " << GetE(i) << " " << GetW(i) << "\t";
    std::cout << GetX(i) << " " << GetY(i) << " " << GetZ(i) << "\t\t";
    std::cout << GetTX(i) << " " << GetTY(i) << " " << GetTZ(i) << std::endl;
  }
}

Mgdraw::Mgdraw(const char *fname) : Base(fname)
{
  // Constructor
  
  //  nevent = ntrack = mtrack = jtrack = icode = 0;
  //  etrack = wtrack = 0.0f;
  if (gVerbose>=kPRINT_SCORED) std::cerr << "Mgdraw constructor" << std::endl;
  fType = 0;
  fGenMax = INT_MAX;
  fGenCur = 0;
  fEmin = -1.0; // put all energies by default
}

Mgdraw::~Mgdraw()
{
  // Destructor
  //  std::cerr << "mgdraw destructor" << std::endl;
  
  int n = fTracks.size(); std::cerr << "tracks: " << n << std::endl;
  for (int i=0; i<n; i++) delete fTracks[i];
  fTracks.clear();

  n = fPoints.size(); std::cerr << "points: " << n << std::endl;
  for (int i=0; i<n; i++) delete fPoints[i];
  fPoints.clear();

  n = fSource.size(); std::cerr << "source: " << n << std::endl;
  for (int i=0; i<n; i++) delete fSource[i];
  fSource.clear();  
}

int Mgdraw::ReadEvent(int type)
{
  /*
    Read a MGDRAW event. The meaning of the argument is the following:
    type = 0: read the standard MGDRAW output (this is the default value)
    type = 1: LTRACK added
   */

  if (gVerbose>=kPRINT_SCORED) std::cerr << "Mgdraw::ReadEvent(" << type << ")" << std::endl;
  
  fType = type;
  int len = SizeStart();
  if (len != 20) {
    std::cerr << "Mgdraw::ReadEvent():\t format error" << std::endl;
    return WRONG_FORMAT;
  }
  
  //	int icase;
  fCASE = -1;
  bool first = true;
  
  for(;;) {
    fCASE = ReadInt(); // ndum
    
    if (fCASE<0) {
      ReadSource();
    } else if (fCASE == 0) {
      ReadEnergy();
      first = false;
    } else if (fCASE > 0) {
      ReadTrack(); // 0x4c
    }
    //    if ( (fNCASE>1) && (fCASE<0) ) {
    //      fin->seekg(-4, std::ios::cur);
    //      std::cerr << "event end" << std::endl;
    //      break;
    //    }
    
    /*    if ( (fType == 1) && (GetGenCur() >= GetGenMax()) ) {
      std::cerr << "max generation => break" << std::endl;
      break;
      }*/

    if ((fCASE < 0) && (first == false)) break;
    first = false;
  }
  
  return fCASE;
}

int Mgdraw::ReadSource()
{
  /*
    Read SODRAW (SOurce particle DRAWing) entry   < 0
    WRITE (IODRAW) -NCASE, NPFLKA, NSTMAX, SNGL (TKESUM),
    &                SNGL (WEIPRI)
    
    WRITE (IODRAW) ( IONID,SNGL(TKEFLK(I)+AMNHEA(-IONID)),
    &                    SNGL (WTFLK(I)), SNGL (XFLK (I)),
    &                    SNGL (YFLK (I)), SNGL (ZFLK (I)),
    &                    SNGL (TXFLK(I)), SNGL (TYFLK(I)),
    &                    SNGL (TZFLK(I)), I = 1, NPFLKA )
    
  */

  if (gVerbose>=kPRINT_SCORED) std::cerr << "Mgdraw::ReadSource()" << std::endl;
  
  fNCASE = -fCASE;       // mdum
  int fNPFLKA = ReadInt();   // jdum
  int fNSTMAX = ReadInt();   // maximum number of particles in stack so far
  float fTKESUM = ReadFloat(); // edum
  fWEIPRI = ReadFloat(); // wdum
  Source *s = new Source(-fCASE, fNPFLKA, fNSTMAX, fTKESUM, fWEIPRI);
  
 
  SizeEnd();	
  SizeStart();
  
  for (int i=0; i<s->GetNpflka(); i++) {
    s->SetID(i, ReadInt());
    s->SetE(i, ReadFloat());
    s->SetW(i, ReadFloat());
    
    s->SetX(i, ReadFloat()); s->SetY(i, ReadFloat()); s->SetZ(i,ReadFloat());
    
    s->SetTX(i, ReadFloat()); s->SetTY(i, ReadFloat()); s->SetTZ(i, ReadFloat());
    
  }

  SizeEnd(); SizeStart();

  s->Check();

  fSource.push_back(s);
  
  //  s->Print();

  return fNCASE;
}

int Mgdraw::ReadEnergy()
{
  /*
    Read ENDRAW (ENergy deposition DRAWing) entry   == 0
    WRITE (IODRAW)  0, ICODE, JTRACK, SNGL (ETRACK), SNGL (WTRACK)
    WRITE (IODRAW)  SNGL (XSCO), SNGL (YSCO), SNGL (ZSCO), SNGL (RULL)
  */

  if (gVerbose>=kPRINT_SCORED) std::cerr << "Mgdraw::ReadEnergy()" << std::endl;
  
  Point *p = new Point(ReadInt(), ReadInt(), ReadFloat(), ReadFloat());
  if (fType == 1) p->SetGen(ReadInt());

  SizeEnd(); SizeStart();
  
  p->SetX(ReadFloat());
  p->SetY(ReadFloat());
  p->SetZ(ReadFloat());
  p->SetEd(ReadFloat());

  SizeEnd(); SizeStart();

  p->Check();
  if (p->GetGen() > GetGenCur()) SetGenCur(p->GetGen());
  if ( ((fType==0) || ((fType>0) && (p->GetGen() <= GetGenMax())))  && 
      (p->GetE()>=fEmin)) fPoints.push_back(p);
  else delete p;
  //  p->Print();
  
  return 0;
}

int Mgdraw::ReadTrack()
{
  // Read track                     > 0

  if (gVerbose>=kPRINT_SCORED) std::cerr << "Mgdraw::ReadTrack()" << std::endl;
  
  Track *t = new Track(fCASE, ReadInt(), ReadInt(), ReadFloat(), ReadFloat());
  if (fType == 1) t->fl = ReadInt();
  
  SizeEnd(); SizeStart();
 
  
  for (int i=0; i<t->fn+1; i++) {
    //		t->SetXYZ(i, ReadFloat(), ReadFloat(), ReadFloat()); -> Why x <-> z ?
    t->fx[i] = ReadFloat();
    t->fy[i] = ReadFloat();
    t->fz[i] = ReadFloat();
  }
  
  for (int j=0; j<t->fm; j++) {
    t->SetEd(j, ReadFloat());
  }
  
  t->SetPath(ReadFloat());
 
  SizeEnd(); SizeStart();
  
  t->Check();
  
  if (t->GetGen() > GetGenCur())   SetGenCur(t->GetGen());
  if ((t->GetGen() <= GetGenMax())  && 
      (t->GetE()>=fEmin)) {
    fTracks.push_back(t);
    std::cerr << t->GetID() << " " << t->GetGen() << " " << t->GetE() << std::endl;
  } else delete t;

  return 0;
}


void Mgdraw::Print(const char *option) const
{
  std::cout << "print" << std::endl;
}
