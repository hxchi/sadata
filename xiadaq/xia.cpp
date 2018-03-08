#include "xia.h"

Eventer::Eventer()
{
  ClearAll();
  fd = -1;
}

Eventer::~Eventer()
{

}

void Eventer::ClearAll()
{
  ch = -1;
  sid = -1;
  cid = -1;
  lhead = 0;
  levt = 0;
  pileup = 0;

  ts = 0;
  cfd = 0;
  cfdft = 0;

  evte = 0;
  ltra = 0;
  outofr = 0;

  esumf = 0;
  trae = 0;
  leae = 0;
  gape = 0;
  base = 0;

  qsumf = 0;
  memset(qs, 0, sizeof(unsigned int)*8);

  etsf = 0;
  ets = 0;

  tracef = 0;
  memset(data, 0, sizeof(unsigned short)*MaxTraceN);
}

bool Eventer::OpenFile(const char * fileN)
{
  if((fd = open(fileN, O_RDONLY)) < 0){
    printf("cannot open file %s.\n", fileN);
    return false;
  }
  printf("打开文件成功！\n");
  return true;
}

bool Eventer::ReadWord()
{
  if(fd < 0){
    printf("error, no opened file found!\n");
    return false;
  }
  size_t n = read(fd, &buff, 4);			// 4 means 4 bytes
  if(n <= 0){
    return false;
  }

  return true;
}

void Eventer::PrintWord()
{
  printf("0x%08x\n", buff);
}

bool Eventer::GetNextWord()
{
  return ReadWord();
}

bool Eventer::GetNextEvent()
{
  ClearAll();
  if(!GetNextWord()){
    return false;						// get the end of this file
  }

  return Decode();
}

bool Eventer::Decode()
{
  /* decode 1st word(32 bits) in this event */
  ch = (buff & kMaskch) >> kShiftch;
  sid = (buff & kMasksid) >> kShiftsid;
  cid = (buff & kMaskcid) >> kShiftcid;
  lhead = (buff & kMasklhead) >> kShiftlhead;
  levt = (buff & kMasklevt) >> kShiftlevt;
  pileup = (buff & kMaskpileup) >> kShiftpileup;

  /* decode 2nd word(32 bits) in this event */
  if(!GetNextWord()){
    printf("cannot get 2nd word(32 bits).\n");
    return false;
  }
  ts = (buff & kMasktslo) >> kShifttslo;

  /* decode 3rd word(32 bits) in this event */
  if(!GetNextWord()){
    printf("cannot get 3rd word(32 bits).\n");
    return false;
  }
  ts = ts + (((unsigned long)(buff & kMasktshi) >> kShifttshi)*0xffffffff);

  cfd = (buff & kMaskcfd) >> kShiftcfd;
  cfdft = (buff & kMaskcfdft) >> kShiftcfdft;

  /* decode 3rd word(32 bits) in this event */
  if(!GetNextWord()){
    printf("cannot get 4th word(32 bits).\n");
    return false;
  }

  evte = (buff & kMaskevte) >> kShiftevte;
  ltra = (buff & kMaskltra) >> kShiftltra;
  outofr = (buff & kMaskoutofr) >> kShiftoutofr;

  /* set esumf, qsumf, etsf, tracef value according to lhead and ltra*/
  switch ((int)lhead) {
  case 4 : 
    esumf = false;
    qsumf = false;
    etsf = false;
    break;

  case 6 : 
    esumf = false;
    qsumf = false;
    etsf = true;
    break;

  case 8 : 
    esumf = true;
    qsumf = false;
    etsf = false;
    break;

  case 10 : 
    esumf = true;
    qsumf = false;
    etsf = true;
    break;

  case 12 : 
    esumf = false;
    qsumf = true;
    etsf = false;
    break;

  case 14 : 
    esumf = false;
    qsumf = true;
    etsf = true;
    break;

  case 16 : 
    esumf = true;
    qsumf = true;
    etsf = false;
    break;

  case 18 : 
    esumf = true;
    qsumf = true;
    etsf = true;
    break;

  default :
    printf("error, lhead is %d.\n", lhead);
    break;
  }
  if(ltra	> 0){
    tracef = true;
  }

  /* get esum value */
  if(esumf){
    if(!GetNextWord()){
      printf("cannot get 1st word(32 bits) in esum.\n");
      return false;
    }
    trae = (buff & kMaskesum) >> kShiftesum;

    if(!GetNextWord()){
      printf("cannot get 2nd word(32 bits) in esum.\n");
      return false;
    }
    leae = (buff & kMaskesum) >> kShiftesum;

    if(!GetNextWord()){
      printf("cannot get 3rd word(32 bits) in esum.\n");
      return false;
    }
    gape = (buff & kMaskesum) >> kShiftesum;

    if(!GetNextWord()){
      printf("cannot get 4th word(32 bits) in esum.\n");
      return false;
    }
    base = (buff & kMaskesum) >> kShiftesum;
  }

  /*get qsum value*/
  if(qsumf){
    for(int i = 0; i < 8; i++){
      if(!GetNextWord()){
	printf("cannot get word(32 bits) %d in qsum.\n", (i+1));
	return false;
      }
      qs[i] = (buff & kMaskqs) >> kShiftqs;
    }
  }

  /*get ets value*/
  if(etsf){
    if(!GetNextWord()){
      printf("cannot get 1st word(32 bits) in ets.\n");
      return false;
    }
    ets = (buff & kMasketslo) >> kShiftetslo;

    if(!GetNextWord()){
      printf("cannot get 2nd word(32 bits) in ets.\n");
      return false;
    }

    ets = ets + (((unsigned long)(buff & kMasketshi) >> kShiftetshi)*0xffffffff); 
  }

  /*get trace value*/
  if(tracef){
    if(ltra > MaxTraceN){
      printf("too much points.\n");
      return false;
    }

    for(int i = 0; i < ltra/2; i++){
      if(!GetNextWord()){
	printf("cannot get word(32 bits) %d in trace data.\n", (i+1));
	return false;
      }
      data[2*i] = (buff & kMaskdata1) >> kShiftdata1;
      data[2*i+1] = (buff & kMaskdata2) >> kShiftdata2;
    }
  }
  return true;
}	
