#include "analysis.h"


int test()
{
  int y[MaxTraceN];		// for orignal filter
  int x[MaxTraceN];

  double yma[MaxTraceN];	// for MA filter
  double xma[MaxTraceN];

  double ymg[MaxTraceN];	// for MG filter
  double xmg[MaxTraceN];

  double ymwd[MaxTraceN];	// for MWD filter
  double xmwd[MaxTraceN];
  
  Analysis *ana = new Analysis("../getxiadata/r.root");

  ana->setMoudleADCMsps(100);
  ana->setPulsePolarity(1);	// positive
  ana->setBaselinCalPonits(10);
  ana->MoveBaseline();
  
  int Ltra = 0;
  ana->getLtra(Ltra);
  ana->getOriWave(x, y);
  ana->fitWave(1);
  
  TCanvas *c = new TCanvas("c", "", 1200, 700);
  c->Divide(2,2);
  TGraph *hg = new TGraph();
  for(int i = 0; i < Ltra; i++){
    hg->SetPoint(i, x[i], y[i]);
  }
  c->cd(1);
  hg->Draw();

  TGraph *hmag = new TGraph();
  ana->setMAFilterParN(1);	// noise is kind of good here, so do not need MAfilter
  ana->MAFilter(xma, yma);
  for(int i = 0; i < Ltra; i++){
    hmag->SetPoint(i, xma[i], yma[i]);
  }
  c->cd(2);
  hmag->Draw();
  hmag->SetLineColor(4);
  hmag->SetMarkerColor(4);

  TGraph *hmgg = new TGraph();
  ana->setMGFilterParN(160);
  ana->MGFilter(xmg, ymg);
  for(int i = 0; i < Ltra; i++){
    hmgg->SetPoint(i, xmg[i], ymg[i]);
  }
  c->cd(3);
  hmgg->Draw();

  ana->MWDFilter(xmwd, ymwd);
  TGraph *hmwdg = new TGraph();
  ana->setMWDFilterParN(100);
  ana->MWDFilter(xmwd, ymwd);
  for(int i = 0; i < Ltra; i++){
    hmwdg->SetPoint(i, xmwd[i], ymwd[i]);
  }
  c->cd(4);
  hmwdg->Draw();

  
  return 0;
}


// this two function here is used to draw picture
// while compiling with g++
void StandaloneApplication(int argc, char** argv) {
  test();			// the real function used in here
}

int main(int argc, char** argv)
{
  TApplication app("ROOT Application", &argc, argv);
  StandaloneApplication(app.Argc(), app.Argv());
  app.Run();
  return 0;
}

