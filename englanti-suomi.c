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
	size_t	 rivilkm;
	size_t	 kysyttava;
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

	vaihtoehdot_idx = random_idx_arr(rivilkm);

	kysyttava = randint(0, KYS_LKM - 1);

	fprintf(stdout, "Sanapeli 1.0, englanti-suomi\n");
	fprintf(stdout, "Valitse seuraavalle suomenkielinen vastine (1-8)\n\n");
	fprintf(stdout, ">> %s\n\n",sanaparit[vaihtoehdot_idx[kysyttava]].eng); 
	fprintf(stdout, "\n 1. %s",sanaparit[vaihtoehdot_idx[1]].fin);
	fprintf(stdout, "\n 2. %s",sanaparit[vaihtoehdot_idx[2]].fin);
	fprintf(stdout, "\n 3. %s",sanaparit[vaihtoehdot_idx[3]].fin);
	fprintf(stdout, "\n 4. %s",sanaparit[vaihtoehdot_idx[4]].fin);
	fprintf(stdout, "\n 5. %s",sanaparit[vaihtoehdot_idx[5]].fin);
	fprintf(stdout, "\n 6. %s",sanaparit[vaihtoehdot_idx[6]].fin);
	fprintf(stdout, "\n 7. %s",sanaparit[vaihtoehdot_idx[7]].fin);
	fprintf(stdout, "\n 8. %s",sanaparit[vaihtoehdot_idx[8]].fin);

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

size_t *
random_idx_arr(size_t riveja) /* generoidaan satunnaislukuja (= valitaan satunnaisesti rivejä sanat.txt tiedostosta) */
{
	size_t 	 j = 0;
	size_t	 i = 0;
	size_t	*t = NULL;

	t = calloc(KYS_LKM, sizeof(size_t));
	if (t == NULL)
		err(1, "malloc failed in func: random_idx_arr()");

	for (i = 0; i < KYS_LKM; i++) {
		t[i] = randint(1, riveja);
		while (i != 0 && j < i) { /* tarkastetaan onko satunnaisluku saatu jo aikaisemmin */
			if (t[j] == t[i]) {
				t[i] = randint(1, riveja); /*jos satunnaisluku on saatu aikaisemmin, luodaan uusi */
				j--; /* koska while-silmukan j++ nostaa luvun haluttuun nollaan. eli taulukkoa aletaan taas käymään nolla-indeksista alkaen */
			}
			j++;
		}
	}

	return t;
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

