// this file is for filter algorithm
// no.1 is Moving Average filter


#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TH1D.h"

using std::cout; using std::endl; using std::pow;

class TGraph;
class TTree;

const unsigned int MaxTraceN = 10000;

class Analysis{
 public:
  Analysis() = default;
  Analysis(const char *infile);
  virtual ~Analysis();

 public:			/* set */
  void setMoudleADCMsps(int fre);
  void setPulsePolarity(bool p){
    plusepolarity = p;
  }
  void setBaselinCalPonits(int num){
    baselinecalpoints = num;
  }
  void setMAFilterParN(int num);
  void setMGFilterParN(int num);

  void setFastFilterPar(int ffl, int ffg, int ffastthresh);
  
 public:			/* process */
  void MoveBaseline();
  void MAFilter(double *lft, double *rgt);
  void MGFilter(double *lft, double *rgt);
  
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
  TGraph *orig;
  TH1D *h;
  
 private:
  int adcmsps;	      	/*  sampling frequency */
  bool plusepolarity;
  int baselinecalpoints;
  double baseline;
  ULong64_t nevt;
  UShort_t ltra;

 private:
  int mafiltern;
  int mgfiltern;

  int fastfilterrange;
  int fl, fg;
  int fastthresh;
  int slowfilterrange;
  int sl, sg;
  
 private:
  UShort_t y[MaxTraceN];
  UShort_t x[MaxTraceN];
  Double_t yy[MaxTraceN];
  Double_t xx[MaxTraceN];
  
  Double_t yma[MaxTraceN];
};


#endif
