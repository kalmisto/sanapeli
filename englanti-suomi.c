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
	char	rivi[BUFSIZ];

	sanatiedosto = fopen(SANA_TIEDOSTO, "r");
	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus epäonnistui\n");
		exit(1);
	}

	fprintf(stdout, "Tiedoston koko tavuina: %lld\n", (long long) get_filesize());
	/*
	while ((fgets(rivi, BUFSIZ - 1, sanatiedosto)) != NULL) {
		fprintf(stdout, "%s", rivi);
	}
	*/

	if (sanatiedosto != NULL)
		fclose(sanatiedosto);

	return 0;
}

/* Return size of the file 'fname */
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
