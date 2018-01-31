#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>


using std::cout; using std::endl; using std::ifstream;

int geteff(int runnumber, double eff[8][4][3])
{
	int count = 0;
	char filename[42];
	sprintf(filename, "./effpar/r%d.txt", runnumber);
	ifstream fp (filename);	

	if (!fp) {
		cout << "can not open the file." << endl;
		return -1;
	}

	double a[3]; // temp valiable

	while (count < 8*4*3) {
		for (int i = 0; i < 8; i++) {
			for(int j = 0; j < 4; j++) {
				for (int k = 0; k < 3; k++) {
					fp >> a[k];
					eff[i][j][k] = a[k];
					count++;
				}
			}
		}
	} 

	// test 
	// for (int i = 0; i < 8; i++) {
	// 	for(int j = 0; j < 4; j++) {
	// 		for (int k = 0; k < 3; k++) {
	// 			cout << eff[i][j][k] << " ";
	// 		}
	// 		cout << endl;
	// 	}
	// }
	return 0;
}

void getabtime(unsigned short *pb, long long etime[8][4][3], long long xtime[8][4][3])
{
	unsigned short time_head = *(pb) & 0xFF00;
	unsigned short time_tail = *(pb) & 0x00FF;
	if ((time_tail >= 1) && (time_tail <= 8)) {
		// printf("%04x\n", time_head);
		switch (time_head) {
			case 0x0000 :
				for (int s = 0; s < 3; s ++) {
					etime[time_tail-1][0][s] = *(pb + 1 + s);
				}
				break;
			case 0x0800 :
				for (int s = 0; s < 3; s ++) {
					etime[time_tail-1][1][s] = *(pb + 1 + s);
				}
				break;
			case 0x1000 :
				for (int s = 0; s < 3; s ++) {
					etime[time_tail-1][2][s] = *(pb + 1 + s);
				}
				break;
			case 0x1800 :
				for (int s = 0; s < 3; s ++) {
					etime[time_tail-1][3][s] = *(pb + 1 + s);
				}
				break;

			case 0x2000 :
				for (int s = 0; s < 3; s ++) {
					xtime[time_tail-1][0][s] = *(pb + 1 + s);
				}
				break;
			case 0x2800 :
				for (int s = 0; s < 3; s ++) {
					xtime[time_tail-1][1][s] = *(pb + 1 + s);
				}
				break;
			case 0x3000 :
				for (int s = 0; s < 3; s ++) {
					xtime[time_tail-1][2][s] = *(pb + 1 + s);
				}
				break;
			case 0x3800 :
				for (int s = 0; s < 3; s ++) {
					xtime[time_tail-1][3][s] = *(pb + 1 + s);
				}
				break;

			default : cout << "something wrong in switch..." << endl;
				break;
		}
	}
	// else {
	// 	cout << "......" << endl;
	// }
}


