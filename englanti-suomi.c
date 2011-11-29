#include <sys/stat.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <bsd/stdlib.h>

#include "englanti-suomi.h"


int
main(int argc, const char *argv[])
{
	FILE	*sanatiedosto = NULL;
	char	 rivi[BUFSIZ];
	int	 i = 0;
	size_t	 rivilkm = 0;
	size_t	 pisteet;
	struct sana_st *sanaparit;
	struct high_score_st *hightaulu;

	sanatiedosto = fopen(SANA_TIEDOSTO, "r"); /* avataan sanat.txt sanatiedosto kahvaan */

	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus ep‰onnistui\n");
		exit(1);
	}

	rivilkm = get_linecount(sanatiedosto); /*laskee sanatiedoston rivien lukum‰‰r‰n*/

	sanaparit = calloc(rivilkm, sizeof(struct sana_st)); /* varataan muistia sanalkm * sizeof(...) */

	if (sanaparit == NULL) /* NULL viittaa tyhj‰‰n eli muistipaikkoja ei ole olemassa */
		err(1, "muistin varaus ep‰onnistui");

	while (fgets(rivi, BUFSIZ, sanatiedosto) != NULL) {
		sanaparit[i] = wordsplitter(rivi);
		if (sanaparit[i].eng[0] == '\0' || sanaparit[i].fin[0] == '\0')
			err(3, "Apua apua, NULL rivill‰ %d\n", i+1);
		i++;
	}

	pisteet = pelikierros(sanaparit, PELILKM, rivilkm);
	hightaulu = luo_highscoretaulu();
	for (i = 0; i <= 10; i++) {
		fprintf(stdout, "ekatsekki %zu\n", hightaulu[i].pojot);
	}
	tallenna_scoret(pisteet, hightaulu);
	for (i = 0; i <= 10; i++) {
		fprintf(stdout, "tokatsekki %zu\n", hightaulu[i].pojot);
	}

	fprintf(stdout, "Pisteesi: %zu/%zu\n", pisteet, PELILKM);

/*	print_highscores(); */

	if (sanatiedosto != NULL) /* voidaan sulkea t‰ss‰ vaiheessa */
		fclose(sanatiedosto);

	if (sanaparit != NULL)
		free(sanaparit);
	sanaparit = NULL;

	return 0;
}

size_t
pelikierros(struct sana_st *sanaparit, size_t kierroslkm, size_t rivit)
{
	size_t	 i = 1;
	size_t	 pojot = 0;
	size_t	*vaihtoehdot_idx = NULL;
	size_t   kysyttava;
	size_t	 vastaus_input;

	while (i <= kierroslkm) {
		vaihtoehdot_idx = random_idx_arr(rivit);

		kysyttava = randint(0, KYS_LKM - 1);

		fprintf(stdout, "Valitse seuraavalle suomenkielinen vastine (1-8)\n\n");
		fprintf(stdout, ">> %s\n",sanaparit[vaihtoehdot_idx[kysyttava]].eng);

		print_vaihtoehdot(sanaparit, vaihtoehdot_idx);

		fprintf(stdout, "[%zu/%zu] %zu > ", i, kierroslkm, pojot);
		fscanf(stdin, "%zu", &vastaus_input);
		if (vastaus_input == (kysyttava + 1)) {
			fprintf(stdout, "Oikein, ");
			pojot++;
		} else {
			fprintf(stdout, "V‰‰rin, ");
		}
		fprintf(stdout, "%s = ", sanaparit[vaihtoehdot_idx[kysyttava]].eng);
		fprintf(stdout, "%s\n", sanaparit[vaihtoehdot_idx[kysyttava]].fin);

		vastaus_input = 0;
		i++;
	}
	if (vaihtoehdot_idx != NULL)
		free(vaihtoehdot_idx);
	vaihtoehdot_idx = NULL;

	return pojot;
}



