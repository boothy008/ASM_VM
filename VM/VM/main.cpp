#include <csignal>

#include "Hardware/LC_3.h"

DWORD fdwMode, fdwOldMode;
HANDLE hStdin;

void disable_input_buffering()
{
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hStdin, &fdwOldMode);
	fdwMode = fdwOldMode
		^ ENABLE_ECHO_INPUT //No input echo
		^ ENABLE_LINE_INPUT; //Return when one of more characters are available
	SetConsoleMode(hStdin, fdwMode);
	FlushConsoleInputBuffer(hStdin);
}

void restore_input_buffering()
{
	SetConsoleMode(hStdin, fdwOldMode);
}

void handle_interrupt(int signal)
{
	restore_input_buffering();
	printf("\n");
	exit(-2);
}

int main(int argc, char *argv[])
{
	//Setup
	signal(SIGINT, handle_interrupt);
	disable_input_buffering();

	LC_3 lc3(hStdin);

	if (argc < 2)
	{
		printf("lc3 [image-file1] ...\n");
		exit(2);
	}

	for (int j = 1; j < argc; ++j)
	{
		if (!lc3.read_image(argv[j]))
		{
			printf("failed to load image: %s\n", argv[j]);
			exit(1);
		}
	}
	
	lc3.start();

	restore_input_buffering();
}