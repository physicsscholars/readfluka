#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <Riostream.h>

static TFile *file;
static TTree *tree;

struct PRIMARY
{
	Float_t E0;
} primary;

extern "C"
{
	void write_open_()
	{
		file = new TFile("mgdraw.root", "recreate", "Example 01");
		tree = new TTree("T", "Kinetic energy of the projectile");
		tree->Branch("primary", &primary, "E0/F");
	}
}

extern "C"
{
	void write_primary_(double *dTKEFLK)
	{
		primary.E0  = (Float_t)*dTKEFLK;
	}
}

extern "C"
{
	void write_close_()
	{
		file = tree->GetCurrentFile();
		tree->Write("", TObject::kOverwrite);
		file->Write();
		tree->Print();
		file->Close();
	}
}
