#include "lists.h"

list_t* 	list_create()
{
	list_t* res = (list_t*)malloc(sizeof(list_t));
	res->head = NULL;
	res->nodes = 0;
	return res;
}


list_node_t*	list_node_create(void* value)
{
	list_node_t* node = (list_node_t*)malloc(sizeof(list_node_t));
	node->value = value;
	node->prev = NULL;
	return node;
}


void 		list_set_head(list_t* list, void* value)
{
	list_node_t* lhead = list_node_create(value);
	list->head = lhead;
	list->head->prev = NULL;
	list->nodes = 1;
}


void		list_add(list_t* list, void* value)
{
	if(list->nodes)
	{
		list_node_t* node = list_node_create(value);
		list_node_t* i;
		for(i = list->head; i->prev != NULL; i = i->prev);
		i->prev = node;
		list->nodes++;
	}
	else
	{
		list_set_head(list, value);
	}
}


list_node_t*	list_search(list_t* list, void* value, int (*comparator)(void*, void*))
{
	if(list->nodes)
	{
		for(list_node_t* i = list->head; i != NULL; i = i->prev)
			if (comparator(value, i->value))
				return i;
		return NULL;
	}
	else
	{
		return NULL;
	}
}


void 		list_remove(list_t* list)
{
	if(list->nodes)
	{
		list_node_t* temp = list->head;
		while(temp != NULL)
		{
			temp = temp->prev;
			free(list->head);
			list->head = temp;
		}
	}
	free(list);
}
