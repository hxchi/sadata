// for 2014 threefold file, copy 1.8G from a file...
// blocksize is 16384...


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	FILE *fpin;
	FILE *fpout;
	if ((fpin=fopen("r42", "rb")) == NULL) {
		printf("can not open the file......\n");
		return 0;
	}
	if ((fpout=fopen("r42_2g", "wb")) == NULL) {
		printf("can not creat the file...\n");
		return 0;
	}

	struct stat stabuff;
	if (stat("r42_2g", &stabuff) < 0) {
		printf("can not open the file...\n");
		return 0;
	}

	unsigned short block[16384];

	while ((stabuff.st_size / 1024.0 / 1024.0) <= 1800) {
		fread(block, sizeof(unsigned short), 16384, fpin);
		fwrite(block, sizeof(unsigned short), 16384, fpout);
		stat("r42_2g", &stabuff);
	}

	return 0;
}