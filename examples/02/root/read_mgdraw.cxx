#include "Mgdraw.h"
#include <iostream>
#include <iomanip>
#include <TLine.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TStyle.h>

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
    
    if (p->GetGen()<=gen_max) {
      if ((p->GetCode()==11) || (p->GetCode()==30))
	p->Print();
    }
  }
}

void print_track(ReadFluka::Mgdraw *m, unsigned int gen_max)
{
  vector<ReadFluka::Track*> tracks = m->GetTracks();
  for (vector<ReadFluka::Track*>::const_iterator it = tracks.begin(); it != tracks.end(); ++it) {
    ReadFluka::Track *t = *it;
    if (t->GetGen() <= gen_max) t->Print();
  }
}

void draw(ReadFluka::Mgdraw *m, unsigned int gen_max)
{
  TCanvas *c1 = new TCanvas("c1", "", -1);
  //  c1->Range(-10, -10, 10, 25);
  c1->Range(10, -10, 24, 17);

  const Int_t colNum=40;
  Int_t my_palette[colNum];
  for(int i=0;i<colNum;i++) my_palette[i]=61+i;
  gStyle->SetPalette(colNum,my_palette);

  
  vector<ReadFluka::Track*> tracks = m->GetTracks();
  for (vector<ReadFluka::Track*>::const_iterator it = tracks.begin(); it != tracks.end(); ++it) {
    ReadFluka::Track *t = *it;

    TLine *l = new TLine;
    float x1, y1, x2, y2;

    if (t->GetGen() <= gen_max) { // draw tracks up to gen_max
      for (int i=0; i<t->GetN()+1; i++) {
	if (i % 2 == 0) {
	  x1 = t->GetX(i);	  y1 = t->GetZ(i);
	} else {
	  x2 = t->GetX(i);	  y2 = t->GetZ(i);
	  //	  cout << x1 << " " << y1 << "\t" << x2 << " " << y2 << "\t\t" << t->GetID() << endl;
	  //	  cout << t->GetID() << " ";

	  l->SetLineColor(kBlack); l->SetLineStyle(kSolid); l->SetLineWidth(1);

	  //	  if (t->GetID() == 3) l->SetLineColor(kGreen);
	  if (t->GetID() == 7) l->SetLineStyle(kDotted);
	  if (t->GetID() == 1) l->SetLineWidth(2);
	  if ( (t->GetID() == 13) || (t->GetID() == 14) ) l->SetLineStyle(kDashed); // pion
	  l->SetLineColor(t->GetGen());

	  l->DrawLine(x1, y1, x2, y2);
	}
      }
      //      cout << endl;
    }
  }
  c1->Print("c1.C");
  c1->Print("c1.png");
}
 
int main(int argc, char **argv)
{
  ReadFluka::Base::gVerbose = ReadFluka::kPRINT_NOTHING;//kPRINT_SCORED;
  ReadFluka::Mgdraw *mgdraw = new ReadFluka::Mgdraw(argv[1]);
  mgdraw->SetGenMax(200);
  mgdraw->SetEmin(5.0);
  mgdraw->ReadEvent(1);

  //  print_track(mgdraw, 1);
  
  cerr << "palette: " << gStyle->GetColorPalette(0) << endl;
  
  draw(mgdraw, 7);
  print_src(mgdraw, 20);
  print_point(mgdraw, 20);
  print_track(mgdraw, 20);
  

  delete mgdraw;
  return 0;
}

