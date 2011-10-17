#include <sys/stat.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "englanti-suomi.h"


int
main(int argc, const char *argv[])
{
	FILE	*sanatiedosto;
	char	 rivi[BUFSIZ];

	sanatiedosto = fopen(SANA_TIEDOSTO, "r");
	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus epäonnistui\n");
		exit(1);
	}

	fprintf(stdout, "Tiedoston rivien määrä: %zu\n", get_linecount(sanatiedosto));

	if (sanatiedosto != NULL)
		fclose(sanatiedosto);

	return 0;
}

size_t
get_filesize(void)
{
	struct stat sb;

	if (stat(SANA_TIEDOSTO, &sb) < 0) {
		fprintf(stderr, "Tiedoston avaus epäonnistui\n");
		exit(2);
	}

	return sb.st_size;
}

size_t
get_linecount(FILE *fname)
{
	int c;
	int n = 0;

	do {
		c = fgetc(fname);
		if (c == '\n')
			n++;
	} while (c != EOF);
	return n;
}