size_t
get_filesize(void)
{
	struct stat sb;

	if (stat(SANA_TIEDOSTO, &sb) < 0) {
		fprintf(stderr, "Tiedoston avaus ep‰onnistui\n");
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
	return (arc4random() % loppu + 1);
}

/* Lˆydyykˆ numer 'vertailtava' taulukosta t.
 * Palauttaa 1, jos lˆytyy
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
random_idx_arr(size_t riveja) /* generoidaan satunnaislukuja (= valitaan satunnaisesti rivej‰ sanat.txt tiedostosta) */
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

struct sana_st
wordsplitter(char *rivi)
{
	char	*loppuosa;
	char	*p;
	struct sana_st leikattu;

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
print_sanaparit(struct sana_st *sp, size_t koko)
{
	int i = 0;

	for (i = 0; i < koko; i++) {
		if (strlen(sp[i].eng) > 0 && strlen(sp[i].fin) > 0)
			fprintf(stdout, "%s = %s\n", sp[i].eng, sp[i].fin);
	}
}

void
print_vaihtoehdot(struct sana_st *sp, size_t *idx)
{
	size_t	i = 0;

	for (i = 0; i < KYS_LKM; i++)
		fprintf(stdout, "%2zu. %s\n", i + 1, sp[idx[i]].fin); /* sp == sanaparit, idx == vaihtoehdot_idx */
}

struct high_score_st *
luo_highscoretaulu(void)
{
	FILE			*h_scoret = NULL;
	struct high_score_st	*highscoretaulu;
	struct stat		 st;
	size_t			 i = 0;

	highscoretaulu = calloc(11, sizeof(struct high_score_st));
	if (highscoretaulu == NULL) /* NULL viittaa tyhj‰‰n eli muistipaikkoja ei ole olemassa */
		err(1, "muistinvaraus ep‰onnistui");


	if (stat(SCORES, &st) == -1) { /* Tiedostoa ei lˆydy */
		h_scoret = fopen(SCORES, "w");
		if (h_scoret == NULL) {
			err(10, "tiedostoa ei lˆytynyt %s", SCORES);
		}
		for (i = 0; i < 11; i++) {
			fputs("1,#,2\n", h_scoret);
		}
		fclose(h_scoret);
	}

	h_scoret = fopen(SCORES, "r");
	if (h_scoret == NULL) {
		err(10, "tiedostoa ei lˆytynyt %s", SCORES);
	}
	for (i = 0; i < 10; i++) {
		size_t luettu = 0;

		luettu = fscanf(h_scoret, "%zu,%s,%zu", &highscoretaulu[i].aika, &highscoretaulu[i].nimi, &highscoretaulu[i].pojot);
		if (luettu == 0)
			break;
	}

	fclose(h_scoret);

	return highscoretaulu;
}

/* Mvoid
print_highscores()
{
	FILE	*h_scoret = NULL;
	size_t	 rivilkm = 0;
	size_t	*vertailtavat;
	size_t	 rivi;
	size_t	 i = 0;


	h_scoret = fopen(SCORES, "r");
	if (h_scoret == NULL) {
		fprintf(stderr, "Scoretiedoston avaus ep‰onnistui\n");
		exit(1);
	}

	rivilkm = get_linecount(h_scoret);

	vertailtavat = calloc(rivilkm, sizeof(size_t));
	if (vertailtavat == NULL)
		err(1, "muistin varaus ep‰onnistui");

	while (fscanf(h_scoret, "%zu,%s,%zu", aika, &nimi, &pisteet) != EOF) {
		vertailtavat[i] = rivi;
		i++;
	}
	qsort(vertailtavat, rivilkm, sizeof(vertailtavat[0]), sort_function);


//	fprintf(stdout, "High scoret: %zu\n", pojot);

	for (i = 0; i < rivilkm; i++) {
		fprintf(stdout, "%zu\n", vertailtavat[i]);
	}

	if (h_scoret != NULL)
		fclose(h_scoret);
}
*/
void
tallenna_scoret(size_t uudet_pisteet, struct high_score_st *vertailtavat)
{
	time_t	 aika;
	size_t	 i = 0;

	aika = time(NULL);

	vertailtavat[10].aika = (int)&aika;
	(void)strncpy(vertailtavat[10].nimi, "quest", 5);
	vertailtavat[10].pojot = uudet_pisteet;

	for (i = 0; i <= 10; i++) {
		fprintf(stdout, "vertailtavat+uusi %zu, %zu\n", vertailtavat[i].pojot, i);
	}

	qsort(vertailtavat, 11, sizeof(vertailtavat[0]), sort_function);
	for (i = 0; i <= 10; i++) {
		fprintf(stdout, "vertailtavat+uusi sorttauksen j‰lkeen %zu, %zu\n", vertailtavat[i].pojot, i);
	}


}

int
sort_function(const void *a, const void *b)
{
	struct high_score_st *ia = (struct high_score_st *)a;
	struct high_score_st *ib = (struct high_score_st *)b;

	return ib->pojot - ia->pojot;
}
