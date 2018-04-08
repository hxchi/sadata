#include "analysis.h"

#include "TROOT.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMath.h"

#include <algorithm>

// fuction used in this file
double getdn(int n, int np);
Double_t Maxwell(Double_t *x, Double_t *par);
Double_t DeuxMaxwell(Double_t *x, Double_t *par);
Double_t Douexp(Double_t *x, Double_t *p);

/* ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... */

// member function of class Analysis
Analysis::Analysis()
{
  // TFile *rf = new TFile(infile, "READ");
  // orit = (TTree*)rf->Get("t"); // how to get a tree from root file
  
  // nevt = orit->GetEntries();

  // orit->SetBranchAddress("ltra", &ltra);
  // orit->SetBranchAddress("data", y);
  // orit->SetBranchAddress("x", x);

  // orig = new TGraph();
  // hc = new TCanvas("hc", "", 600, 400);
  // hc->Divide(2, 2);
  
  // orit->GetEntry(1);
  // int i = 0;
  // while(1){
  //   if(ltra != 0){
  //     break;
  //   }
  //   orit->GetEntry(i);
  //   i++;
  // }

  // cout << "ltra = " << ltra << endl;
  // // for(int i = 0; i < ltra; i++){
  // //   cout << x[i] << " " << y[i] << endl;
  // // }
  
  // hc = new TCanvas("hc", "", 600, 400);
  // hc->Divide(2, 2);
  // hg1 = new TGraph();
  // hg2 = new TGraph();
}

Analysis::~Analysis()		
{
  // delete hc;
  // delete hg1;
  // delete hg2;
}

void Analysis::_setEventData(unsigned short size, unsigned short *data)
{
  Size = size;		// for distinguish
  for(int i = 0; i < Size; i++){
    Data[i] = data[i];
  }

  _dataPrimaryProcess();
}


void Analysis::_setEventData(int size, short *data)
{
  Size = size;		// for distinguish
  for(int i = 0; i < Size; i++){
    Data[i] = data[i];
  }

  _dataPrimaryProcess();
}


void Analysis::_setEventData(int size, int *data)
{
  Size = size;		// for distinguish
  for(int i = 0; i < Size; i++){
    Data[i] = data[i];
  }

  _dataPrimaryProcess();
}


void Analysis::_getWaveData(int *data)
{
  for(int i = 0; i < Size; i++){
    data[i] = Data[i];
  }
}

int Analysis::_getWaveHigh()
{
  int temp = -1;
  int tempn = -1;
  for(int i = 0; i < Size; i++){
    if(Data[i] > temp){
      temp = Data[i];
      tempn = i;
    }
  }

  temp = 0;
  for(int i = tempn; i < (tempn+maxcalpoints); i++){
    temp += Data[i];
  }

  return temp*1.0/maxcalpoints;
}


void Analysis::_MAFilter(double *data)
{
  for(int i = 0; i < mafiltern; i++){
    data[i] = Data[i];
  }

  for(int i = mafiltern; i < Size; i++){
    data[i] = data[i-1] + (Data[i] - Data[i-mafiltern])/mafiltern;
  }
}


void Analysis::_MGFilter(double *data)
{
  double dn = 0;
  for(int i = 0; i < mgfiltern; i++){
    data[i] = 0;
  }
  
  for(int i = mgfiltern; i < (Size-mgfiltern); i++){
    for(int j = -mgfiltern; j <= mgfiltern; j++){
      dn = getdn(j, mgfiltern);
      // cout << "dn = " << dn << endl;
      data[i] += dn*Data[i+j];
    }
  }

  for(int i = (Size-mgfiltern); i < Size; i++){
    data[i] = data[Size-mgfiltern-1];
  }

  int temp = data[0];
  int tempn = -1;
  for(int i = 0; i < Size; i++){
    if(data[i] > temp){
      temp = data[i];
      tempn = i;
    }
  }

  maxvalueofmgfilter = temp;
  maxpointofmgfilter = tempn;

  // 测试
  // cout << "max is " << temp << endl;
  // cout << "max point is " << tempn << endl;
}

void Analysis::_fitWave()
{
  // 初始化
  TCanvas *hc = new TCanvas("hc", "", 600, 400);
  hc->Divide(2, 2);
  auto hg1 = new TGraph();
  auto hg2 = new TGraph();
  
  for(int i = 0; i < Size; i++){
    hg1->SetPoint(i, i, Data[i]);
    hg2->SetPoint(i, i, Data[i]);
  }
  
  // fetch the TF1 named "deuxmax"
  TF1 *hf1 =(TF1 *)gROOT->FindObject("hf1");
  if(!hf1){
    hf1 = new TF1("hf1", DeuxMaxwell, 0, Size, 6);
  }
  hf1->SetParameters(1000, 1000, 400, 1000, 1000, 400);
  hf1->SetLineColor(kGreen);
  hf1->SetLineWidth(3);
  hc->cd(1);
  hg1->Draw();
  hg1->Fit("hf1");

  hf1->GetParameters(par);
  for(int i = 0; i < 6; i++){
    cout << "par " << i << " = " << par[i] << endl;
  }
  cout << "\n";
  
  // fetch the TF1 named "hf2"
  TF1 *hf2 = new TF1("hf2", Douexp, 500, 1500, 4);
  
  hf2->SetParameters(20, 3000, 1000, 1);
  hf2->SetLineColor(kBlue);
  hf2->SetLineWidth(3);
  hc->cd(2);
  hg2->Draw();
  hg2->Fit("hf2");
  
  hf2->GetParameters(par);
  for(int i = 0; i < 4; i++){
    cout << "par " << i << " = " << par[i] << endl;
  }

  // 拟合曲线图
  TGraph *hg3 = new TGraph();
  TGraph *hg4 = new TGraph();

  Double_t hx1[2000], hy1[2000];
  Double_t hx2[2000], hy2[2000];
  for(int i = 0; i < 2000; i++){
    hx1[i] = i;
    hx2[i] = i;
    hy1[i] = (par[3]/(par[0]-par[1])*TMath::Exp((par[2]-i)/par[0]));
    hy2[i] = -(par[3]/(par[0]-par[1])*TMath::Exp((par[2]-i)/par[1]));

    hg3->SetPoint(i, hx1[i], hy1[i]);
    hg4->SetPoint(i, hx2[i], hy2[i]);
  }

  hc->cd(3);
  hg3->Draw();
  hc->cd(4);
  hg4->Draw();

  // 如果不想作出图像，可以将内存删除
  delete hc;
  delete hg1;
  delete hg2;
}

