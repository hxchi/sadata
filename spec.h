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


void getadc_tdc_timestamp(unsigned short *pb, unsigned short labei_tail, unsigned short adc[9][4], unsigned short tdc[9][4], long long etime[9][4][2], long long xtime[9][4][2], unsigned short qdc[64][4], long long qtime[64][4][2]) 
{
	unsigned short clvnum = 0;
	unsigned short segnum = 0;

	unsigned short csinum = 0;
	unsigned short qnum = 0;

	// get clover number
	switch (labei_tail) {
		// for clover
		case 0x040 :
		case 0x041 :
		case 0x042 :
		case 0x043 :
		case 0x050 :
		case 0x051 :
		case 0x052 :
		case 0x053 : clvnum = 0;
			break;

		case 0x048 :
		case 0x049 :
		case 0x04a :
		case 0x04b :
		case 0x058 :
		case 0x059 :
		case 0x05a :
		case 0x05b : clvnum = 1;
			break;

		case 0x060 :
		case 0x061 :
		case 0x062 :
		case 0x063 :
		case 0x070 :
		case 0x071 :
		case 0x072 :
		case 0x073 : clvnum = 2;
			break;

		case 0x068 :
		case 0x069 :
		case 0x06a :
		case 0x06b :
		case 0x078 :
		case 0x079 :
		case 0x07a :
		case 0x07b : clvnum = 3;
			break;

		case 0x080 :
		case 0x081 :
		case 0x082 :
		case 0x083 :
		case 0x090 :
		case 0x091 :
		case 0x092 :
		case 0x093 : clvnum = 4;
			break;

		case 0x088 :
		case 0x089 :
		case 0x08a :
		case 0x08b :
		case 0x098 :
		case 0x099 :
		case 0x09a :
		case 0x09b : clvnum = 5;
			break;

		case 0x0a0 :
		case 0x0a1 :
		case 0x0a2 :
		case 0x0a3 :
		case 0x0b0 :
		case 0x0b1 :
		case 0x0b2 :
		case 0x0b3 : clvnum = 6; // this is a broken clover...
			break;

		case 0x0a8 :
		case 0x0a9 :
		case 0x0aa :
		case 0x0ab :
		case 0x0b8 :
		case 0x0b9 :
		case 0x0ba :
		case 0x0bb : clvnum = 7;
			break;

		case 0x0c0 :
		case 0x0c1 :
		case 0x0c2 :
		case 0x0c3 :
		case 0x0d0 :
		case 0x0d1 :
		case 0x0d2 :
		case 0x0d3 : clvnum = 8;
			break;

		// for csi 
		case 0x780 :
		case 0x781 :
		case 0x782 :
		case 0x783 : csinum = 0;
			break;

		case 0x784 :
		case 0x785 :
		case 0x786 :
		case 0x787 : csinum = 1;
			break;

		case 0x788 :
		case 0x789 :
		case 0x78a :
		case 0x78b : csinum = 2;
			break;

		case 0x78c :
		case 0x78d :
		case 0x78e :
		case 0x78f : csinum = 3;
			break;

		case 0x790 :
		case 0x791 :
		case 0x792 :
		case 0x793 : csinum = 4;
			break;

		case 0x794 :
		case 0x795 :
		case 0x796 :
		case 0x797 : csinum = 5;
			break;

		case 0x798 :
		case 0x799 :
		case 0x79a :
		case 0x79b : csinum = 6;
			break;

		case 0x79c :
		case 0x79d :
		case 0x79e :
		case 0x79f : csinum = 7;
			break;

		case 0x7a0 :
		case 0x7a1 :
		case 0x7a2 :
		case 0x7a3 : csinum = 8;
			break;

		case 0x7a4 :
		case 0x7a5 :
		case 0x7a6 :
		case 0x7a7 : csinum = 9;
			break;

		case 0x7a8 :
		case 0x7a9 :
		case 0x7aa :
		case 0x7ab : csinum = 10;
			break;

		case 0x7ac :
		case 0x7ad :
		case 0x7ae :
		case 0x7af : csinum = 11;
			break;

		case 0x7b0 :
		case 0x7b1 :
		case 0x7b2 :
		case 0x7b3 : csinum = 12;
			break;

		case 0x7b4 :
		case 0x7b5 :
		case 0x7b6 :
		case 0x7b7 : csinum = 13;
			break;

		case 0x7b8 :
		case 0x7b9 :
		case 0x7ba :
		case 0x7bb : csinum = 14;
			break;

		case 0x7bc :
		case 0x7bd :
		case 0x7be :
		case 0x7bf : csinum = 15;
			break;

		case 0x7c0 :
		case 0x7c1 :
		case 0x7c2 :
		case 0x7c3 : csinum = 16;
			break;

		case 0x7c4 :
		case 0x7c5 :
		case 0x7c6 :
		case 0x7c7 : csinum = 17;
			break;

		case 0x7c8 :
		case 0x7c9 :
		case 0x7ca :
		case 0x7cb : csinum = 18;
			break;

		case 0x7cc :
		case 0x7cd :
		case 0x7ce :
		case 0x7cf : csinum = 19;
			break;

		case 0x7d0 :
		case 0x7d1 :
		case 0x7d2 :
		case 0x7d3 : csinum = 20;
			break;

		case 0x7d4 :
		case 0x7d5 :
		case 0x7d6 :
		case 0x7d7 : csinum = 21;
			break;

		case 0x7d8 :
		case 0x7d9 :
		case 0x7da :
		case 0x7db : csinum = 22;
			break;

		case 0x7dc :
		case 0x7dd :
		case 0x7de :
		case 0x7df : csinum = 23;
			break;

		case 0x7e0 :
		case 0x7e1 :
		case 0x7e2 :
		case 0x7e3 : csinum = 24;
			break;

		case 0x7e4 :
		case 0x7e5 :
		case 0x7e6 :
		case 0x7e7 : csinum = 25;
			break;

		case 0x7e8 :
		case 0x7e9 :
		case 0x7ea :
		case 0x7eb : csinum = 26;
			break;

		case 0x7ec :
		case 0x7ed :
		case 0x7ee :
		case 0x7ef : csinum = 27;
			break;

		case 0x7f0 :
		case 0x7f1 :
		case 0x7f2 :
		case 0x7f3 : csinum = 28;
			break;

		case 0x7f4 :
		case 0x7f5 :
		case 0x7f6 :
		case 0x7f7 : csinum = 29;
			break;

		case 0x7f8 :
		case 0x7f9 :
		case 0x7fa :
		case 0x7fb : csinum = 30;
			break;

		case 0x7fc :
		case 0x7fd :
		case 0x7fe :
		case 0x7ff : csinum = 31;
			break;

		case 0x800 :
		case 0x801 :
		case 0x802 :
		case 0x803 : csinum = 32;
			break;

		case 0x804 :
		case 0x805 :
		case 0x806 :
		case 0x807 : csinum = 33;
			break;

		case 0x808 :
		case 0x809 :
		case 0x80a :
		case 0x80b : csinum = 34;
			break;

		case 0x80c :
		case 0x80d :
		case 0x80e :
		case 0x80f : csinum = 35;
			break;

		case 0x810 :
		case 0x811 :
		case 0x812 :
		case 0x813 : csinum = 36;
			break;

		case 0x814 :
		case 0x815 :
		case 0x816 :
		case 0x817 : csinum = 37;
			break;

		case 0x818 :
		case 0x819 :
		case 0x81a :
		case 0x81b : csinum = 38;
			break;

		case 0x81c :
		case 0x81d :
		case 0x81e :
		case 0x81f : csinum = 39;
			break;

		case 0x820 :
		case 0x821 :
		case 0x822 :
		case 0x823 : csinum = 40;
			break;

		case 0x824 :
		case 0x825 :
		case 0x826 :
		case 0x827 : csinum = 41;
			break;

		case 0x828 :
		case 0x829 :
		case 0x82a :
		case 0x82b : csinum = 42;
			break;

		case 0x82c :
		case 0x82d :
		case 0x82e :
		case 0x82f : csinum = 43;
			break;

		case 0x830 :
		case 0x831 :
		case 0x832 :
		case 0x833 : csinum = 44;
			break;

		case 0x834 :
		case 0x835 :
		case 0x836 :
		case 0x837 : csinum = 45;
			break;

		case 0x838 :
		case 0x839 :
		case 0x83a :
		case 0x83b : csinum = 46;
			break;

		case 0x83c :
		case 0x83d :
		case 0x83e :
		case 0x83f : csinum = 47;
			break;

		case 0x840 :
		case 0x841 :
		case 0x842 :
		case 0x843 : csinum = 48;
			break;

		case 0x844 :
		case 0x845 :
		case 0x846 :
		case 0x847 : csinum = 49;
			break;

		case 0x848 :
		case 0x849 :
		case 0x84a :
		case 0x84b : csinum = 50;
			break;

		case 0x84c :
		case 0x84d :
		case 0x84e :
		case 0x84f : csinum = 51;
			break;

		case 0x850 :
		case 0x851 :
		case 0x852 :
		case 0x853 : csinum = 52;
			break;

		case 0x854 :
		case 0x855 :
		case 0x856 :
		case 0x857 : csinum = 53;
			break;

		case 0x858 :
		case 0x859 :
		case 0x85a :
		case 0x85b : csinum = 54;
			break;

		case 0x85c :
		case 0x85d :
		case 0x85e :
		case 0x85f : csinum = 55;
			break;

		case 0x860 :
		case 0x861 :
		case 0x862 :
		case 0x863 : csinum = 56;
			break;

		case 0x864 :
		case 0x865 :
		case 0x866 :
		case 0x867 : csinum = 57;
			break;

		case 0x868 :
		case 0x869 :
		case 0x86a :
		case 0x86b : csinum = 58;
			break;

		case 0x86c :
		case 0x86d :
		case 0x86e :
		case 0x86f : csinum = 59;
			break;

		case 0x870 :
		case 0x871 :
		case 0x872 :
		case 0x873 : csinum = 60;
			break;

		case 0x874 :
		case 0x875 :
		case 0x876 :
		case 0x877 : csinum = 61;
			break;

		case 0x878 :
		case 0x879 :
		case 0x87a :
		case 0x87b : csinum = 62;
			break;

		case 0x87c :
		case 0x87d :
		case 0x87e :
		case 0x87f : csinum = 63;
			break;


		default : 
				// cout << "something wrong in switch..." << endl;
				// printf("%04x  ", *pb);
				// printf("%04x  ", *(pb-3));
				// printf("\n");
			break;
	}

	// get segnum
	if (labei_tail >= 0x0040 && labei_tail <= 0x00d3) { // if clover fired
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

	else if (labei_tail >= 0x0780 && labei_tail <= 0x087f) { // if csi fired
		qnum = (labei_tail%4); // get qnum from 0 to 3
		qdc[csinum][qnum] = *(pb-1) & 0x3FFF;
		qtime[csinum][qnum][0] = *(pb-3); // 0-15bit
		qtime[csinum][qnum][1] = *(pb-2); // 16-31bit
	}
	
	else {
		// labr or lips fired, do nothing... 
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