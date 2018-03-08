#include "r2root.h"

#include <climits>
#include <iostream>

using std::cout; using std::endl;

R2root::R2root(const char *infileN, const char *outfileN)
{
  rawevt = new Eventer();
  rawevt->OpenFile(infileN);

  havedata = false;

  nevt = 0;

  rf = new TFile(outfileN, "recreate");
  benchmark = new TBenchmark();
  t = new TTree("t", "south afria xia data");

  t->Branch("pileup", &pileup, "pileup/O");
  t->Branch("outofr", &outofr, "outofr/O");

  t->Branch("cid", &cid, "cid/S");
  t->Branch("sid", &sid, "sid/S");
  t->Branch("ch", &ch, "ch/S");
  t->Branch("evte", &evte, "evte/s");
  t->Branch("ts", &ts, "ts/l");
  t->Branch("ets", &ets, "ets/l");
  
  t->Branch("cfd", &cfd, "cfd/s");
  t->Branch("cfdft", &cfdft, "cfdft/O");
  
  t->Branch("trae", &trae, "trae/i");
  t->Branch("leae", &leae, "leae/i");
  t->Branch("gape", &gape, "gape/i");
  t->Branch("base", &base, "base/D");

  t->Branch("qs", &qs,"qs[8]/i");

  t->Branch("ltra", &ltra, "ltra/s");
  t->Branch("data", &data, "data[ltra]/s");
  t->Branch("x", &x, "x[ltra]/s");

  t->Branch("nevt", &nevt, "nevt/I");
}


R2root::~R2root()
{
  delete rawevt;
}

void R2root::Process()
{
  benchmark->Start("this program");	// start timing

  ULong64_t timestamp;
  nevt = 0;

  havedata = rawevt->GetNextEvent();
  cout << "test here..." << endl;
  while(true){
    timestamp = ULLONG_MAX;
    if(havedata){
      if(rawevt->getts() < timestamp){
	timestamp = rawevt->getts();
      }
    }
    else{
      cout << "break here" << endl;
      break;
    }

    ClearOpt();

    ch = rawevt->getch();
    sid = rawevt->getsid();
    cid = rawevt->getcid();
    pileup = rawevt->getpileup();

    ts = rawevt->getts();
    cfd = rawevt->getcfd();
    cfdft = rawevt->getcfdft();

    evte = rawevt->getevte();
    ltra = rawevt->getltra();
    outofr = rawevt->getoutofr();

    if(rawevt->getesumf()){
      trae = rawevt->gettrae();
      leae = rawevt->getleae();
      gape = rawevt->getgape();
      // base = rawevt->getbase();
      base = IeeeBin2Float(rawevt->getbase());
    }

    if(rawevt->getqsumf()){
      rawevt->getqs(qs);
    }

    if(rawevt->getetsf()){
      ets = rawevt->getets();
    }

    if(rawevt->gettracef()){
      rawevt->gettrace(data);
      for(int i = 0; i < (int)ltra; i++){
	x[i] = i;
      }
    }
   
    t->Fill();
   
    nevt++;
    if((nevt%10000) == 0){
      cout << "nunber of event: " << nevt << endl; 
    }

    havedata = rawevt->GetNextEvent();

    // for test
    if(nevt == 1){
      cout << "test no." << nevt << " event." << endl;
      cout << "module number is: " << sid << endl;
      cout << "channel number is: " << ch << endl;
      cout << "length of trace is: " << ltra << endl;
    }
  }

  rf->cd();
  t->Write();
  rf->Close();

  benchmark->Show("this program");	// end of timing
}


bool R2root::IsFileExit(const char *infileN)
{
  if((access(infileN, F_OK)) != -1){
    printf("the file %s does not exit.", infileN);
    return false;
  }
  return true;
}


void R2root::ClearOpt()
{
  ch = -1;
  sid = -1;
  cid = -1;
  pileup = false;

  ts = 0;
  cfd = 0;
  cfdft = 0;

  evte = 0;
  ltra = 0;
  outofr = false;

  trae = 0;
  leae = 0;
  gape = 0;
  base = 0;

  ets = 0;

  memset(qs, 0, sizeof(UInt_t)*8);
  memset(data, 0, sizeof(UShort_t)*MaxTraceN);
  memset(x, 0, sizeof(UShort_t)*MaxTraceN);
}


double R2root::IeeeBin2Float(unsigned int ieenum)
{
  short signbit, exponent;
  double mantissa, decimalnumber;
	
  signbit = (short)(ieenum >> 31);
  exponent = (short)((ieenum & 0x7F800000) >> 23) - 127;
  mantissa = 1.0 + (double)(ieenum & 0x7FFFFF) / std::pow(2.0, 23.0);
  if(signbit == 0){
    decimalnumber = mantissa * std::pow(2.0, (double)exponent);
  }
  else{
    decimalnumber = - mantissa * std::pow(2.0, (double)exponent);
  }
	
  return decimalnumber;
}

