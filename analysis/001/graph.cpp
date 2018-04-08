// root -l graph.cpp


#include "TMath.h"
#include "TCanvas.h"
#include "TGraph.h"

// hf1
Double_t douexp(Double_t *x, Double_t *p)
{
  if((x[0] > p[2])  && (p[0] > 0) && (p[1] > 0)){
    return (1/(p[0]-p[1]))*(TMath::Exp((p[2]-x[0])/p[0]) - TMath::Exp((p[2]-x[0])/p[1]));
  }
  else{
    return 0;
  }
}

// hf2
Double_t Maxwell(Double_t *x, Double_t *par)
{
  if(x[0] > par[1] && par[2] > 0 && par[0] > 0){
    return par[0]*(x[0]-par[1])/par[2]*TMath::Exp(-(x[0]-par[1])/par[2]); 
  }
  else{
    return 0.;
  }
}

Double_t DeuxMaxwell(Double_t *x, Double_t *par)
{
  return Maxwell(x,par)+Maxwell(x,&par[3]);
}

void graph()
{
  double x[100], y[100];
  TCanvas *c = new TCanvas("c", " ", 1200, 400);
  c->Divide(2, 1);
  

  TF1 *hf1 = new TF1("hf1", douexp, 0, 3000, 3);
  hf1->SetParameters(10, 2000, 1000);
  c->cd(1);
  hf1->Draw();
  hf1->SetLineWidth(3);

  TF1 *hf2 = new TF1("hf2", DeuxMaxwell, 0, 3000, 6);
  hf2->SetParameters(1000, 1000, 400, 1000, 1000, 400);
  hf2->SetLineColor(kGreen);
  hf2->SetLineWidth(3);
  c->cd(2);
  hf2->Draw();
}
