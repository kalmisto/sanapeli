#ifndef _ENGLANTI_SUOMI_H_
#define _ENGLANTI_SUOMI_H_

#define SANA_TIEDOSTO "/home/kalle/bin/programs/sanastoja/sanoja.txt"

struct sana {
	char suomi[128];
	char eng[128];
};

size_t get_filesize(void);
size_t get_linecount(FILE *);

#endif /* _ENGLANTI_SUOMI_H_ */

