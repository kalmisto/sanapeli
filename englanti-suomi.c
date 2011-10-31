#include <sys/stat.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "englanti-suomi.h"


int
main(int argc, const char *argv[])
{
	FILE	*sanatiedosto;
	char	 rivi[BUFSIZ];
	int	 i = 0;
	size_t	 rivilkm;
	struct sana *sanaparit;

	srand(time(NULL));

	sanatiedosto = fopen(SANA_TIEDOSTO, "r"); /* avataan sanat.txt sanatiedosto kahvaan */
	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus epäonnistui\n");
		exit(1);
	}

	rivilkm = get_linecount(sanatiedosto); /*laskee sanatiedoston rivien lukumäärän*/
	sanaparit = calloc(rivilkm, sizeof(struct sana)); /* varataan muistia sanalkm * sizeof(...) */
	if (sanaparit == NULL) /* NULL viittaa tyhjään eli muistipaikkoja ei ole olemassa */
		err(1, "muistin varaus epäonnistui");


	rewind(sanatiedosto);
	while (fgets(rivi, BUFSIZ, sanatiedosto) != NULL) {
		sanaparit[i] = wordsplitter(rivi);
		if (sanaparit[i].eng[0] == '\0' || sanaparit[i].fin[0] == '\0')
			err(3, "Apua apua, NULL rivillä %d\n", i+1);
		i++;
	}
	i = randint(0, rivilkm);
	printf("%s %s", sanaparit[i].eng, sanaparit[i].fin);
	/*
	print_sanaparit(sanaparit, rivilkm);
	fprintf(stdout, "Tiedoston rivien määrä: %zu\n", rivilkm);
	*/
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

size_t
randint(size_t alku, size_t loppu)
{
	return (rand() % loppu + 1);
}


struct sana
wordsplitter(char *rivi) 
{
	char	*loppuosa;
	struct sana leikattu;

	memset(&leikattu, 0, sizeof(leikattu));

	if (strchr(rivi, '=') == NULL) {
		return leikattu;
	}

	strtok_r(rivi, "=", &loppuosa);
	strncpy(leikattu.eng, rivi, WORDLEN);
	strncpy(leikattu.fin, loppuosa, WORDLEN);

	return leikattu; /* 0 == virhe */

}

void
print_sanaparit(struct sana *sp, size_t koko)
{
	int i = 0;

	for (i = 0; i < koko; i++) {
		if (strlen(sp[i].eng) > 0 && strlen(sp[i].fin) > 0)
			fprintf(stdout, "%s = %s\n", sp[i].eng, sp[i].fin);
	}
}

