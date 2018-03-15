#include "analysis.h"
#include "TMath.h"

// fuction used in this file
// no.1
double getdn(int n, int np)
{
  double dn = 0;
  if(n == 0){
    return 0;
  }

  if(n > 0){
    for(int i = (1-n); i <= n; i++){
      dn += 2.0/(i+np)/(2*np+1)/(2*np);
    }
    return dn;
  }
  else{
    return -getdn(-n, np);
  }
}

// no.2
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

// no.3
// p0: fall time, unit is number of points
// p1: rise time, unit is number of points
// p2: event start time, unit is number of points
// p3: cofefficient
// p4: y axias translation
Double_t douexp(Double_t *x, Double_t *p)
{
  if((x[0] > p[2])  && (p[0] > 0) && (p[1] > 0)){
    return (p[3]/(p[0]-p[1]))*(TMath::Exp((p[2]-x[0])/p[0]) - TMath::Exp((p[2]-x[0])/p[1])) + p[4];
  }
  else{
    return 0;
  }
}


// member function of class Analysis
Analysis::Analysis(const char *infile)
{
  TFile *rf = new TFile(infile, "READ");
  orit = (TTree*)rf->Get("t"); // how to get a tree from root file
  
  nevt = orit->GetEntries();

  orit->SetBranchAddress("ltra", &ltra);
  orit->SetBranchAddress("data", y);
  orit->SetBranchAddress("x", x);

  orig = new TGraph();
  hc = new TCanvas("hc", "", 1200, 700);
  hc->Divide(2, 2);
  
  orit->GetEntry(1);
  // cout << "ltra = " << ltra << endl;
  // for(int i = 0; i < ltra; i++){
  //   cout << x[i] << " " << y[i] << endl;
  // }
}

Analysis::~Analysis()		
{
   if(orig != NULL){
    delete orig;
   }

  if(orit != NULL){
    delete orit;
  }

  if(hc != NULL){
    delete hc;
  }
}



void Analysis::MoveBaseline()
{
  baseline = 0;
  for(int i = 0; i < baselinecalpoints; i++){
    // cout << y[i] << endl;
    baseline += y[i];
  }
  baseline /= baselinecalpoints;
  // cout << baseline << endl;
  
  if(plusepolarity){
    for(int i = 0; i < ltra; i++){
      yy[i] = y[i] - baseline;
      xx[i] = x[i];
      // cout << xx[i] << " " << yy[i] << endl;
    }
  }
  else{
    for(int i = 0; i < ltra; i++){
      yy[i] = baseline - y[i];
      xx[i] = x[i];
    }
  }

  for(int i = 0; i < ltra; i++){
    orig->SetPoint(i, xx[i], yy[i]);
  }
  orig->Draw();
}


void Analysis::MAFilter(double *lft, double *rgt)
{
  for(int i = 0; i < mafiltern; i++){
    rgt[i] = yy[i];
    lft[i] = xx[i];
  }

  for(int i = mafiltern; i < ltra; i++){
    rgt[i] = rgt[i-1] + (yy[i] - yy[i-mafiltern])/mafiltern;
    lft[i] = xx[i];
  }
}


void Analysis::MGFilter(double *lft, double *rgt)
{
  double dn = 0;
  for(int i = 0; i < mgfiltern; i++){
    lft[i] = i;
    rgt[i] = 0;
  }
  
  for(int i = mgfiltern; i < (ltra-mgfiltern); i++){
    for(int j = -mgfiltern; j <= mgfiltern; j++){
      dn = getdn(j, mgfiltern);
      // cout << "dn = " << dn << endl;
      rgt[i] += dn*yy[i-j];
    }
    lft[i] = i;
    // cout << xx[i] << " " << yy[i] << endl;
    // cout << lft[i] << " " << rgt[i] << endl;
  }

  for(int i = (ltra-mgfiltern); i < ltra; i++){
    lft[i] = i;
    rgt[i] = rgt[ltra-mgfiltern-1];
  }
}

