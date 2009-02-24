#ifndef _Mgdraw_h_
#define _Mgdraw_h_

#include "Base.h"
#include <vector>

namespace ReadFluka {
  class Track {           // continuos energy depotition
  public:
    int fn;               // number of trajectory segments
    int fm;               // number of energy deposition events along the trajectory
    int fj;               // particle type
    float fe;             // total energy of the particle (mass+kinetic)
    float fw;             // particle weight
    unsigned int fl;      // generation number
    
    float *fx;            // coordinates defining
    float *fy;            // the upstream end of the (i+1)th segment
    float *fz;            // last point -> end of the trajectory
    
    float *fd;            // energy deposition in the j-th deposition event along the trajectory
    float  fc;            // total curved path
    
    Track(int n, int m, int j, float e, float w, unsigned int l=0);
    virtual ~Track();

    inline int GetN() const { return fn; }
    inline int GetM() const { return fm; }
    inline int GetID() const { return fj; }
    
    void SetXYZ(int i, float x, float y, float z) {fx[i] = x; fy[i] = y, fz[i] = z;}
    inline float GetX(int i) const {return fx[i]; }
    inline float GetY(int i) const {return fy[i]; }
    inline float GetZ(int i) const {return fz[i]; }

    inline void SetEd(int j, float d) {fd[j] = d;}
    inline float GetEd(int j) const {return (j<fm) ? fd[j] : 0.0;}
    inline void SetPath(float c) {fc = c;}
    inline float GetPath() const {return fc;}

    inline unsigned int GetGen() const {return fl;}

    bool Check() const;
    void Print() const;
  };

  class Point { // point energy deposition
    int fcode;  // type of point event giving rise to energy deposition
    int fid;     // particle type
    float fe;   // total energy of the particle (mass+kinetic)
    float fw;   // particle weight
    unsigned int   fl; // generation number
    
    float fx;    // coordinates of the energy deposition point
    float fy;
    float fz;
    float frull; // amount of energy deposited
  public:
    Point(int code, int jtrack, float etrack, float wtrack, unsigned int ltrack=0);
    virtual ~Point() {;}

    inline int GetCode() const {return fcode;}
    inline int GetID() const {return fid;}
    inline float GetE() const {return fe;}
    inline float GetW() const {return fw;}

    inline void SetGen(unsigned int l) {fl = l;}
    unsigned int GetGen() const {return fl;}

    inline void SetX(float x) {fx = x;}
    inline float GetX() const {return fx;}
    inline void SetY(float y) {fy = y;}
    inline float GetY() const {return fy;}
    inline void SetZ(float z) {fz = z;}
    inline float GetZ() const {return fz;}

    inline void SetEd(float rull) {frull = rull;}
    inline float GetEd() const {return frull;}

    bool Check() const;
    void Print() const;
  };

  class Source { // source particles
    int fncase; //  number of primaries treated so far (including the current one)
    int fnpflka; // number of particles in the stack
    int fnstmax; // maximum number of particles in the stack so far
    float ftkesum; // total kin energy of the primaries of a user written SOURCE
    float fw; // total weight of the primaries handled so far
    int   *filoflk; // type of the i-th stack particle
    float *fetot; // total energy of i-th stack particle
    float *fwtflk; // weight
    float *fx; // coordinates
    float *fy;
    float *fz;
    float *fcosX; // direction cosines
    float *fcosY;
    float *fcosZ;
  public:
    Source(int ncase, int npflka, int nstmax, float tkesum, float weipri);
    virtual ~Source();

    inline int GetNcase() const { return fncase; }
    inline int GetNpflka() const { return fnpflka; }
    inline int GetNstmax() const { return fnstmax; }
    inline float GetTkesum() const { return ftkesum; }
    inline float GetWeipri() const { return fw; }

    inline void SetID(int i, int id) {filoflk[i] = id;}
    inline int GetID(int i) const {return filoflk[i];}
    inline void SetE(int i, float e) {fetot[i] = e;}
    inline float GetE(int i) const {return fetot[i];}
    inline void SetW(int i, float w) {fwtflk[i] = w;}
    inline float GetW(int i) const {return fwtflk[i];}

    inline void SetX(int i, float x) {fx[i] = x;}
    inline float GetX(int i) const {return fx[i];}

    inline void SetY(int i, float y) {fy[i] = y;}
    inline float GetY(int i) const {return fy[i];}    

    inline void SetZ(int i, float z) {fz[i] = z;}
    inline float GetZ(int i) const {return fz[i];}

    inline void SetTX(int i, float cosx) {fcosX[i] = cosx;}
    inline float GetTX(int i) const {return fcosX[i];}

    inline void SetTY(int i, float cosy) {fcosY[i] = cosy;}
    inline float GetTY(int i) const {return fcosY[i];}    

    inline void SetTZ(int i, float cosz) {fcosZ[i] = cosz;}
    inline float GetTZ(int i) const {return fcosZ[i];}

    bool Check() const;
    void Print() const;
  };
  
  class Mgdraw : public Base {
    int fCASE;   // flag to read one of 3 entry types and also the first variable of each entry.
    int fType; // event type
    std::vector<Source*> fSource;
    std::vector<Track*> fTracks;
    std::vector<Point*> fPoints;
    
  public:
    Mgdraw(const char *fname);
    virtual ~Mgdraw();
    
    int ReadEvent(int type=0);
    int ReadTrack();
    int ReadEnergy();
    int ReadSource();

    std::vector<Source*> GetSource() const {return fSource;}
    std::vector<Track*>  GetTracks() const {return fTracks;}
    std::vector<Point*>  GetPoints() const {return fPoints;}
    
    void Print(const char *option="") const;
  };
}

#endif