void addback(double eff[8][4][3], unsigned short adc[8][4], unsigned short tdc[8][4], long long etime[8][4][3], double eaddback[8], long long taddback[8])
{
	double cyle[8][4];
	long long cylt[8][4];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			cyle[i][j] = 0.0;
			cylt[i][j] = 0;
		}
	}

	double random = (rand()%999 + 1) / 1000.0;
	for (int i = 0; i < 8; i++) {
		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]<=0) && (adc[i][2]<=0) && (adc[i][3]<=0)) { // 0
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2]; // calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;

			eaddback[i] = cyle[i][0];
			taddback[i] = cylt[i][0];
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][0]<=0) && (adc[i][2]<=0) && (adc[i][3]<=0)) { // 1
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;

			eaddback[i] = cyle[i][1];
			taddback[i] = cylt[i][1];
		}

		if ((adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][0]<=0) && (adc[i][1]<=0) && (adc[i][3]<=0)) { // 2
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][2];
			taddback[i] = cylt[i][2];
		}

		if ((adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0) && (adc[i][1]<=0) && (adc[i][2]<=0)) { // 3
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][3];
			taddback[i] = cylt[i][3];
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]<=0) && (adc[i][3]<=0)) { // 0+1
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][1];
			if (cyle[i][0] > cyle[i][1]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = cylt[i][1];
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][1]<=0) && (adc[i][3]<=0)) { // 0+2
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2]; // calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][2];
			if (cyle[i][0] > cyle[i][2]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = cylt[i][2];
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][1]<=0) && (adc[i][2]<=0)) { // 0+3
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2]; // calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][3];
			if (cyle[i][0] > cyle[i][3]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = cylt[i][3];
			}
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][0]<=0) && (adc[i][3]<=0)) { // 1+2
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][1] + cyle[i][2];
			if (cyle[i][1] > cyle[i][2]) {
				taddback[i] = cylt[i][1];
			}
			else {
				taddback[i] = cylt[i][2];
			}
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0) && (adc[i][2]<=0)) { // 1+3
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][1] + cyle[i][3];
			if (cyle[i][1] > cyle[i][3]) {
				taddback[i] = cylt[i][1];
			}
			else {
				taddback[i] = cylt[i][3];
			}
		}

		if ((adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0) && (adc[i][1]<=0)) { // 2+3
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][2] + cyle[i][3];
			if (cyle[i][2] > cyle[i][3]) {
				taddback[i] = cylt[i][2];
			}
			else {
				taddback[i] = cylt[i][3];
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]<=0)) { // 0+1+2
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][1] + cyle[i][2];
			if (cyle[i][0] > cyle[i][1] && cyle[i][0] > cyle[i][2]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = ((cyle[i][1] > cyle[i][2]) ? cyle[i][1] : cyle[i][2]);
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][2]<=0)) { // 0+1+3
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][1] + cyle[i][3];
			if (cyle[i][0] > cyle[i][1] && cyle[i][0] > cyle[i][3]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = ((cyle[i][1] > cyle[i][3]) ? cyle[i][1] : cyle[i][3]);
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][1]<=0)) { // 0+2+3
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][0] + cyle[i][2] + cyle[i][3];
			if (cyle[i][0] > cyle[i][2] && cyle[i][0] > cyle[i][3]) {
				taddback[i] = cylt[i][0];
			}
			else {
				taddback[i] = ((cyle[i][2] > cyle[i][3]) ? cyle[i][2] : cyle[i][3]);
			}
		}

		if ((adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF) && (adc[i][0]<=0)) { // 1+2+3
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2];// calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

			eaddback[i] = cyle[i][1] + cyle[i][2] + cyle[i][3];
			if (cyle[i][1] > cyle[i][2] && cyle[i][1] > cyle[i][3]) {
				taddback[i] = cylt[i][1];
			}
			else {
				taddback[i] = ((cyle[i][2] > cyle[i][3]) ? cyle[i][2] : cyle[i][3]);
			}
		}

		if ((adc[i][0]>0) && (adc[i][0]<0xFFFF) && (adc[i][1]>0) && (adc[i][1]<0xFFFF) && (adc[i][2]>0) && (adc[i][2]<0xFFFF) && (adc[i][3]>0) && (adc[i][3]<0xFFFF)) { // 0+1+2+3
			cyle[i][0] = eff[i][0][0] + adc[i][0]*eff[i][0][1] + adc[i][0]*eff[i][0][2]*eff[i][0][2];// calculation
			cylt[i][0] = 10 * ((etime[i][0][0]<<32) + (etime[i][0][1]<<16) + (etime[i][0][2])) - 5 + 10 * tdc[i][0] / 65536.0;
			cyle[i][1] = eff[i][1][0] + adc[i][1]*eff[i][1][1] + adc[i][1]*eff[i][1][2]*eff[i][1][2]; // calculation
			cylt[i][1] = 10 * ((etime[i][1][0]<<32) + (etime[i][1][1]<<16) + (etime[i][1][2])) - 5 + 10 * tdc[i][1] / 65536.0;
			cyle[i][2] = eff[i][2][0] + adc[i][2]*eff[i][2][1] + adc[i][2]*eff[i][2][2]*eff[i][2][2]; // calculation
			cylt[i][2] = 10 * ((etime[i][2][0]<<32) + (etime[i][2][1]<<16) + (etime[i][2][2])) - 5 + 10 * tdc[i][2] / 65536.0;
			cyle[i][3] = eff[i][3][0] + adc[i][3]*eff[i][3][1] + adc[i][3]*eff[i][3][2]*eff[i][3][2]; // calculation
			cylt[i][3] = 10 * ((etime[i][3][0]<<32) + (etime[i][3][1]<<16) + (etime[i][3][2])) - 5 + 10 * tdc[i][3] / 65536.0;

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