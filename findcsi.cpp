// find csi fired event

#include <stdio.h>
#include <stdlib.h>

int main()
{
	const unsigned short blocksize = 32768;

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

	unsigned short block[blocksize];
	unsigned short *pb;

	FILE *fp;
	
	if((fp=fopen("../d17/R51_0", "rb")) == NULL){
		printf("cannot open the file!\n");
		exit(EXIT_FAILURE);
	}

	fread(block, sizeof(unsigned short), blocksize, fp);
	pb = block; // for print 

	pb += sizeof(dataheader_)/sizeof(unsigned short); // jump data header

	pb += 3;
	// printf("%d\n", (*pb)&0x0FFF);  // for test, it should be 324(c144).
	unsigned short label;
	int k = 0;
	while (1) {
		label = (*pb)&0x0FFF;
		// printf("%04x\n", *pb);
		if (label >= 1920 && label <= 2175) {
			printf("find it.... %04x\n", *pb);
			printf("print 6 fired include this one.\n");
			pb = pb - 3;
			for (int i = 0; i < 6; i++) {
				printf("#%d: ", i);
				for (int j = 0; j < 4; j++) {
					printf("%04x  ", *pb);
					pb++;
				}
				printf("\n");
			}
			pb = pb - 1 - 8;
			k++;
		} 
		pb += 4;
		if (k == 2) {
			break;
		}
	}

	return 0;
}