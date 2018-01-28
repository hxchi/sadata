#include <iostream>
#include <fstream>
#include <string>

using std::ifstream; using std::ofstream; 
using std::cout; using std::endl; using std::string;

int main(int argc, char *argv[])
{
	char filein[42], fileout[42]; 
	int startnum = 3;
	int endnum = 72;
	string s;
	double a[36][6];

	for (int i = startnum; i <= endnum; i++) {
		sprintf(filein, "r%d", i);
		sprintf(fileout, "r%d.txt", i);
		ifstream fpi (filein, ifstream::in);
		ofstream fpo (fileout, ofstream::out);

		if (!filein) {
			cout << "can not open r" <<i << " input file..." << endl;
			return 0;
		}
		if (!filein) {
			cout << "can not open r" <<i << " output file..." << endl;
			return 0;
		}

		getline(fpi, s); // do not need the first line 

		while (fpi.good()) { // needs to be 36 of the array baecause of fpi.good()
			for (int i = 0; i < 36; i++) {
				for (int j = 0; j < 6; j++) {
					fpi >> a[i][j];
				}
			}
		}
		
		for (int i = 0; i < 32; i++) {
				for (int j = 2; j <= 4; j++) {
					fpo << a[i][j] << "  ";
				}
				fpo << endl;
			}
		fpi.close();
		fpo.close();
	}
	
	return 0;
}