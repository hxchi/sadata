#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

using std::ifstream; using std::cin; using std::cout;
using std::endl;

int geteff(double eff[9][4][3])
{
	ifstream fp ("../d17/eff/r17.txt");	

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
					eff[j][i][k] = a[k];
				}
			}
		}
	} 
	return 0;
}


double getene(unsigned short labei_tail, unsigned short adc, double eff[9][4][3]) 
{
	double ene = 0;
	unsigned short clonum = 0;
	unsigned short segnum = 0;

	// get clover number
	switch (labei_tail) {
		case 64 :
		case 65 :
		case 66 :
		case 67 : clonum = 0;
			break;

		case 72 :
		case 73 :
		case 74 :
		case 75 : clonum = 1;
			break;

		case 96 :
		case 97 :
		case 98 :
		case 99 : clonum = 2;
			break;

		case 104 :
		case 105 :
		case 106 :
		case 107 : clonum = 3;
			break;

		case 128 :
		case 129 :
		case 130 :
		case 131 : clonum = 4;
			break;

		case 136 :
		case 137 :
		case 138 :
		case 139 : clonum = 5;
			break;

		case 160 :
		case 161 :
		case 162 :
		case 163 : clonum = 6;
			break;

		case 168 :
		case 169 :
		case 170 :
		case 171 : clonum = 7;
			break;

		case 192 :
		case 193 :
		case 194 :
		case 195 : clonum = 8;
			break;

		default : cout << "something wrong in switch..." << endl;
			break;
	}

	// get segnum
	segnum = (labei_tail&0x000F) % 4;

	ene = eff[clonum][segnum][0] + adc * eff[clonum][segnum][1] + adc * adc * eff[clonum][segnum][2];
	return 2 * ene;
}