#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

using std::ifstream; using std::cin; using std::cout;
using std::endl;

int getcalpar(double calpar[9][4][3])
{
	ifstream fp ("../d17/eneparcal/r17.txt");	

	if (!fp) {
		cout << "can not open the file." << endl;
		return -1;
	}

	double a[3]; // temp valiable

	while (!fp.eof()) {
		for (int i = 0; i < 4; i++) {
			for(int j = 0; j < 9; j++) {
				for (int k = 0; k < 3; k++) {
					fp >> a[k];
					calpar[j][i][k] = a[k];
				}
			}
		}
	} 
	return 0;
}


void getadc_tdc_timestamp(unsigned short *pb, unsigned short labei_tail, unsigned short adc[9][4], unsigned short tdc[9][4], long long etime[9][4][2], long long xtime[9][4][2]) 
{
	unsigned short clvnum = 0;
	unsigned short segnum = 0;

	// get clover number
	switch (labei_tail) {
		case 0x40 :
		case 0x41 :
		case 0x42 :
		case 0x43 :
		case 0x50 :
		case 0x51 :
		case 0x52 :
		case 0x53 : clvnum = 0;
			break;

		case 0x48 :
		case 0x49 :
		case 0x4a :
		case 0x4b :
		case 0x58 :
		case 0x59 :
		case 0x5a :
		case 0x5b : clvnum = 1;
			break;

		case 0x60 :
		case 0x61 :
		case 0x62 :
		case 0x63 :
		case 0x70 :
		case 0x71 :
		case 0x72 :
		case 0x73 : clvnum = 2;
			break;

		case 0x68 :
		case 0x69 :
		case 0x6a :
		case 0x6b :
		case 0x78 :
		case 0x79 :
		case 0x7a :
		case 0x7b : clvnum = 3;
			break;

		case 0x80 :
		case 0x81 :
		case 0x82 :
		case 0x83 :
		case 0x90 :
		case 0x91 :
		case 0x92 :
		case 0x93 : clvnum = 4;
			break;

		case 0x88 :
		case 0x89 :
		case 0x8a :
		case 0x8b :
		case 0x98 :
		case 0x99 :
		case 0x9a :
		case 0x9b : clvnum = 5;
			break;

		case 0xa0 :
		case 0xa1 :
		case 0xa2 :
		case 0xa3 :
		case 0xb0 :
		case 0xb1 :
		case 0xb2 :
		case 0xb3 : clvnum = 6; // this is a broken clover...
			break;

		case 0xa8 :
		case 0xa9 :
		case 0xaa :
		case 0xab :
		case 0xb8 :
		case 0xb9 :
		case 0xba :
		case 0xbb : clvnum = 7;
			break;

		case 0xc0 :
		case 0xc1 :
		case 0xc2 :
		case 0xc3 :
		case 0xd0 :
		case 0xd1 :
		case 0xd2 :
		case 0xd3 : clvnum = 8;
			break;

		default : 
				// cout << "something wrong in switch..." << endl;
				// printf("%04x  ", *pb);
				// printf("%04x  ", *(pb-3));
				// printf("\n");
			break;
	}

	// get segnum
	segnum = (labei_tail&0x000F) % 4; // 0, 1, 2, 3 corresponding to a, b, c, d
	if ((((labei_tail>>4) & 0x00F)%2) == 0) {
		adc[clvnum][segnum] = *(pb-1) & 0x3FFF;
		etime[clvnum][segnum][0] = *(pb-3); // 0-15bit
		etime[clvnum][segnum][1] = *(pb-2); // 16-31bit
	}  
	else {
		tdc[clvnum][segnum] = *(pb-3) & 0x3FFF;
		xtime[clvnum][segnum][0] = *(pb-3);
		xtime[clvnum][segnum][1] = *(pb-2);
	}
	
}


