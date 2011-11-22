#ifndef _ENGLANTI_SUOMI_H_
#define _ENGLANTI_SUOMI_H_

#define SANA_TIEDOSTO "sanat.txt"
#define SCORES "scores.txt"
#define WORDLEN 128
#define KYS_LKM 8
#define PELILKM 10

struct sana {
	char eng[WORDLEN];
	char fin[WORDLEN];
};

size_t pelikierros(struct sana *, size_t, size_t);
size_t get_filesize(void);
size_t get_linecount(FILE *);
size_t randint(size_t, size_t);
size_t* random_idx_arr(size_t);
int numberexist(size_t *, size_t, size_t);
struct sana wordsplitter(char *);
void print_sanaparit(struct sana *, size_t);
void print_vaihtoehdot(struct sana *, size_t *);
void print_highscores();
void tallenna_scoret(size_t);

#endif /* _ENGLANTI_SUOMI_H_ */
