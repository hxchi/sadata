/*get xia data*/

#include "eventer.h"
#include "r2root.h"

// root
#include "TCanvas.h"
#include "TRandom.h"


#include <stdio.h>
#include <stdlib.h>

#include <string>

using std::string;

int main(int argc, char const *argv[])
{
  // if(argc != 2){
  // 	printf("需要一个参数，表示文件编号。\n");
  // 	printf("例如：./aout 520(必须是4位，不足需补0)\n");
  // 	return 0;
  // }

  // /* set input filename */
  // const string filepath = "/home/xch/Desktop/xiadata/";
  // const string datahead = "data_R";
  // const string dataend = "_M00.bin";
  // char inputfile[80];
  // strcpy(inputfile, filepath.c_str());
  // strcat(inputfile, datahead.c_str());
  // strcat(inputfile, argv[1]);
  // strcat(inputfile, dataend.c_str());

  // for south afria data
  char inputfile[80] = "/home/xch/Desktop/xiadata/rawdata_mod2-DDAS-2017-12-11-04:24:30.bin";

  R2root r2r(inputfile, "./file-out/rsf0407.root");
  r2r.Process();
	
  return 0;
}