void Analysis::fitWave(int ci)
{
 // fetch the TF1 named "deuxmax"
 TF1 *hf1 =(TF1 *)gROOT->FindObject("deuxmax");
 if(!hf1){
   hf1 = new TF1("hf1", DeuxMaxwell, 0, 3000, 6);
   // deuxmw->SetParNames("C_{1}","B_{1}","T_{1}","C_{2}","B_{2}","T_{2}");
 }
 hf1->SetParameters(1000, 1000, 400, 1000, 1000, 400);
 hf1->SetLineColor(kGreen);
 hf1->SetLineWidth(3);

 // fetch the TF1 named "hf2"
 TF1 *hf2 = new TF1("hf2", douexp, 500, 1500, 5);
 hf2->SetParameters(5000, 10, 1000, 100000, 0);
 
 hc->cd(1);
 orig->Fit("hf2");

 hf2->Draw("same");
 hf2->SetLineColor(kBlue);
 hf2->SetLineWidth(4);

 hf2->GetParameters(par);
 for(int i = 0; i < 6; i++){
   cout << "par " << i << " = " << par[i] << endl;
 }

 // draw each singal of double exp
 TGraph *hg1 = new TGraph();
 TGraph *hg2 = new TGraph();

 Double_t hx1[2000], hy1[2000];
 Double_t hx2[2000], hy2[2000];
 for(int i = 0; i < 2000; i++){
   hx1[i] = i;
   hx2[i] = i;
   hy1[i] = (par[3]/(par[0]-par[1])*TMath::Exp((par[2]-i)/par[0]));
   hy2[i] = -(par[3]/(par[0]-par[1])*TMath::Exp((par[2]-i)/par[1]));

   hg1->SetPoint(i, hx1[i], hy1[i]);
   hg2->SetPoint(i, hx2[i], hy2[i]);
 }

 TCanvas *hc1 = new TCanvas("hc1", "par[0]", 600, 400);
 TCanvas *hc2 = new TCanvas("hc2", "par[1]", 600, 400);
 hc1->cd();
 hg1->Draw();
 hc2->cd();
 hg2->Draw();
 
}

void Analysis::MWDFilter(double *lft, double *rgt)
{
  // get u(k)
  Double_t u[MaxTraceN];
  for(int i = 0; i < ltra; i++){
    for(int j = 0; j <= i; j++){
      u[i] += (yy[j]/par[0]);
    }
    u[i] += yy[i];
  }


  for(int i = 0; i < mwdfiltern; i++){
    lft[i] = i;
    rgt[i] = 0;
  }
  for(int i = mwdfiltern; i < ltra; i++){
    lft[i] = i;
    rgt[i] = u[i] - u[i-mwdfiltern];
  }
  // for(int i = 0; i < mwdfiltern; i++){
  //   lft[i] = i;
  //   rgt[i] = 0;
  // }
  // for(Double_t i = mwdfiltern; i < ltra; i++){
  //   lft[(int)i] = i;
  //   for(Double_t j = (i-mwdfiltern); j <= (i-1); j++){
  //     rgt[(int)i] += (douexp(&j, par))/par[1];
  //     cout << "..." << rgt[(int)i] << endl;
  //   }
  //   Double_t k = i - mwdfiltern;
  //   rgt[(int)i] = rgt[(int)i] + douexp(&i, par) - douexp(&k, par);

  //   cout << lft[(int)i] << " " << rgt[(int)i] << endl;
  // }

  
  
}

void Analysis::getNevt()
{
  cout << nevt << endl;
}



void Analysis::getLtra(int &tracelength)
{
  orit->GetEntry(1);
  tracelength = ltra;
  cout << "ltra = " << ltra << endl;
}

void Analysis::getOriWave(int *lft, int *rgt)
{
  for(int i = 0; i < ltra; i++){
    lft[i] = xx[i];
    rgt[i] = yy[i];
  }
}