void Analysis::_MWDFilter(double *data)
{
  // get u(k)
  double *u = new double[Size];
  for(int i = 0; i < Size; i++){
    for(int j = 0; j <= i; j++){
      u[i] += (Data[j]/par[1]);
    }
    u[i] += Data[i];
  }
  
  for(int i = 0; i < mwdfiltern; i++){
    data[i] = 0;
  }
  for(int i = mwdfiltern; i < Size; i++){
    data[i] = u[i] - u[i-mwdfiltern];
  }
}


void Analysis::_getRiseTime()
{
  double percentL, percentH;

  int temp = -1;
  int tempn = -1;

  for(int i = 0; i < Size; i++){
    if(Data[i] > temp){
      temp = Data[i];
      tempn = i;
    }
  }
  cout << "最大值：" << temp << "\n";
  cout << "最大值的点：" << tempn << "\n";

  temp = 0;
  for(int i = tempn; i < (tempn+maxcalpoints); i++){
    temp += Data[i];
  }

  percentL = 0.1*temp/maxcalpoints; // 0.1 to 0.9
  percentH = 0.9*temp/maxcalpoints;

  // cout << "percentL = " << percentL << "\n";
  // cout << "percentH = " << percentH << "\n";
  
  int rStart = -1, rStop = -1;
  for(int i = 0; i < Size; i++){
    if(Data[i] > percentL){
      rStart = i-1;
      break;
    }
  }
  for(int i = 0; i < Size; i++){
    if(Data[i] > percentH){
      rStop = i-1;
      break;
    }
  }

  // 输出测试
  // for(int i = 0; i < Size; i++){
  //   cout << i << " " << Data[i] << "\n";
  // }

  risetime = rStop - rStart;
}


void Analysis::_getCFD(double *data)
{ 
  double cfdfactor = 0.4;
  int decaypoint = (int)(risetime*cfdfactor);
  for(int i = 0; i < decaypoint; i++){
    data[i] = 0;
  }
  for(int i = decaypoint; i < Size; i++){
    data[i] = Data[i-decaypoint] - cfdfactor*Data[i];
  }

  // get cross zero time
  double tmpmax = -1;
  double tmpmin = -1;
  int tmpmaxN = -1, tmpminN = -1;
  int zerocross = 0;
  
  for(int i = 0; i < Size; i++){
    if(tmpmax < data[i]){
      tmpmax = data[i];
      tmpmaxN = i;
      // cout << i << "目前最大值 = " << tmpmax << "\n";
    }
    if(tmpmin > data[i]){
      tmpmin = data[i];
      tmpminN = i;
      // cout << i << "目前最小值 = " << tmpmin << "\n\n";
    }
  }

  // cout << "xmax is " << tmpmax << endl;
  // cout << "xmin is " << tmpmin << endl;

  for(int i = tmpminN; i <= tmpmaxN; i++){
    if((data[i] < 0) && (data[i+1] > 0)){
      zerocross = ((data[i]+data[i+1]) > 0 ? i : i+1);
    }
  }

  // 输出测试
  // for(int i = 0; i < Size; i++){
  //   cout << i << " " << data[i] << "\n";
  // }
  // cout << "zerocross is " << zerocross << endl;
}

void Analysis::_getPPS(int *data){
  for(int i = 0; i < Size; i++){
    cout << "in _getPPs, Data is " << Data[i] << endl;
  }
}


void Analysis::_dataPrimaryProcess()
{
  int baseline = 0;
  for(int i = 0; i < baselinecalpoints; i++){
    baseline += Data[i];
  }
  baseline /= baselinecalpoints;
  // cout << baseline << endl;
  
  if(plusepolarity){
    for(int i = 0; i < Size; i++){
      Data[i] -= baseline;
    }
  }
  else{
    for(int i = 0; i < Size; i++){
      Data[i] = baseline - Data[i];
    }
  }
}


/* ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... */
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
// p0: 上升时间相关
// p1: 下降时间相关
// p2: 事件起点
// p3: 系数
Double_t Douexp(Double_t *x, Double_t *p)
{
  if((x[0] > p[2])  && (p[0] > 0) && (p[1] > 0)){
    return p[3]*(1/(p[0]-p[1]))*(TMath::Exp((p[2]-x[0])/p[0]) - TMath::Exp((p[2]-x[0])/p[1]));
  }
  else{
    return 0;
  }
}

