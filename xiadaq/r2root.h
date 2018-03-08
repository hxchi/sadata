// for get rootfile

#ifndef R2ROOT_H_
#define R2ROOT_H_

#include "xia.h"

#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBenchmark.h"

const int MaxBoard = 24;	/* maximum value of board number */

class R2root
{
 public:
  R2root() = default;
  R2root(const char *infileN, const char *outfileN);
  ~R2root();

  void Process();
  double IeeeBin2Float(unsigned int ieenum);

 private:
  bool IsFileExit(const char *infileN);
  void ClearOpt();
	
 private:
  Eventer *rawevt;
  bool havedata;

  Int_t nevt;

  TFile *rf;
  TTree *t;
      
  TBenchmark *benchmark;
	
 private:
  Short_t ch;
  Short_t sid;
  Short_t cid;
  Bool_t pileup;

  ULong64_t ts;

  UShort_t cfd;
  Bool_t cfdft;

  UShort_t evte;
  UShort_t ltra;
  Bool_t outofr;

  Bool_t esumf;
  UInt_t trae;
  UInt_t leae;
  UInt_t gape;
  UInt_t base;

  UInt_t qs[8];
  ULong64_t ets;
  UShort_t data[MaxTraceN];
  UShort_t x[MaxTraceN];		// for tgraph x alias	
};


#endif
