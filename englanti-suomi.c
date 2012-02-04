#include <sys/stat.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <bsd/stdlib.h>
#include <termios.h>

#include "englanti-suomi.h"
#include "slist.h"

int
main(int argc, const char *argv[])
{
	FILE	*sanatiedosto = NULL;
	char	 rivi[BUFSIZ];
	int	 i = 0;
	int	 ch;
	size_t	 rivilkm = 0;
	size_t	 pisteet;
	struct sana_st *sanaparit;
	struct high_score_st *hightaulu;

	if (argc != 1) {
		while ((ch = getopt(argc, (char *const*)argv, "a:")) != -1) {
			switch (ch) {
			case 'a' :
				sanatiedosto = fopen(SANA_TIEDOSTO, "a"); /* avataan sanat.txt sanatiedosto kahvaan */
				fprintf(sanatiedosto, "%s=%s\n", argv[2], argv[3]);
				fclose(sanatiedosto);
				return 0;
			default:
				fprintf(stdout, "Virheellinen optio. (Sanaparin lis‰‰miseen k‰yt‰ optiota 'a'.)\n");
				return 1;
			}
		}
	}

	sanatiedosto = fopen(SANA_TIEDOSTO, "r"); /* avataan sanat.txt sanatiedosto kahvaan */

	if (sanatiedosto == NULL) {
		fprintf(stderr, "Tiedoston avaus ep√§onnistui\n");
		exit(1);
	}

	rivilkm = get_linecount(sanatiedosto); /*laskee sanatiedoston rivien lukum√§√§r√§n*/

	sanaparit = calloc(rivilkm, sizeof(struct sana_st)); /* varataan muistia sanalkm * sizeof(...) */

	if (sanaparit == NULL) /* NULL viittaa tyhj√§√§n eli muistipaikkoja ei ole olemassa */
		err(1, "muistin varaus ep√§onnistui");

	while (fgets(rivi, BUFSIZ, sanatiedosto) != NULL) {
		sanaparit[i] = wordsplitter(rivi);
		if (sanaparit[i].eng[0] == '\0' || sanaparit[i].fin[0] == '\0')
			err(3, "Apua apua, NULL rivill√§ %d\n", i+1);
		i++;
	}

	pisteet = pelikierros(sanaparit, PELILKM, rivilkm);
	fprintf(stdout, "Pisteesi: %zu/%zu\n", pisteet, PELILKM*20); /*maksimipisteet=PELILKM*20*/
	hightaulu = luo_highscoretaulu();
	tallenna_scoret(pisteet, hightaulu);
	fprintf(stdout, "\n");
	print_linked_list();
	fclose(sanatiedosto);

	free(sanaparit);
	sanaparit = NULL;

	free(hightaulu);
	hightaulu = NULL;

	return 0;
}

size_t
pelikierros(struct sana_st *sanaparit, size_t kierroslkm, size_t rivit)
{
	size_t	 i = 1;
	size_t	 pojot = 0;
	size_t	*vaihtoehdot_idx = NULL;
	size_t	*kysytyt = NULL;
	size_t   kysyttava;
	size_t	 vastaus_input;
	time_t	 start, end;
	double	 dif = 0;

	kysytyt = calloc(PELILKM, sizeof(size_t));

	while (i <= kierroslkm) {
		vaihtoehdot_idx = random_idx_arr(rivit);
		kysyttava = randint(0, KYS_LKM - 1);
		if (i > 1) {
			while (numberexist(kysytyt, i, vaihtoehdot_idx[kysyttava]) == 1) {
				vaihtoehdot_idx = random_idx_arr(rivit);
				kysyttava = randint(0, KYS_LKM - 1);
			}
		}
		kysytyt[i - 1] = vaihtoehdot_idx[kysyttava];
		fprintf(stdout, "Valitse seuraavalle suomenkielinen vastine (1-8)\n\n");
		fprintf(stdout, ">> %s\n\n",sanaparit[vaihtoehdot_idx[kysyttava]].eng);

		print_vaihtoehdot(sanaparit, vaihtoehdot_idx);
		fprintf(stdout, "\n[%zu/%zu] %zu > ", i, kierroslkm, pojot);
		time(&start);
		fscanf(stdin, "%zu", &vastaus_input);
		time(&end);
		dif = difftime(end, start);
		if (vastaus_input == (kysyttava + 1)) {
			fprintf(stdout, "Oikein, ");
			if (dif <= 3)
				pojot = pojot + 20;
			else if (dif <= 5)
				pojot = pojot + 10;
			else if (dif <= 6)
				pojot = pojot + 8;
			else if (dif <= 8)
				pojot = pojot + 6;
			else if (dif <= 10)
				pojot = pojot + 4;
			else if (dif <= 12)
				pojot = pojot + 2;
			else    pojot++;
		} else {
			fprintf(stdout, "V‰‰rin, ");
			insert_begining(&sanaparit[vaihtoehdot_idx[kysyttava]]);
			kierroslkm--;
		}
		fprintf(stdout, "%s = ", sanaparit[vaihtoehdot_idx[kysyttava]].eng);
		fprintf(stdout, "%s, (aika %g sek)\n\n", sanaparit[vaihtoehdot_idx[kysyttava]].fin, dif);

		vastaus_input = 0;
		i++;
	}
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
	return (arc4random() % (loppu + 1));

}

