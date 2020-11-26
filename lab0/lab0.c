// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void segfault()
{
	char *ptr = NULL;
	*ptr = '0';
}

static void handler(int signum)
{
	fprintf(stderr, "Caught signal: %s\n", strerror(signum));
	exit(4);
}

int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"input", required_argument, 0, 'i'},
		{"output", required_argument, 0, 'o'},
		{"segfault", no_argument, 0, 's'},
		{"catch", no_argument, 0, 'c'},
		{0, 0, 0, 0}};

	int ch = 0, option_index = 0, i = 0, o = 0, s = 0, c = 0;
	char *input_file = "", *output_file = "";
	while ((ch = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
	{
		switch (ch)
		{
		case 'i':
			i = 1;
			input_file = optarg;
			break;
		case 'o':
			o = 1;
			output_file = optarg;
			break;
		case 's':
			s = 1;
			break;
		case 'c':
			c = 1;
			break;
		default:
			fprintf(stderr, "Invalid argument\n");
			exit(1);
		}
	}

	if (s)
	{
		if (c)
			signal(SIGSEGV, handler);
		segfault();
	}

	if (i)
	{
		int ifd = open(input_file, O_RDONLY);
		if (ifd < 0)
		{
			fprintf(stderr, "Unable to open input file: %s\n", strerror(errno));
			exit(2);
		}
		close(0);
		dup(ifd);
		close(ifd);
	}

	if (o)
	{
		int ofd = creat(output_file, 0666);
		if (ofd < 0)
		{
			fprintf(stderr, "Unable to open input file: %s\n", strerror(errno));
			exit(3);
		}
		close(1);
		dup(ofd);
		close(ofd);
	}

	int ret = 0;
	char buf;
	while ((ret = read(0, &buf, 1)) > 0)
		write(1, &buf, 1);

	return 0;
}
