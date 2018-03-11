// for auto calibration

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TSpectrum.h"

using std::cout; using std::endl;

TH1F* GetSpec()
{
  FILE *infile;
  if((infile = fopen("Clover01_a.E", "rb")) == NULL){
    printf("cannot open the file.\n");
    return NULL;
  }

  UInt_t header[128] = {0};		// from line 0 to line 31, sum 32*4=128
  UInt_t buff[16384] = {0};

  fread(header, sizeof(UInt_t), 128, infile);
  fread(buff, sizeof(UInt_t), 16384, infile);
  fclose(infile);

  auto c = new TCanvas();
  // TFile *rf = new TFile("clovera_4.root", "recreate");
  Double_t binmin = 0.62;
  Double_t binmax = 16383*0.32 + 0.62;
  Double_t binwidth = (binmax-binmin)/16384;
  cout << binwidth << endl;
  TH1F *h = new TH1F("h", " ", 16384, 0, 16383);
  for(int i = 0; i < 12600; i++){ // cut at 12608 channel
    // h->Fill(i, buff[i]);
    h->SetBinContent(i, buff[i]);
  }
  // h->Draw("HIST");		// HIST means no error
  return h;
  
  // rf->Write();
  // rf->Close();
  
  // test read
  // for(int i = 0; i < 16384; i++){
  //   // printf("%08x\n", buff[i]);
  //   cout << buff[i] << endl;
  // }
}


void calibration()
{
  TH1F *h = new TH1F();
  h = GetSpec();
  h->Draw("HIST");

  // TSpectrum *spec = new TSpectrum();
  // // TH1 *specback = spec->Background((TH1 *)h, 20, "same");
  // // h->Draw("HIST SAME");  // about background

  // Int_t numofpeaks = spec->Search(h);

  // cout << spec->GetNPeaks() << endl; // return numbers of peaks we found
  // cout << numofpeaks << endl;

  // Double_t *xpos = spec->GetPositionX();
  // for(int i = 0; i < 14; i++){
  //   cout << xpos[i] << endl;
  // }

  // h->Draw("HIST");
  
}
