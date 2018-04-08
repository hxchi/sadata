#include "analysis.h"

#include "TApplication.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TBenchmark.h"		// 计时
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


int test()
{
  TBenchmark *benchmark = new TBenchmark(); // 计时
  // 为了批量处理，用chain类型打开root文件
  TChain *hch = new TChain("t");
  hch->Add("/home/xch/Desktop/sadata/convert-root/file-out/rsf.root");
  // hch->Print();

  /* ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... */
  // 需要从root文件中提取出信息

  Bool_t pileup;		// 需要和tree中的Branch一致
  Bool_t outofr;
  Short_t cid;
  Short_t sid;
  Short_t ch;
  UShort_t evte;
  ULong64_t ts;
  ULong64_t ets;
  UShort_t cfd;
  Bool_t cfdft;
  UInt_t trae;
  UInt_t leae;
  UInt_t gape;
  Double_t base;
  UInt_t qs[8];
  UShort_t ltra;
  UShort_t data[10000];		// or [ltra]
  UShort_t x[10000];		// or [ltra]
  Int_t nevt;

  TBranch *b_pileup;
  TBranch *b_outofr;
  TBranch *b_cid;
  TBranch *b_sid;
  TBranch *b_ch;
  TBranch *b_evte;
  TBranch *b_ts;
  TBranch *b_ets;
  TBranch *b_cfd;
  TBranch *b_cfdft;
  TBranch *b_trae;
  TBranch *b_leae;
  TBranch *b_gape;
  TBranch *b_base;
  TBranch *b_qs;
  TBranch *b_ltra;
  TBranch *b_data;		// or [ltra]
  TBranch *b_x;		// or [ltra]
  TBranch *b_nevt;

  // 将声明的变量和root文件中tree的变量关联
  hch->SetBranchAddress("pileup", &pileup, &b_pileup);
  hch->SetBranchAddress("outofr", &outofr, &b_outofr);
  hch->SetBranchAddress("cid", &cid, &b_cid);
  hch->SetBranchAddress("sid", &sid, &b_sid);
  hch->SetBranchAddress("ch", &ch, &b_ch);
  hch->SetBranchAddress("evte", &evte, &b_evte);
  hch->SetBranchAddress("ts", &ts, &b_ts);
  hch->SetBranchAddress("ets", &ets, &b_ets);
  hch->SetBranchAddress("cfd", &cfd, &b_cfd);
  hch->SetBranchAddress("cfdft", &cfdft, &b_cfdft);
  hch->SetBranchAddress("trae", &trae, &b_trae);
  hch->SetBranchAddress("leae", &leae, &b_leae);
  hch->SetBranchAddress("gape", &gape, &b_gape);
  hch->SetBranchAddress("base", &base, &b_base);
  hch->SetBranchAddress("qs", qs, &b_qs);
  hch->SetBranchAddress("ltra", &ltra, &b_ltra);
  hch->SetBranchAddress("data", data, &b_data);
  hch->SetBranchAddress("x", x, &b_x);
  hch->SetBranchAddress("nevt", &nevt, &b_nevt);

  
  /* ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... */
  // 进入数据处理
  printf("开始处理...\n");

  benchmark->Start("hana");	// 计时开始

  ULong64_t totalEntry = hch->GetEntries(); // TChain中总行数
  std::cout << "总行数为：" << totalEntry << "\n";

  Analysis *ana = new Analysis();

  // 设置类Analysis的输入值
  int fre = 100;		// 采样频率100MHZ
  bool pl = 1;			// 正信号
  int bcal = 10;		// 用100个点求基线的平均值
  int mcal = 10;		// 计算最高点的平均值所用的点的个数
  int maf = 1; 		        // mafiliter的参数
  int mgf = 6;			// mgfiliter的参数
  int mwdf = 300;		// mwdfiliter的参数 

  ana->_setMoudleADCMsps(fre);
  ana->_setPulsePolarity(pl);
  ana->_setBaselinCalPonits(bcal);
  ana->_setMaxCalPoints(mcal);
  ana->_setMAFilterParN(maf);
  ana->_setMGFilterParN(mgf);
  ana->_setMWDFilterParN(mwdf);

  
  // 设置变量，用来获取root中的数据
  int y[EVENTLENGTH];		// for orignal filter
  double yma[EVENTLENGTH];	// for MA filter
  double ymg[EVENTLENGTH];	// for MG filter
  double ymwd[EVENTLENGTH];	// for MWD filter
  double ycfd[EVENTLENGTH];	// for cfd
  // double ypps[EVENTLENGTH];	// for PPS
  
  // 通过循环处理TChain中的每一行数据
  for(Long64_t entry = 0; entry < 1; entry++){
    hch->GetEvent(entry);	// 读取TChain中的每一行数据
    ana->_setEventData(ltra, data); // data是从root文件中得到的数据，通过该函数传给Ana类
    ana->_getWaveData(y);

    if(ltra == 0){
      continue;
    }
    else{
      cout << "entry = " << entry << endl;
      cout << "sid = " << sid << endl;
      cout << "cid = " << cid << endl;
      cout << "ch = " << ch << endl;
      cout << "\n\n";
    }
    // 输出测试
    cout << "第" << entry << "行：" << "\n";
    cout << "采样点的个数：" << ltra << "\n";  

    // 赋值
    for(int i = 0; i < ltra; i++){
      // yma[i] = y[i];
      // ymg[i] = y[i];
      // ymwd[i] = y[i];
      // ycfd[i] = y[i];
    }
    // 图像绘制
    // TCanvas *c = new TCanvas("c", "", 1200, 700);
    // c->Divide(2,2);
    // TGraph *hg = new TGraph();
    // for(int i = 0; i < ltra; i++){
    //   hg->SetPoint(i, i, y[i]);
    // }
    // c->cd(1);
    // hg->Draw();
    // hg->SetTitle("orignal waveform");

    // TGraph *hmag = new TGraph();
    // ana->_MAFilter(yma);
    // for(int i = 0; i < ltra; i++){
    //   hmag->SetPoint(i, i, y[i]);
    // }
    // c->cd(2);
    // hmag->Draw();
    // hmag->SetTitle("filter waveform");
    // hmag->SetLineColor(4);
    // hmag->SetMarkerColor(4);
    
    // TGraph *hmgg = new TGraph();
    // ana->_MGFilter(ymg);
    // for(int i = 0; i < ltra; i++){
    //   hmgg->SetPoint(i, i, ymg[i]);
    // }
    // c->cd(2);
    // hmgg->Draw("same");
    // hmgg->SetLineColor(2);

    // 能量算法，不稳定，暂不加入
    // ana->_fitWave();
    // TGraph *hmwdg = new TGraph();
    // ana->_MWDFilter(ymwd);
    // for(int i = mwdf+1; i < ltra; i++){
    //   hmwdg->SetPoint(i, i, ymwd[i]);
    // }
    // c->cd(3);
    // hmwdg->Draw();
    // hmwdg->SetTitle("energy algorithm");
    // hmwdg->GetXaxis()->SetRangeUser(100, ltra);

    // cfd算法，稳定，暂时不需要
    // ana->_getRiseTime();
    // ana->_getCFD(ycfd);
    // TGraph *hcfdg = new TGraph();
    // for(int i = 0; i < ltra; i++){
    //   hcfdg->SetPoint(i, i, ycfd[i]);
    // }
    // c->cd(4);
    // hcfdg->Draw();
    // hcfdg->SetTitle("cfd cross zero point");

    // ana->getPPS(xpps, ypps, 2);
  }

  benchmark->Show("hana");
  
  return 0;
}


// 当使用个g++编译，需要这两个函数配合才能出现图像
void StandaloneApplication(int argc, char** argv) {
  test();
}

// 主函数
int main(int argc, char** argv)
{
  TApplication app("ROOT Application", &argc, argv);
  StandaloneApplication(app.Argc(), app.Argv());
  app.Run();
  
  return 0;
}

