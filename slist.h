#ifndef _SLIST_H_
#define _SLIST_H_

#include "englanti-suomi.h"

struct	node {
	struct sana_st virhepari;
	struct node *next;
};

#endif /* _SLIST_H_ */
