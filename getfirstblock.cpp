#include <stdio.h>
#include <stdlib.h>

int main()
{
	const unsigned short blocksize = 32768;
	FILE *fp1;

	if ((fp1 = fopen("../d17/R17_0", "rb")) == NULL) {
		printf("can not open the file.\n");
		return -1;
	}

	unsigned short block[blocksize] = {0};
	fread(block, sizeof(unsigned short), blocksize, fp1);
	fclose(fp1);


	FILE *fp2;

	if ((fp2 = fopen("../d17/R17_test", "w")) == NULL) {
		printf("can not open the file.\n");
		return -1;
	}

	fwrite(block, sizeof(unsigned short), blocksize, fp1);
	fclose(fp2);

	return 0;
}