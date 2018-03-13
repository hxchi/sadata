#include "analysis.h"

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


Analysis::Analysis(const char *infile)
{
  TFile *rf = new TFile(infile, "READ");
  orit = (TTree*)rf->Get("t"); // how to get a tree from root file
  
  nevt = orit->GetEntries();

  orit->SetBranchAddress("ltra", &ltra);
  orit->SetBranchAddress("data", y);
  orit->SetBranchAddress("x", x);

  orig = new TGraph();

  orit->GetEntry(1);
  // cout << "ltra = " << ltra << endl;
  // for(int i = 0; i < ltra; i++){
  //   cout << x[i] << " " << y[i] << endl;
  // }
}

Analysis::~Analysis()		// make draw useless
{
   if(orig != NULL){
    delete orig;
   }

  if(orit != NULL){
    delete orit;
  }
}

void Analysis::setMoudleADCMsps(int fre)
{
  adcmsps = fre;
}

void Analysis::setMAFilterParN(int num)
{
  mafiltern = num;
}

void Analysis::setMGFilterParN(int num)
{
  mgfiltern = num;
}

void Analysis::setFastFilterPar(int ffl, int ffg, int ffastthresh)
{
  fastfilterrange = 0;

  if(adcmsps == 100){
    fl = (unsigned int)round(ffl * (double)adcmsps / pow(2.0, (double)fastfilterrange));
    fg = (unsigned int)round(ffg * (double)adcmsps / pow(2.0, (double)fastfilterrange));
  }
  
  else{
    if(adcmsps == 250){
      fl = (unsigned int)round(ffl * (double)(adcmsps/2) / pow(2.0, (double)fastfilterrange));
      fg = (unsigned int)round(ffg * (double)(adcmsps/2) / pow(2.0, (double)fastfilterrange));
    }
    else{
      if(adcmsps == 500){
	fl = (unsigned int)round(ffl * (double)(adcmsps/5) / pow(2.0, (double)fastfilterrange));
	fg = (unsigned int)round(ffg * (double)(adcmsps/5) / pow(2.0, (double)fastfilterrange));
      }
    }
  }

  fastthresh = ffastthresh;

  cout << "fl = " << fl << endl;
  cout << "fg = " << fg << endl;
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
