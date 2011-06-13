draw(const char *fname="ex03_001.root")
{
  TFile *file = TFile::Open(fname);

  TCanvas *c1 = new TCanvas("c1", file->GetTitle());
  c1->Divide(2, 1);
  
  c1->cd(1);
  EVENTDAT->Draw("Ed[2]");

  c1->cd(2);
  usrbin->Project3D("xy")->Draw("col");
}
