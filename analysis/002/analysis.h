/* 用于处理数据的类 */
/* 该类不作为root文件的接口，而是接收从root文件中提取的数据（通过_setEventData()函数） */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>

#define EVENTLENGTH 65535

using std::cout; using std::endl;

class TGraph;			/* 声明这些类，下面才可以调用 */
class TTree;
class TH1D;
class TCanvas;


class Analysis{
 public:
  Analysis();
  virtual ~Analysis();

 /* 信号处理过程 */
  void _setMoudleADCMsps(int &fre){
    adcmsps = fre;
  }
  void _setPulsePolarity(bool &p){
    plusepolarity = p;
  }
  void _setBaselinCalPonits(int &num){
    baselinecalpoints = num;
  }
  void _setMaxCalPoints(int &num){
    maxcalpoints = num;
  }
  void _setMAFilterParN(int &num){
    mafiltern = num;
  }
  void _setMGFilterParN(int &num){
    mgfiltern = num;
  }
  void _setMWDFilterParN(int &num){
    mwdfiltern = num;
  }

  void _setEventData(unsigned short size, unsigned short *data);
  void _setEventData(int size, short *data);
  void _setEventData(int size, int *data);

  void _getWaveData(int *data);
  int _getWaveHigh();
  
  void _MAFilter(double *data);
  void _MGFilter(double *data);

  void _fitWave();
  void _MWDFilter(double *data);

  void _getRiseTime();

  void _getCFD(double *data);

  void _getPPS(int *data);
  
 private:
  void _dataPrimaryProcess();	/* 基线平移和将信号变为正信号 */
  
 private:
  int adcmsps;	      	/* 采样频率 */
  bool plusepolarity;	/* 信号极性 */
  int baselinecalpoints;	/* 求基线的平均值 */
  int maxcalpoints;		/* 计算最大值所用的点的个数(平均) */
  

 private:
  int mafiltern;
  int mgfiltern;
  int mwdfiltern;
  double par[6];
  int maxvalueofmgfilter;
  int maxpointofmgfilter;
  double risetime;
  
 private:
  int Size;
  int Data[EVENTLENGTH];
};


#endif
