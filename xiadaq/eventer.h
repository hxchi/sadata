/* eventer类是用来表示xia获取中每一次点火记录的事件 */
/* 包含的信息包括channel，能量，时间等等 */
/* eventer类为r2root提供所需数据 */
/* 类的成员函数以_开头 */

#ifndef EVENTER_H_
#define EVENTER_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 		/* open(),read() */
#include <string.h>		/* memset() */

const int  MaxTraceN = 10000;		/* 采样点数上限 */
/* ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... */

class Eventer{
public:
  Eventer();
  virtual ~Eventer();

  void _clearAll();
  bool _openFile(const char*);

  void _printWord(); // for debug purpose
  bool _getNextWord(); // for debug purpose

  bool _getNextEvent();

  inline short _getch() {return ch;}
  inline short _getsid() {return sid;}
  inline short _getcid() {return cid;}
  inline unsigned short _getlhead() {return lhead;}
  inline unsigned short _getlevt() {return levt;}
  inline bool _getpileup() {return pileup;}

  inline unsigned long _getts() {return ts;}
  inline unsigned short _getcfd() {return cfd;}
  inline bool _getcfdft() {return cfdft;}
  inline unsigned short _getevte() {return evte;}
  inline unsigned short _getltra() {return ltra;}
  inline bool _getoutofr() {return outofr;}
  
  inline bool _getesumflag() {return esumf;}
  inline unsigned int _gettrae() {return trae;}
  inline unsigned int _getleae() {return leae;}
  inline unsigned int _getgape() {return gape;}
  inline unsigned int _getbase() {return base;}

  inline bool _getqsumflag() {return qsumf;}
  inline void _getqs(unsigned int *qqs) {memcpy(qqs, qs, sizeof(unsigned int)*8);}

  inline bool _getetsflag() {return etsf;}
  inline unsigned long _getets() {return ets;}
  
  inline bool _gettraceflag() {return tracef;}
  inline void _gettrace(unsigned short *da) {memcpy(da, data, sizeof(unsigned short)*ltra);}

private:
  bool _readWord();
  bool _decode();		/* 解码，也就是读取数据 */

  int fd; // File descripter
  unsigned int buff; // read 4 bytes from file(32bits)
	
  /* 数据 */
	
  short ch;  			/* channel number */
  short sid; 			/* slot id */
  short cid; 			/* crate id */
  unsigned short lhead; 	/* header length */
  unsigned short levt;  	/* event length */
  bool pileup; 			/* 0-->good event 1-->pileup event */

  unsigned long ts; 		/* timestamp of this event */
  unsigned short cfd;   	/* CFD fractional time */
  bool cfdft;			/* CFD forced trigger bit */
  
  unsigned short evte;  	/* Event enrgy */
  unsigned short ltra;  	/* Trace length */
  bool outofr;          	/* Trace Out-of-Range Flag */
  
  bool esumf;
  unsigned int trae;		/* trailing energy sum */
  unsigned int leae;		/* leading energy sum */
  unsigned int gape;		/* Gap energy sum */
  unsigned int base;		/* baseline value */

  bool qsumf;
  unsigned int qs[8];		/* qdc sum #0-#7 */

  bool etsf;
  unsigned long ets; 		/* external clock timestamp */
  
  // pulse shape (trace enabled)
  bool tracef;
  unsigned short  data[MaxTraceN]; /*  采样点信息 */

  // 数据结构
  const static unsigned int kMaskch  =	        0x0000000f;
  const static unsigned int kShiftch =	        0x0;
  const static unsigned int kMasksid =		0x000000f0;
  const static unsigned int kShiftsid =		4;
  const static unsigned int kMaskcid =		0x00000f00;
  const static unsigned int kShiftcid =		8;
  const static unsigned int kMasklhead =        0x0001f000;
  const static unsigned int kShiftlhead = 	12;
  const static unsigned int kMasklevt =         0x7ffe0000;
  const static unsigned int kShiftlevt =        17;
  const static unsigned int kMaskpileup =       0x80000000;
  const static unsigned int kShiftpileup = 	31;

  const static unsigned int kMasktslo =         0xffffffff;
  const static unsigned int kShifttslo =        0;
  const static unsigned int kMasktshi =         0x0000ffff;
  const static unsigned int kShifttshi =        0;
  const static unsigned int kMaskcfd = 		0x7fff0000;
  const static unsigned int kShiftcfd =		16;
  const static unsigned int kMaskcfdft =        0x80000000;
  const static unsigned int kShiftcfdft =       31;
  
  const static unsigned int kMaskevte =         0x0000ffff;
  const static unsigned int kShiftevte =        0;
  const static unsigned int kMaskltra =		0x7fff0000;
  const static unsigned int kShiftltra =        16;
  const static unsigned int kMaskoutofr =       0x80000000;
  const static unsigned int kShiftoutofr =      31;
  
  const static unsigned int kMaskesum =         0xffffffff;
  const static unsigned int kShiftesum =        0;
  const static unsigned int kMaskqs = 		0xffffffff;
  const static unsigned int kShiftqs = 		0;

  const static unsigned int kMasketslo =         0xffffffff;
  const static unsigned int kShiftetslo =        0;
  const static unsigned int kMasketshi =         0x0000ffff;
  const static unsigned int kShiftetshi =        0;

  const static unsigned int kMaskdata1 =       	0x0000ffff;
  const static unsigned int kShiftdata1 = 	0;
  const static unsigned int kMaskdata2 =        0xffff0000;
  const static unsigned int kShiftdata2 = 	16;

};

#endif
