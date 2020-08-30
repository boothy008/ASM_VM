#include <cstdio>
#include <cstdlib>

#include "Hardware/Tiny402.h"

int main(int argc, char *argv[])
{
	Tiny402 tiny{};
	
	if (argc < 2)
	{
		printf("AVR [image-file1] ...\n");
		exit(2);
	}

	tiny.load_software(argv[1]);
	
	return 0;
}