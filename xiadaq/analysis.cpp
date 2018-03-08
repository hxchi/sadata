#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TROOT.h"
#include "TBenchmark.h"

const Int_t MaxTraceN = 10000;

ULong64_t *GetChannel(const Short_t tmpch, ULong64_t arrch[16]);

void analysis()
{
  TBenchmark *benchmark = new TBenchmark();
  TFile *rf = new TFile("rsf.root");
  TTree *t = (TTree*)rf->Get("t");
  TGraph *g = new TGraph();

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
  Double_t base;

  UInt_t qs[8];
  ULong64_t ets;
  UShort_t data[MaxTraceN];
  UShort_t x[MaxTraceN];		// for tgraph x alias	

  t->SetBranchAddress("pileup", &pileup);
  t->SetBranchAddress("outofr", &outofr);
  
  t->SetBranchAddress("sid", &sid);
  t->SetBranchAddress("cid", &cid);
  t->SetBranchAddress("ch", &ch);
  t->SetBranchAddress("evte", &evte);
  t->SetBranchAddress("ts", &ts);
  t->SetBranchAddress("ets", &ets);
  
  t->SetBranchAddress("cfd", &cfd);
  t->SetBranchAddress("cfdft", &cfdft);
  
  t->SetBranchAddress("trae", &trae);
  t->SetBranchAddress("leae", &leae);
  t->SetBranchAddress("gape", &gape);
  t->SetBranchAddress("base", &base);

  t->SetBranchAddress("qs", qs);
  
  t->SetBranchAddress("ltra", &ltra);
  t->SetBranchAddress("data", data);
  t->SetBranchAddress("x", x);
  
  ULong64_t nevt = t->GetEntries();
  cout << nevt << endl;

  int cnt = 0;
  ULong64_t arrch[16] = {0};

  benchmark->Start("analysis");
  for(int i = 0; i < nevt; i++){
    t->GetEntry(i);
    if(ltra > 0){
      // for trace
      // cout << "channel is: " << ch << endl;
      // cout << "ltra is " << ltra << endl;
      // for(int j = 0 ; j < ltra; j++){
      // 	g->SetPoint(j, x[j], data[j]);
      // 	cout << x[j] << " " <<data[j] << endl;
      // }
      // if(cnt == 0){
      // 	g->Draw();
      // }
      // else{
      // 	g->Draw("same");
      // }
      // cnt++;

      // for qdc value
      // for(int j = 0; j < 8; j++){
      // 	cout << qs[j] << endl;
      // }
    }
    // for channel used here
    GetChannel(ch, arrch);
    cnt++;
    
    // if(cnt == 1){
    //   break;
    // }
  }

  for(int i = 0; i < 16; i++){
    if(arrch[i] > 0){
      cout << "channel no." << i << ".  total event numbers is "
	   << arrch[i] << endl; 
    }
  }
  benchmark->Show("analysis");
}


// function
ULong64_t *GetChannel(const Short_t tmpch, ULong64_t arrch[16])
{
  // cout << tmpch << endl;
  arrch[tmpch]++;
  return arrch;
}



