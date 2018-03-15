// this file is for filter algorithm
// no.1 is Moving Average filter


#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>

#include "TApplication.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TBenchmark.h"
#include "TBranch.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TColor.h"
#include "TCutG.h"
#include "TDatime.h"
#include "TError.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TFormula.h"
#include "TGaxis.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TList.h"
#include "TLorentzVector.h"
#include "TMarker.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TMultiGraph.h"
#include "TNtuple.h"
#include "TObject.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TRandom.h"
#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TRint.h"
#include "TROOT.h"
#include "TSlider.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTimer.h"
#include "TTimeStamp.h"
#include "TTree.h"
#include "TVector3.h"
#include "TVectorD.h"

using std::cout; using std::endl; using std::pow;

class TGraph;
class TTree;
class TH1D;
class TCanvas;

const unsigned int MaxTraceN = 10000;

class Analysis{
 public:
  Analysis() = default;
  Analysis(const char *infile);
  virtual ~Analysis();

 public:			/* set */
  void setMoudleADCMsps(int fre){
    adcmsps = fre;
  }
  void setPulsePolarity(bool p){
    plusepolarity = p;
  }
  void setBaselinCalPonits(int num){
    baselinecalpoints = num;
  }
  void setMAFilterParN(int num){
    mafiltern = num;
  }
  void setMGFilterParN(int num){
    mgfiltern = num;
  }
  void setMWDFilterParN(int num){
    mwdfiltern = num;
  }

  
 public:			/* process */
  void MoveBaseline();
  void getAmplitude(int lft, int rgt); /* from point lft to point rgt calculate amplitude, this calculate right after eliminating baseline */
  void MAFilter(double *lft, double *rgt);
  void MGFilter(double *lft, double *rgt);

  void fitWave();
  void MWDFilter(double *lft, double *rgt);

  void getRiseStart();
  
 public:			/* get par */
  void getNevt();
  
  int getMAFilterParN(){
    return mafiltern;
  }

  int getMGFilterParN(){
    return mgfiltern;
  }

  void getLtra(int &tracelength);
  void getOriWave(int *lft, int *rgt);
  
 private:
  TTree *orit;
  TCanvas *hc;
  TGraph *orig;
  TH1D *h;
  
 private:
  int adcmsps;	      	/*  sampling frequency */
  bool plusepolarity;
  int baselinecalpoints;
  double baseline;
  double amplitude;
  
  ULong64_t nevt;
  UShort_t ltra;

 private:
  int mafiltern;
  int mgfiltern;
  int mwdfiltern;
  Double_t par[6];
  Double_t maxvalueofmgfilter;
  int maxpointofmgfilter;
  double risestart;		/* unit is point */
  
 private:
  UShort_t y[MaxTraceN];
  UShort_t x[MaxTraceN];	/* orignal data */
  Double_t yy[MaxTraceN];
  Double_t xx[MaxTraceN];	/* data after move baseline */
  

};


#endif
