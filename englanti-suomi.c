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
	FILE	*sanatiedosto = NULL;
	char	 rivi[BUFSIZ];
	int	 i = 0;
	size_t	*vaihtoehdot_idx = NULL;
	size_t 	 vastaus_input;
	size_t	 rivilkm;
	size_t	 kysyttava;
	size_t	 pisteet;
	struct sana *sanaparit;

	srand(time(NULL)); /*  alustetaan satunnaislukugeneraattori */

	sanatiedosto = fopen(SANA_TIEDOSTO, "r"); /* avataan sanat.txt sanatiedosto kahvaan */

	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus epäonnistui\n");
		exit(1);
	}

	rivilkm = get_linecount(sanatiedosto); /*laskee sanatiedoston rivien lukumäärän*/

	sanaparit = calloc(rivilkm, sizeof(struct sana)); /* varataan muistia sanalkm * sizeof(...) */


	if (sanaparit == NULL) /* NULL viittaa tyhjään eli muistipaikkoja ei ole olemassa */
		err(1, "muistin varaus epäonnistui");

	while (fgets(rivi, BUFSIZ, sanatiedosto) != NULL) {
		sanaparit[i] = wordsplitter(rivi);
		if (sanaparit[i].eng[0] == '\0' || sanaparit[i].fin[0] == '\0')
			err(3, "Apua apua, NULL rivillä %d\n", i+1);
		i++;
	}
	if (sanatiedosto != NULL) /* voidaan sulkea tässä vaiheessa */
		fclose(sanatiedosto);

	i = 1;
	pisteet = 0;
	while (i <= PELILKM) {
		vaihtoehdot_idx = random_idx_arr(rivilkm);

		kysyttava = randint(0, KYS_LKM - 1);

		fprintf(stdout, "Valitse seuraavalle suomenkielinen vastine (1-8)\n\n");
		fprintf(stdout, ">> %s\n",sanaparit[vaihtoehdot_idx[kysyttava]].eng);

		print_vaihtoehdot(sanaparit, vaihtoehdot_idx);

		fprintf(stdout, "[%zu/%zu] %zu > ", i, PELILKM, pisteet);
		fscanf(stdin, "%zu", &vastaus_input);
		if (vastaus_input == (kysyttava + 1)) {
			fprintf(stdout, "Oikein \n");
			pisteet++;
		} else {
			fprintf(stdout, "Väärin \n");
		}
		fprintf(stdout, "%s = ", sanaparit[vaihtoehdot_idx[kysyttava]].eng);
		fprintf(stdout, "%s\n", sanaparit[vaihtoehdot_idx[kysyttava]].fin);

		vastaus_input = 0;
		i++;
	}

	if (sanaparit != NULL)
		free(sanaparit);
	if (vaihtoehdot_idx != NULL)
		free(vaihtoehdot_idx);
	sanaparit = NULL;
	vaihtoehdot_idx = NULL;

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
	rewind(fname); /* "tiedostotela" palautetaan alkuun (getlinecount vei telan loppuun) */

	return n;
}

size_t
randint(size_t alku, size_t loppu)
{
	return (rand() % loppu + 1);
}

/* Löydyykö numer 'vertailtava' taulukosta t.
 * Palauttaa 1, jos löytyy
 * 0 muutoin
 */
int
numberexist(size_t *t, size_t koko, size_t vertailtava)
{
	size_t	i = 0;

	for (i = 0; i < koko; i++) {
		if (t[i] == vertailtava)
			return 1;
	}
	return 0;
}

size_t *
random_idx_arr(size_t riveja) /* generoidaan satunnaislukuja (= valitaan satunnaisesti rivejä sanat.txt tiedostosta) */
{
	size_t	 i = 0;
	size_t	*t = NULL;

	t = calloc(KYS_LKM, sizeof(size_t));
	if (t == NULL)
		err(1, "malloc failed in func: random_idx_arr()");

	for (i = 0; i < KYS_LKM; i++) {
		t[i] = randint(1, riveja - 1);
		while (numberexist(t, i, t[i]) == 1) {
			t[i] = randint(1, riveja - 1);
		}
	}

	return t;
}

struct sana
wordsplitter(char *rivi)
{
	char	*loppuosa;
	char	*p;
	struct sana leikattu;

	memset(&leikattu, 0, sizeof(leikattu));

	if (strchr(rivi, '=') == NULL) {
		return leikattu;
	}

	strtok_r(rivi, "=", &loppuosa);
	strncpy(leikattu.eng, rivi, WORDLEN);
	if ((p = strchr(leikattu.eng, '\n')) != NULL)
		*p = '\0';

	leikattu.eng[WORDLEN - 1] = '\0';
	strncpy(leikattu.fin, loppuosa, WORDLEN);
	leikattu.fin[WORDLEN - 1] = '\0';
	if ((p = strchr(leikattu.fin, '\n')) != NULL)
		*p = '\0';

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

void
print_vaihtoehdot(struct sana *sp, size_t *idx)
{
	size_t	i = 0;

	for (i = 0; i < KYS_LKM; i++)
		fprintf(stdout, "%2zu. %s\n", i + 1, sp[idx[i]].fin); /* sp == sanaparit, idx == vaihtoehdot_idx */
}

