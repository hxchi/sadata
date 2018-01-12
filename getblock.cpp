// test block size of 2017 iThemba's data

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
	
	printf("the first block: \n");
	for(int i = 0; i < (int)(sizeof(dataheader_) / sizeof(unsigned short)); i++){
		printf("%04x  ", *pb);
		pb += 1;
	}
	printf("\n");
	
	printf("the first fired: \n");
	for(int i = 0; i < 4; i++){
		printf("%04x  ", *pb);
		pb += 1;
	}
	printf("\n\n");
	
	printf("the second block: \n");
	fread(block, sizeof(unsigned short), blocksize, fp);
	pb = block;
	
	for(int i = 0; i < (int)(sizeof(dataheader_) / sizeof(unsigned short)); i++){
		printf("%04x  ", *pb);
		pb += 1;
	}
	printf("\n");

	printf("the first fired: \n");
	for(int i = 0; i < 4; i++){
		printf("%04x  ", *pb);
		pb += 1;
	}
	printf("\n");

	return 0;
}