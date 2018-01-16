#include "../include/spec.h"

// root
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"

int main()
{
	// from root
	TFile *prf = new TFile("~/sadata/out/totalspec.root", "recreate");
	TCanvas *c = new TCanvas("c", " ", 600, 600);
	TH1D *h = new TH1D("h", " ", 16384, 0, 16383);

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


	// for eff
	double eff[9][4][3];
	geteff(eff);

	FILE *fp;

	if ((fp=fopen("~/sadata/d17/R17_0", "rb")) == NULL) {
		printf("can not open the file.\n");
		return -1;
	}

	unsigned short timestamp[2];
	unsigned short adc = 0;
	unsigned short tdc = 0;
	unsigned short label_head;
	unsigned short label_tail;
	double ene = 0;
	unsigned short block[blocksize];
	unsigned short *pb;


	while (fread(block, sizeof(unsigned short), blocksize, fp)) {
		pb = block;
		// jump block header
		pb += sizeof(dataheader_)/2; 

		pb += 3; // jump to the last unsignede short number in a fired
		while (pb != (block + blocksize)) {
			pb += 4;

			// test if the end 
			if (*pb == 0x5e5e && *(pb+1) == 0x5e5e && *(pb-1) == 0x5e5e) {
				// cout << "end of a block..." << endl;
				break;
			}

			label_head = *pb >> 12;
			label_tail = *pb & 0x0FFF;

			if (label_head == 8) { // jump useless data
				continue;
			}
			else if (label_head == 12) {
				timestamp[0] = *(pb-3);
				timestamp[1] = *(pb-2);
				if ((((label_tail>>4) & 0x00F)%2) == 0) { // get the middle number of label_tail
					adc = *(pb-1) & 0x3FFF; 
				}
				else {
					tdc = *(pb-1) & 0x3FFF; 
				}
			}
			else { // to check if there is any other possibility ?
				// cout << "line 104... other label_head..." << endl;
				continue;
			}

			// caculate energy
			if (adc >= 2 && tdc == 0) {
				ene = getene(label_tail, adc, eff);
				h->Fill(ene);
			}

			adc = 0;
			tdc = 0;
		}
	}
	
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