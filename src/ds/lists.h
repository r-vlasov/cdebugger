#ifndef _DS_LISTS_H
#define _DS_LISTS_H

#include <stdlib.h>

typedef struct list_node {
	void			*value;	
	struct list_node	*prev;
} list_node_t;

typedef struct list {
	size_t 		nodes;
	list_node_t	*head;
} list_t;




	list_t*		list_create();
	list_node_t*	list_node_create(void* value);
	void		list_add(list_t* list, void* value);
	void 		list_set_head(list_t* list, void* value);
	list_node_t*	list_search(list_t* list, void* value, int (*comparator)(void*, void*));
	void		list_remove(list_t* list);


#endif
