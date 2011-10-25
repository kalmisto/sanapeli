#ifndef _ENGLANTI_SUOMI_H_
#define _ENGLANTI_SUOMI_H_

#define SANA_TIEDOSTO "/home/kalle/bin/programs/sanastoja/sanoja.txt"

struct sana {
	char eng[128];
	char fin[128];
};

size_t get_filesize(void);
size_t get_linecount(FILE *);
void print_sanaparit(struct sana *, size_t);

#endif /* _ENGLANTI_SUOMI_H_ */