void addback(double calpar[9][4][3], unsigned short adc[9][4], unsigned short tdc[9][4], long long etime[9][4][2], double eaddback[9], long long taddback[9])
{
	double cyle[9][4];
	long long cylt[9][4];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 4; j++) {
			cyle[i][j] = 0.0;
			cylt[i][j] = 0;
		}
	}

	double random = (rand()%999 + 1) / 1000.0;
	for (int i = 0; i < 8; i++) {
		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]<=0) && (adc[i][2]<=0) && (adc[i][3]<=0)) { // 0
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2]; // calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;

			eaddback[i] = cyle[i][0];
			taddback[i] = cylt[i][0];
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][0]<=0) && (adc[i][2]<=0) && (adc[i][3]<=0)) { // 1
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;

			eaddback[i] = cyle[i][1];
			taddback[i] = cylt[i][1];
		}

		if ((adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][0]<=0) && (adc[i][1]<=0) && (adc[i][3]<=0)) { // 2
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][2];
			taddback[i] = cylt[i][2];
		}

		if ((adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0) && (adc[i][1]<=0) && (adc[i][2]<=0)) { // 3
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][3];
			taddback[i] = cylt[i][3];
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]<=0) && (adc[i][3]<=0)) { // 0+1
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][1];
			if (cyle[i][0] > cyle[i][1]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = cylt[i][1];
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][1]<=0) && (adc[i][3]<=0)) { // 0+2
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2]; // calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][2];
			if (cyle[i][0] > cyle[i][2]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = cylt[i][2];
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][1]<=0) && (adc[i][2]<=0)) { // 0+3
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2]; // calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][3];
			if (cyle[i][0] > cyle[i][3]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = cylt[i][3];
			}
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][0]<=0) && (adc[i][3]<=0)) { // 1+2
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][1] + cyle[i][2];
			if (cyle[i][1] > cyle[i][2]) {
				taddback[i] = cylt[i][1];
			}
			else {
				taddback[i] = cylt[i][2];
			}
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0) && (adc[i][2]<=0)) { // 1+3
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][1] + cyle[i][3];
			if (cyle[i][1] > cyle[i][3]) {
				taddback[i] = cylt[i][1];
			}
			else {
				taddback[i] = cylt[i][3];
			}
		}

		if ((adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0) && (adc[i][1]<=0)) { // 2+3
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][2] + cyle[i][3];
			if (cyle[i][2] > cyle[i][3]) {
				taddback[i] = cylt[i][2];
			}
			else {
				taddback[i] = cylt[i][3];
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]<=0)) { // 0+1+2
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][1] + cyle[i][2];
			if (cyle[i][0] > cyle[i][1] && cyle[i][0] > cyle[i][2]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = ((cyle[i][1] > cyle[i][2]) ? cyle[i][1] : cyle[i][2]);
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][2]<=0)) { // 0+1+3
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][1] + cyle[i][3];
			if (cyle[i][0] > cyle[i][1] && cyle[i][0] > cyle[i][3]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = ((cyle[i][1] > cyle[i][3]) ? cyle[i][1] : cyle[i][3]);
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][1]<=0)) { // 0+2+3
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][2] + cyle[i][3];
			if (cyle[i][0] > cyle[i][2] && cyle[i][0] > cyle[i][3]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = ((cyle[i][2] > cyle[i][3]) ? cyle[i][2] : cyle[i][3]);
			}
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0)) { // 1+2+3
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2];// calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][1] + cyle[i][2] + cyle[i][3];
			if (cyle[i][1] > cyle[i][2] && cyle[i][1] > cyle[i][3]) {
				taddback[i] = cylt[i][1];
			}
			else {
				taddback[i] = ((cyle[i][2] > cyle[i][3]) ? cyle[i][2] : cyle[i][3]);
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF)) { // 0+1+2+3
			cyle[i][0] = calpar[i][0][0] + adc[i][0]*calpar[i][0][1] + adc[i][0]*calpar[i][0][2]*calpar[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][1]<<16) + (etime[i][0][0])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = calpar[i][1][0] + adc[i][1]*calpar[i][1][1] + adc[i][1]*calpar[i][1][2]*calpar[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][1]<<16) + (etime[i][1][0])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = calpar[i][2][0] + adc[i][2]*calpar[i][2][1] + adc[i][2]*calpar[i][2][2]*calpar[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][1]<<16) + (etime[i][2][0])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = calpar[i][3][0] + adc[i][3]*calpar[i][3][1] + adc[i][3]*calpar[i][3][2]*calpar[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][1]<<16) + (etime[i][3][0])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][1] + cyle[i][2] + cyle[i][3];
			if (cyle[i][0] > cyle[i][1] && cyle[i][0] > cyle[i][2] && cyle[i][0] > cyle[i][3]) {
				taddback[i] = cylt[i][0];
			}
			else if (cyle[i][1] > cyle[i][0] && cyle[i][1] > cyle[i][2] && cyle[i][1] > cyle[i][3]) {
				taddback[i] = cylt[i][1];
			}
			else {
				taddback[i] = ((cyle[i][2] > cyle[i][3]) ? cyle[i][2] : cyle[i][3]);
			}
		}
	}
}