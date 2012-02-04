#ifndef _ENGLANTI_SUOMI_H_
#define _ENGLANTI_SUOMI_H_

#define SANA_TIEDOSTO "sanat.txt"
#define SCORES "scores.txt"
#define WORDLEN 128
#define KYS_LKM 8
#define PELILKM 10

struct sana_st {
	char eng[WORDLEN];
	char fin[WORDLEN];
};

struct high_score_st {
	time_t aika;
	size_t pojot;
	char   nimi[WORDLEN];
};



size_t pelikierros(struct sana_st *, size_t, size_t);
size_t get_filesize(void);
size_t get_linecount(FILE *);
size_t randint(size_t, size_t);
int numberexist(size_t *, size_t, size_t);
size_t* random_idx_arr(size_t);
struct sana_st wordsplitter(char *);
struct high_score_st *luo_highscoretaulu(void);
void print_sanaparit(struct sana_st *, size_t);
void print_vaihtoehdot(struct sana_st *, size_t *);
void tallenna_scoret(size_t, struct high_score_st *);
int sort_function(const void *, const void *);


#endif /* _ENGLANTI_SUOMI_H_ */
