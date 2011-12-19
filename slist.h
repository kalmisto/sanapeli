#ifndef _SLIST_H_
#define _SLIST_H_

#include "englanti-suomi.h"

struct	node {
	struct sana_st virhepari;
	struct node *next;
};

void print_linked_list(void);
void insert_begining(struct sana_st *);
void remove_begining(void);
#endif /* _SLIST_H_ */
