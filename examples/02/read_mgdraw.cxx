#include "Mgdraw.h"
#include <iostream>
#include <iomanip>

using namespace std;

void print_src(ReadFluka::Mgdraw *m, unsigned int gen)
{
  vector<ReadFluka::Source*> sources = m->GetSource();
  vector<ReadFluka::Source*>::iterator sources_iter = sources.begin();
  
  sources[0]->Print();
}

void print_point(ReadFluka::Mgdraw *m, unsigned int gen_max)
{
  vector<ReadFluka::Point*> points = m->GetPoints();
  for (vector<ReadFluka::Point*>::const_iterator it = points.begin(); it != points.end(); ++it) {
    ReadFluka::Point *p = *it;
    if (p->GetGen() <= gen_max) {
      //      if (p->GetCode()<20) p->Print();
      p->Print();
    }
  }
}

void print_track(ReadFluka::Mgdraw *m, unsigned int gen_max)
{
  vector<ReadFluka::Track*> tracks = m->GetTracks();
  for (vector<ReadFluka::Track*>::const_iterator it = tracks.begin(); it != tracks.end(); ++it) {
    ReadFluka::Track *p = *it;
    if (p->GetGen() <= gen_max) p->Print();
  }
}

int main(int argc, char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;
  ReadFluka::Mgdraw *mgdraw = new ReadFluka::Mgdraw(argv[1]);
  
  mgdraw->ReadEvent(1);

  //  print_track(mgdraw, 2);
  //  print_point(mgdraw, 40);
  
  delete mgdraw;
  return 0;
}

