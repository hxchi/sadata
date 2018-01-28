// for 2014 south africa data three demention 

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
// #include <io.h> // for _access()

// from me
#include "threefold.h"

// from root
#include "TCanvas.h"
#include "TH1D.h"
#include "TFile.h"

using std::cin; using std::cout; using std::endl; 
using std::ifstream; using std::ofstream; using std::string;

static const unsigned short blocksize = 32768;
static const unsigned short nclv = 8;


int main(int argc, char *argv[])
{
	// for root
	TFile *nf = new TFile("r.root", "recreate");
	TH1D *htime = new TH1D("htime", " ", 8000, 0, 8000);
	TH1D *hspec = new TH1D("hspec", " ", 4096, 0, 2047);

	struct SARDATAHEADER // south africa data header
	{
		char   header_id[8];             
		int    header_sequence;          
		short  header_stream;            
		short  header_tape;              
		short  header_MyEndian;          
		short  header_DataEndian;        
		int    header_dataLen;           
	}	dataheader_;

	struct EVTHEAD
	{
		unsigned short EventStart;
		unsigned short EventSize;
	} evthead_;

	FILE *fpout;
	if ((fpout=fopen("r42", "ab")) == NULL) {
		printf("%s\n", "can not open the output file...");
		return 0;
	}

	// get eff par
	double eff[8][4][3];
	geteff(eff);

	// data clv(clover), cyl(crystal)
	unsigned short adc[nclv][4]; 
	unsigned short tdc[nclv][4];
	long long etime[nclv][4][3];
	long long xtime[nclv][4][3];
	double eaddback[nclv];
	long long taddback[nclv];
	unsigned short label;
	unsigned short label_head;
	unsigned short label_tail;
	unsigned short nextone;
	unsigned short block[blocksize];
	unsigned short *pb;
	int evtnum = 0;

	// for threefold
	long long threefoldeventnum = 0; 
	int firecount;
	int timecount;

	// for write into file
	int blocksizecount = 0;

	// for lots of file
	int runstart, runend;
	cout << "enter start runnumber and end run number, like this \"runstart runend\". " << endl;
	cin >> runstart >> runend;
	cout << runstart << " " << runend << endl;
	cout << "...start..." << endl;

	FILE *fpin;
	char rawdatafilename[42];
	string pathdatafile;
	int r0123;
	for (int i = runstart; i <= runend; i++) {
		if (i == 1 || i == 2 || i == 35) {
			continue;
		}
		cout << "run ..." << i << " ..." << endl;
		r0123 = 1;
		while (1) { // for _0 _1 _2 ...
			sprintf(rawdatafilename, "R%d_%d", i, r0123);
			pathdatafile = "./" + string(rawdatafilename);
			if ((fpin=fopen(pathdatafile.c_str(), "rb")) == NULL) { 
				// printf("can not open %s\n", rawdatafilename);
				break;
			}
			else {
				r0123++;
				while (fread(block, sizeof(unsigned short), blocksize, fpin)) { // in a file 
					pb = block;
					pb += sizeof(dataheader_)/2; // jump block header

					evthead_.EventStart = *pb;
					evthead_.EventSize = *(pb+1);

					if (evthead_.EventStart==0xFFFF&&evthead_.EventSize==0x0000) { // jump the end of a block
						continue;
					}
					while (evthead_.EventStart==0xFFFF&&evthead_.EventSize!=0x0000) {
						// set adc tdc etime xtime to be zero 
						firecount = 0;
						timecount = 0;
						for (int s = 0; s < nclv; s++) {
							eaddback[s] = 0.0;
							taddback[s] = 0;
						}
						for (int s = 0; s < nclv; s++) {
							for (int q = 0; q < 4; q++) {
								adc[s][q] = 0;
								tdc[s][q] = 0;
							}
						}
						for (int s = 0; s < nclv; s++) {
							for (int q = 0; q < 4; q++) {
								for (int r = 0; r < 3; r++) {
									etime[s][q][r] = 0;
									xtime[s][q][r] = 0;
								}
							}
						}

						pb += 2;
						evtnum = (evthead_.EventSize - 4) / 2;
						for (int j = 0; j < evtnum; ) {
							// printf("%04x\n", *pb);
							label = *pb;
							nextone = *(pb+1);
							label_head = label & 0xFF00;
							label_tail = label & 0x00FF;
							if ((label_head == 0x4800) && (label_tail >= 9)) {
								pb += 10;
								j += 10;
								continue;
							}
							if ((label_head == 0x4800) && (label_tail >= 1) && (label_tail <= 8)) {
								for (int s = 0; s < 4; s++) {
									adc[label_tail-1][s] = *(pb + s + 1);
									tdc[label_tail-1][s] = *(pb + s + 5);
								}
								pb += 10;
								j += 10;
								continue;
							}

							if (((label_head&0xF000) == 0x8000) && (nextone == 0x270F)) {
								label_tail = *pb & 0x0FFF;
								pb += 2;
								j += 2;
								for (int s = 0; s < label_tail; s += 4) {
									getabtime(pb, etime, xtime);
									pb += 4;
									j += 4;
								}
								continue;
							}
						}
						
						// add_back
						addback(eff, adc, tdc, etime, eaddback, taddback);
						// getspecture
						for (int s = 0; s < nclv; s++) {
							if ((eaddback[s] >= 30) && (eaddback[s] <= 2047)) {
								hspec->Fill(eaddback[s]);
							}
						}

						// threefold
						for (int m = 0; m < nclv; m++) {
							if ((eaddback[m] >= 30) && (eaddback[m] <= 2047)) {
								firecount++;
							}
						}
						if (firecount == 0) {
							evthead_.EventStart = *pb;
							evthead_.EventSize = *(pb+1);
							continue; // jump the event without clover fired...
						}
						

						int firedclvnum[8] = {0};
						int ss = 0; // temp for fireclover
						for (int s = 0; s < nclv; s++) {
							if ((eaddback[s] >= 30) && (eaddback[s] <= 2047)) {
								firedclvnum[ss] = s;
								ss++;
							}
						}
						
						long long timediff[8] = {0};

						
						if (firecount >= 2) { // for time difference
							for (int s = 0; s < (firecount-1); s++) {
								timediff[s] = taddback[firedclvnum[s]] - taddback[firedclvnum[s+1]] + 4000;
							}
							timediff[firecount-1] = taddback[firedclvnum[firecount-1]] - taddback[firedclvnum[0]] + 4000;
							for (int s = 0; s < firecount; s++) {
								htime->Fill(timediff[s]);
							}
							timediff[firecount] = {0};
						}

						if (firecount >= 3) {
							for (int s = 0; s < (firecount-1); s++) {
								timediff[s] = taddback[firedclvnum[s]] - taddback[firedclvnum[s+1]] + 4000;
							}
							timediff[firecount-1] = taddback[firedclvnum[firecount-1]] - taddback[firedclvnum[0]] + 4000;
						}
						for (int s = 0; s < firecount; s++) {
							if (timediff[s] >= 3950 && timediff[s] <= 4150) {
								timecount++;
							}
						}
						if ((timecount == firecount) && (firecount>=3)) {
							threefoldeventnum++;
							const int writesize = 2 + 2*firecount;
							unsigned short writedata[writesize];

							writedata[0] = 0xFFFF;
							writedata[1] = (unsigned short)(writesize*2);
							for (int s = 0; s < firecount; s++) {
								writedata[2*(s+1)] = (unsigned short)firedclvnum[s];
								writedata[2*(s+1)+1] = (unsigned short)eaddback[firedclvnum[s]];
							}

							// write data
							unsigned short *tmpptr;
							tmpptr = writedata;
							for (int s = 0; s < writesize; s++) {
								fwrite(tmpptr, sizeof(unsigned short), 1, fpout);
								blocksizecount++;
								tmpptr++;
							}
						}

						if ((16384-blocksizecount) <= 16) {
							int tmpcount = (16384-blocksizecount);
							unsigned short blockend[16384-blocksizecount];
							blockend[0] = 0xFFFF;
							for (int s = 1; s < (16384-blocksizecount); s++) {
								blockend[s] = 0x0000;
			 				}
			 				unsigned short *tmpptrp;
			 				tmpptrp = blockend;
			 				for (int s = 0; s < tmpcount; s++) {
			 					fwrite(tmpptrp, sizeof(unsigned short), 1, fpout);
			 					blocksizecount++;
			 					tmpptrp++;
			 				}
			 				blocksizecount = 0;
						}

						evthead_.EventStart = *pb;
						evthead_.EventSize = *(pb+1);
					} 
			 	} // while (fread(block, sizeof(unsigned short), blocksize, fpin))
			}
			fclose(fpin);
		} // while (1) { // for _0 _1 _2 ...
	}
	fclose(fpout);
	nf->Write();
	nf->Close();
	cout << "...done..." << endl;
	cout << threefoldeventnum << endl;
	return 0;
}



