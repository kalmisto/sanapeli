#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


int
main(int argc, const char *argv[])
{
	FILE	*sanatiedosto;
	char	rivi[BUFSIZ];

	sanatiedosto = fopen("/home/kalle/programs/sanastoja/sanoja.txt", "r");
	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus epäonnistui\n");
		exit(1);
	}

	while ((fgets(rivi, BUFSIZ - 1, sanatiedosto)) != NULL) {
		fprintf(stdout, "%s", rivi);
	}

	if (sanatiedosto != NULL)
		fclose(sanatiedosto);

	return 0;
}
