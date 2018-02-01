#include "../include/spec.h"

// root
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"

int main()
{
	// from root
	TFile *prf = new TFile("r.root", "recreate");
	TH1D *htime = new TH1D("htime", " ", 16000, 2000, 6000);
	TH1D *hspec = new TH1D("hspec", " ", 4096, 0, 2047);
	TH1D *hmultifired = new TH1D("hmultifired", " ", 8, 1, 9);

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

	static unsigned short blocksize = 32768;


	// for energy cal
	double calpar[9][4][3];
	getcalpar(calpar);

	FILE *fp;

	if ((fp=fopen("../d17/R51_0", "rb")) == NULL) {
		printf("can not open the file.\n");
		return -1;
	}


	// const value
	const int coindiencetime = 150; // unit 10 ns 

	unsigned short adc[9][4];
	unsigned short tdc[9][4];
	long long etime[9][4][2];
	long long xtime[9][4][2];
	unsigned short label_head;
	unsigned short label_tail;
	double addbackene = 0;
	unsigned short block[blocksize];
	unsigned short *pb;

	// for event
	const int timeciondience = 150;
	int kk = 0;
	bool eventflag = 1;
	long long tmptime = 0;
 	unsigned short tmptimestamp[2][2];
 	long long t0 = 0;
 	long long t1 = 0; // for time coindience
 	int firecount;

 	// for addback
 	double eaddback[9];
 	long long taddback[9];

 	// for csi
 	unsigned short qdc[64][4];
 	long long qtime[64][4][2];

	while (fread(block, sizeof(unsigned short), blocksize, fp)) {
		// fread(block, sizeof(unsigned short), blocksize, fp);
		pb = block;
		// jump block header
		pb += sizeof(dataheader_)/2; 

		pb += 7; // jump to the last unsignede short number in a fired
		// in r17_0, the first fired is c052(clover0 segement c), but without energy, only tdc, so jump this fired...
		// what in other data file, still needs to be comfired...
		while (pb != (block + blocksize)) { // while (pb != (block + blocksize)) // here is in each block...
			if (*pb == 0x5e5e && *(pb+1) == 0x5e5e && *(pb-1) == 0x5e5e) { // test if the end of a block
				break;
			}

			// if (kk == 2) { // control event number
			// 	break;
			// }

			eventflag = 1;
			while (eventflag) { // in an event
				// init
				// cout << "...start an event..." << endl;
				firecount = 0;
				// tmptimestampcount = 0;
				for (int m = 0; m < 9; m++) {
					for (int n = 0; n < 4; n++) {
						adc[m][n] = 0;
						tdc[m][n] = 0;
					}
				}

				for (int r = 0; r < 9; r++) {
					for (int s = 0; s < 4; s++) {
						for (int t = 0; t < 2; t++) {
							etime[r][s][t] = 0;
							xtime[r][s][t] = 0;
						}
					}
				}

				for (int m = 0; m < 2; m++) {
					for (int n = 0; n < 2; n++) {
						tmptimestamp[m][n] = 0;
					}
				}

				for (int s = 0; s < 9; s++) {
					eaddback[s] = 0;
					taddback[s] = 0;
				}

				label_head = *pb >> 12; // for example, "c06b" get c
				label_tail = *pb & 0x0FFF; // get label

				if (label_head == 8) { // jump useless data // if is ok? shall we need while...
					pb+=4; // we don't need continue here, because maybe there is a 8440 in an event...
				}
				else { // read some information, included adc, tdc, timestamp
					getadc_tdc_timestamp(pb, label_tail, adc, tdc, etime, xtime, qdc, qtime);
					tmptimestamp[0][0] = *(pb-3); // 0-15bit
					tmptimestamp[0][1] = *(pb-2); // 16-32bit
					t0 = (tmptimestamp[0][1]<<16) + tmptimestamp[0][0];
					// tmptimestampcount++;
					pb+=4;
				}

				while (1) { // read next fired. still in this event...
					
					label_head = *pb >> 12;
					label_tail = *pb & 0x0FFF;
					if (label_head == 8) {
						// cout << "... meet 8 ..." << endl;
						pb+=4; 
					}
					else {
						tmptimestamp[1][0] = *(pb-3);
						tmptimestamp[1][1] = *(pb-2);

						// for (int m = 0; m < 2; m++) {
						// 	for (int n = 0; n < 2; n++) {
						// 		printf("%04x  ", tmptimestamp[m][n]);
						// 	}
						// 	printf("\n");
						// }

						t1 = (tmptimestamp[1][1]<<16) + tmptimestamp[1][0];
						// cout << t0 << endl;
						// cout << t1 << endl;
						// cout << abs(t0-t1) << "......" << endl;

 						if (abs((int)(t0 - t1)) >= timeciondience) {
							eventflag = 0;
							kk++;
							break;
						}

						getadc_tdc_timestamp(pb, label_tail, adc, tdc, etime, xtime, qdc, qtime);
						// tmptimestampcount++;
						pb+=4;
					}
				} // after here, event read done.


				// add back and energy cal
				addback(calpar, adc, tdc, etime, eaddback, taddback);

				// for (int s = 0; s < 9; s++) {
				// 	cout << eaddback[s] << "  ";
				// }
				// cout << "......" << endl;
				
				// get energy specture
				for (int s = 0; s < 9; s++) {
					if ((eaddback[s] >= 30) && (eaddback[s] <= 2047)) {
						hspec->Fill(eaddback[s]);
					}
				}

				// // get multi specture
				// for (int m = 0; m < 9; m++) {
				// 	if ((eaddback[m] >= 30) && (eaddback[m] <= 2047)) { // ((eaddback[m] >= 30) && (eaddback[m] <= 2047))
				// 		firecount++;
				// 	}
				// }
				// hmultifired->Fill(firecount);

				// if (firecount == 0) {
				// 	continue; // jump the event without clover fired...
				// }

				// //  get coindience time soecture
				// int firedclvnum[9] = {0};
				// int ss = 0; // temp for fireclover
				// for (int s = 0; s < 9; s++) {
				// 	if ((eaddback[s] >= 30) && (eaddback[s] <= 2047)) {
				// 		firedclvnum[ss] = s;
				// 		ss++;
				// 	}
				// }
				
				// long long timediff[8] = {0};
				
				// if (firecount >= 2) { // for time difference
				// 	for (int s = 0; s < (firecount-1); s++) {
				// 		timediff[s] = taddback[firedclvnum[s]] - taddback[firedclvnum[s+1]] + 4000;
				// 	}
				// 	timediff[firecount-1] = taddback[firedclvnum[firecount-1]] - taddback[firedclvnum[0]] + 4000;
				// 	for (int s = 0; s < firecount; s++) {
				// 		htime->Fill(timediff[s] + ((rand()%999 + 1) / 100.0));
				// 		// cout << timediff[s] << endl;
				// 		// cout << (rand()%999 + 1) / 1000.0 << endl; 
				// 	}
				// 	timediff[firecount] = {0};
				// }


				// for (int m = 0; m < 9; m++) { // test adc value
				// 	for (int n = 0; n < 4; n++) {
				// 		printf("%04x  ", adc[m][n]);
				// 	}
				// 	printf("......\n");
				// }

				// for (int m = 0; m < 64; m++) { // test qdc value
				// 	for (int n = 0; n < 4; n++) {
				// 		printf("%04x  ", qdc[m][n]);
				// 	}
				// 	printf("......\n");
				// }
				// printf("\n\n");

				// else if (label_head == 12) {
				// 	timestamp[0] = *(pb-2);
				// 	timestamp[1] = *(pb-3);
				// 	tmptime = timestamp[0]<<16 + timestamp[1];
				// 	if ((((label_tail>>4) & 0x00F)%2) == 0) { // check the label means adc or tdc
				// 		adc = *(pb-1) & 0x3FFF; 
				// 	}
				// 	else {
				// 		tdc = *(pb-1) & 0x3FFF; 
				// 	}
				// }
				// else { // to jump like e082, because the energy in these fired is zero, only happened in adc channel...
				// 	// cout << " ... other label_head..." << endl;
				// 	// printf("%04x  ", *pb);
				// 	// printf("%04x  ", *(pb-1));
				// 	// printf("%04x\n", *(pb-3));
				// 	pb+=4; // jump the tdc channel after it...
				// 	continue;
				// }

				// // caculate energy
				// if (adc > 0 && tdc == 0) { // make sure is the adc file
				// 	addbackene = getene(label_tail, adc, eff);
				// 	h->Fill(addbackene);
				// }

				// adc = 0;
				// tdc = 0; // important
			}
			
		}
	} // while (fread(block, sizeof(unsigned short), blocksize, fp))
	
	fclose(fp);
	prf->Write();
	prf->Close();

	// test #0 test fuction eff
	// for (int i = 0; i < 4; i++) {
	// 	for(int j = 0; j < 9; j++) {
	// 		for (int k = 0; k < 3; k++) {
	// 			cout << eff[j][i][k] << " ";
	// 		}
	// 		cout << endl;
	// 	}
	// }

	// test #1
	// for (int i = 0; i < 4; i++) {
	// 	printf("%04x  ", *pb);
	// 	pb++;
	// }

	// test #2
	// pb += 7;
	// label_head = *pb >> 12;
	// label_tail = *pb & 0x0FFF;
	// cout << label_head << endl;
	// cout << label_tail << endl;

	// test #3
	// if (label_head == 8) { // jump useless data
	// 		cout << "useless data" << endl;;
	// 	}

	// else if (label_head == 12) {
	// 	timestamp[0] = *(pb-3);
	// 	timestamp[1] = *(pb-2);
	// 	printf("%04x  %04x\n", timestamp[0], timestamp[1]);
	// 	if ((((label_tail>>4) & 0x00F)%2) == 0) { // get the middle number of label_tail
	// 		adc = *(pb-1) & 0x3FFF; 
	// 		cout << "adc = " << adc << endl;
	// 	}
	// 	else {
	// 		tdc = *(pb-1) & 0x3FFF; 
	// 		cout << "tdc = " << tdc << endl;
	// 	}
	// }

	// test #4 function getene() 
	// is right. answer is 439.329
 	// cout << getene(label_tail, adc, eff) << endl; 

	return 0;
}