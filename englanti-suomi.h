#ifndef _ENGLANTI_SUOMI_H_
#define _ENGLANTI_SUOMI_H_

#define SANA_TIEDOSTO "sanat.txt"
#define WORDLEN 128
#define KYS_LKM 6

struct sana {
	char eng[WORDLEN];
	char fin[WORDLEN];
};

size_t get_filesize(void);
size_t get_linecount(FILE *);
size_t randint(size_t, size_t);
size_t* random_idx_arr(size_t);
struct sana wordsplitter(char *);
void print_sanaparit(struct sana *, size_t);

#endif /* _ENGLANTI_SUOMI_H_ */