/* L√∂ydyyk√∂ numer 'vertailtava' taulukosta t.
 * Palauttaa 1, jos l√∂ytyy
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
random_idx_arr(size_t riveja) /* generoidaan satunnaislukuja (= valitaan satunnaisesti rivej√§ sanat.txt tiedostosta) */
{
	size_t	 i = 0;
	size_t	*t = NULL;

	t = calloc(KYS_LKM, sizeof(size_t));
	if (t == NULL)
		err(1, "malloc failed in func: random_idx_arr()");

	for (i = 0; i < KYS_LKM; i++) {
		t[i] = randint(1, riveja - 1);
		while ((numberexist(t, i, t[i])) == 1) {
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

	for (i = 0; i < KYS_LKM; i++) {
		fprintf(stdout, "%2zu. %s\n", i + 1, sp[idx[i]].fin); /* sp == sanaparit, idx == vaihtoehdot_idx */
	}
}

struct high_score_st *
luo_highscoretaulu(void)
{
	FILE			*h_scoret = NULL;
	struct high_score_st	*highscoretaulu;
	struct stat		 st;
	size_t			 i = 0;

	highscoretaulu = calloc(11, sizeof(struct high_score_st));
	if (highscoretaulu == NULL) /* NULL viittaa tyhj√§√§n eli muistipaikkoja ei ole olemassa */
		err(1, "muistinvaraus ep‰onnistui");

	if (stat(SCORES, &st) == -1) { /* Tiedostoa ei l√∂ydy */
		h_scoret = fopen(SCORES, "w");
		if (h_scoret == NULL) {
			err(10, "tiedostoa ei lˆytynyt %s", SCORES);
		}
		for (i = 0; i < 11; i++) {
			fputs(" , ,lsd\n", h_scoret);
		}
		fclose(h_scoret);
	}

	h_scoret = fopen(SCORES, "r");
	if (h_scoret == NULL) {
		err(10, "tiedostoa ei lˆytynyt %s", SCORES);
	}
	for (i = 0; i < 10; i++) {
		size_t luettu = 0;

		luettu = fscanf(h_scoret, "%zu,%zu,%s", &highscoretaulu[i].aika, &highscoretaulu[i].pojot, &highscoretaulu[i].nimi);
		if (luettu == 0)
			break;
	}

	fclose(h_scoret);

	return highscoretaulu;
}

void
tallenna_scoret(size_t uudet_pisteet, struct high_score_st *vertailtavat)
{
	FILE	 *h_scoret;
	size_t	 i = 0;

	vertailtavat[10].aika = time(NULL);
	(void)strncpy(vertailtavat[10].nimi, "quest", 5);
	vertailtavat[10].pojot = uudet_pisteet;

	qsort(vertailtavat, 11, sizeof(vertailtavat[0]), sort_function);

	h_scoret = fopen(SCORES, "w");
	if (h_scoret == NULL) {
		fprintf(stderr, "Scoretiedoston avaus ep‰onnistui\n");
		fclose(h_scoret);
		exit(1);
	}

	for (i = 0; i < 10; i++) {
		fprintf(h_scoret, "%zu,%zu,%s\n", vertailtavat[i].aika, vertailtavat[i].pojot, vertailtavat[i].nimi);
		fprintf(stdout, "%zu  %s  %s",  vertailtavat[i].pojot, vertailtavat[i].nimi,asctime(localtime(&vertailtavat[i].aika)));
	}
	fclose(h_scoret);
}

int
sort_function(const void *a, const void *b)
{
	struct high_score_st *ia = (struct high_score_st *)a;
	struct high_score_st *ib = (struct high_score_st *)b;

	return ib->pojot - ia->pojot;
}

