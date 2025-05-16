#include <stdio.h>

#include "system.h"

#define TRUE 1
#define FALSE 0

extern tree* structure;

void pwd(tree* structure, stack* newstk)
{
	dir* temp = structure->current;
	if(temp->parent == NULL)
	{
		printf("/\n");
			
		return;
	}

	while(temp->parent != NULL);
	{
		push(newstk, temp->name);
		temp = temp->parent;
	}
	printf("/");
	while(1)
	{
		if(check_stack_empty(newstk) == TRUE) {
			break;
		}
		else
		{
			printf("%s/", newstk->datas[newstk->top]);
			pop(newstk);
		}
	}
	printf("\n");

}
