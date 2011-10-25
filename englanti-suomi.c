#include <sys/stat.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "englanti-suomi.h"


int
main(int argc, const char *argv[])
{
	FILE	*sanatiedosto;
	char	 rivi[BUFSIZ];
	size_t	 sanalkm;
	struct sana *sanaparit;
	char	*c;

	sanatiedosto = fopen(SANA_TIEDOSTO, "r");
	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus epäonnistui\n");
		exit(1);
	}

	sanalkm = get_linecount(sanatiedosto);
	sanaparit = calloc(sanalkm, sizeof(struct sana));
	if (sanaparit == NULL)
		err(1, "malloc");
	strncpy(sanaparit[0].eng, "dog\0", 4);
	strncpy(sanaparit[0].fin, "koira\0", 6);
	char a;
	char b;
	strcmp(&a, &b);

	print_sanaparit(sanaparit, sanalkm);
	fprintf(stdout, "Tiedoston rivien määrä: %zu\n", sanalkm);

	if (sanaparit != NULL)
		free(sanaparit);
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

void
print_sanaparit(struct sana *sp, size_t koko)
{
	int i = 0;

	for (i = 0; i < koko; i++) {
		if (strlen(sp[i].eng) != 0 && strlen(sp[i].fin) != 0)
			fprintf(stdout, "%s = %s\n", sp[i].eng, sp[i].fin);
	}
}

